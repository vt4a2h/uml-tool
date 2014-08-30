#include "database.h"
#include "helpfunctions.h"
#include "scope.h"
#include "constants.cpp"

#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>

namespace db {

    Database::Database(Database &&src)
    {
        moveFrom(src);
    }

    Database::Database(const Database &src)
    {
        copyFrom(src);
    }

    Database::Database(const QString &name, const QString &path)
        : m_Name(name.isEmpty() ? DEFAULT_DATABASE_NAME : name)
        , m_Path(path.isEmpty() ? DEFAULT_DATABASE_PATH : QDir::currentPath())

    {
    }

    Database::~Database()
    {
    }

    Database &Database::operator =(Database &&rhs)
    {
        if (this != &rhs)
            moveFrom(rhs);

        return *this;
    }

    Database &Database::operator =(Database rhs)
    {
        moveFrom(rhs);
        return *this;
    }

    QString Database::path() const
    {
        return m_Path;
    }

    void Database::setPath(const QString &path)
    {
        m_Path = path.simplified();
    }

    QString Database::name() const
    {
        return m_Name;
    }

    void Database::setName(const QString &name)
    {
        m_Name = name;
    }

    entity::SharedScope Database::getScope(const QString &id) const
    {
        return (m_Scopes.contains(id) ? m_Scopes[id] : nullptr);
    }

    entity::SharedScope Database::addScope(const QString &name, const QString &parentScopeId)
    {
        entity::SharedScope scope(nullptr);

        if (parentScopeId.isEmpty()) {
            scope = std::make_shared<entity::Scope>(name, GLOBAL_SCOPE_ID);
            m_Scopes.insert(scope->id(), scope);
        } else {
            auto searchResults = std::move(makeDepthIdList(parentScopeId));
            if (!searchResults.isEmpty()) {
                auto depthScope = getScopeWithDepthList(searchResults);
                if (depthScope) scope = depthScope->addChildScope(name);
            }
        }

        return scope;
    }

    bool Database::containsScope(const QString &id) const
    {
        return m_Scopes.contains(id);
    }

    bool Database::anyScopes() const
    {
        return !m_Scopes.isEmpty();
    }

    void Database::removeScope(const QString &id)
    {
        m_Scopes.remove(id);
    }

    entity::ScopesList Database::scopes()
    {
        return m_Scopes.values();
    }

    entity::SharedScope Database::depthScopeSearch(const QString &scopeId) const
    {
        return getScopeWithDepthList(makeDepthIdList(scopeId));
    }

    entity::SharedType Database::depthTypeSearch(const QString &typeId) const
    {
        entity::SharedType result(nullptr);

        for (auto scope : m_Scopes.values()) {
            getDepthType(scope, typeId, result);
            if (result) break;
        }

        return result;
    }

    QStringList Database::makeDepthIdList(const QString &id) const
    {
        QStringList result;

        if (containsScope(id)) {
            result << id;
        } else {
            for (auto scope : m_Scopes.values()) {
                recursiveFind(scope, id, result);
                if (!result.empty()) break;
            }
        }

        return result;
    }

    entity::SharedScope Database::getScopeWithDepthList(const QStringList &ids) const
    {
        entity::SharedScope result(nullptr);
        if (!ids.isEmpty() && containsScope(ids[0])) {
            result = getScope(ids[0]);
            for (int i = 1, size = ids.size(); i < size; ++i)
                if (result->containsChildScope(ids[i]))
                    result = result->getChildScope(ids[i]);
                else
                    break;
        }

        return result;
    }

    void Database::getDepthType(const entity::SharedScope &scope, const QString &id, entity::SharedType &result) const
    {
        if (scope->containsType(id)) {
            result = scope->getType(id);
            return;
        } else if (scope->hasChildScopes()){
            for (auto child_scope : scope->scopes()) {
                if (result) break;
                getDepthType(child_scope, id, result);
            }
        }

    }

    void Database::load(QStringList &errorList)
    {
        QFile f(makeFullPath());
        if (f.open(QIODevice::WriteOnly)) {
            QJsonParseError errorMessage;
            QJsonDocument jdoc(QJsonDocument::fromJson(f.readAll(), &errorMessage));

            if (errorMessage.error == QJsonParseError::NoError) {
                fromJson(jdoc.object(), errorList);
            } else {
                errorList << errorMessage.errorString();
            }
        } else {
            errorList << QObject::tr("Can't load database: %1.").arg(f.fileName());
        }
    }

    void Database::clear()
    {
        m_Scopes.clear();
    }

    void Database::save() const
    {
        QFile f(makeFullPath());
        if (f.open(QIODevice::WriteOnly)) {
            QJsonDocument jdoc(toJson());
            QTextStream stream(&f);
            stream << jdoc.toJson();
        }
    }

    QJsonObject Database::toJson() const
    {
        QJsonArray scopes;
        for (auto &&scope : m_Scopes.values()) scopes.append(scope->toJson());

        QJsonObject result;
        result.insert("Scopes", scopes);

        return result;
    }

    void Database::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        clear();
        utility::checkAndSet(src, "Scopes", errorList, [&src, &errorList, this](){
            if (src["Scopes"].isArray()) {
                entity::SharedScope scope;
                for (auto &&val : src["Scopes"].toArray()) {
                    scope = std::make_shared<entity::Scope>();
                    scope->fromJson(val.toObject(), errorList);
                    m_Scopes.insert(scope->id(), scope);
                }
            } else {
                errorList << "Error: \"Scopes\" is not array";
            }
        });
    }

    void Database::moveFrom(Database &src)
    {
        m_Name = std::move(src.m_Name);
        m_Path = std::move(src.m_Path);

        m_Scopes = std::move(src.m_Scopes);
    }

    void Database::copyFrom(const Database &src)
    {
        m_Name = src.m_Name;
        m_Path = src.m_Path;

        utility::deepCopySharedPointerHash(src.m_Scopes, m_Scopes, &entity::Scope::id);
    }

    QString Database::makeFullPath() const
    {
        return QString("%1%2%3.%4")
                .arg(m_Path,
                     m_Path.isEmpty() ? "" : "/",
                     m_Name.toLower(),
                     DEFAULT_DATABASE_EXTENSION);
    }

    void Database::recursiveFind(entity::SharedScope scope, const QString &id, QStringList &ids) const
    {
        if (scope->scopes().isEmpty())  {
            ids.clear();
            return;
        }

        ids << scope->id();

        if (scope->containsChildScope(id)) {
            ids << scope->getChildScope(id)->id();
            return;
        } else {
            for (auto sc : scope->scopes()) recursiveFind(sc, id, ids);
        }
    }

} // namespace db
