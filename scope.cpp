#include "scope.h"
#include "type.h"
#include "class.h"
#include "templateclass.h"
#include "union.h"
#include "enum.h"
#include "enums.h"
#include "extendedtype.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <utility>

#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

namespace entity {

    Scope::Scope(const QString &scopeName, const QString &scopeId)
        : m_Name(!scopeName.isEmpty() ? scopeName : DEFAULT_NAME)
        , m_Id(utility::genId())
        , m_ParentScopeId(!scopeId.isEmpty() ? scopeId : GLOBAL_SCOPE_ID)
    {
    }

    QString Scope::name() const
    {
        return m_Name;
    }

    void Scope::setName(const QString &name)
    {
        m_Name = name;
    }

    SharedType Scope::getType(const QString &typeId) const
    {
        return (m_Types.contains(typeId) ? m_Types[typeId] : nullptr);
    }

    SharedType Scope::takeType(const QString &typeId)
    {
        SharedType result(nullptr);

        if (m_Types.contains(typeId)) {
            result = m_Types[typeId];
            m_Types.remove(typeId);
        }

        return result;
    }

    bool Scope::containsType(const QString &typeId) const
    {
        return m_Types.contains(typeId);
    }

    void Scope::removeType(const QString &typeId)
    {
        m_Types.remove(typeId);
    }

    TypesList Scope::types() const
    {
        return m_Types.values();
    }

    SharedExtendedType Scope::getExtendedType(const QString &typeId) const
    {
        return (m_ExtendedTypes.contains(typeId) ? m_ExtendedTypes[typeId] : nullptr);
    }

    SharedExtendedType Scope::takeExtendedType(const QString &typeId)
    {
        SharedExtendedType result(nullptr);

        if (m_ExtendedTypes.contains(typeId)) {
            result = m_ExtendedTypes[typeId];
            m_ExtendedTypes.remove(typeId);
        }

        return result;
    }

    SharedExtendedType Scope::addExtendedType()
    {
        auto type = std::make_shared<ExtendedType>();
        type->setScopeId(m_Id);

        return *m_ExtendedTypes.insert(type->id(), type);
    }

    bool Scope::containsExtendedType(const QString &typeId) const
    {
        return m_ExtendedTypes.contains(typeId);
    }

    void Scope::removeExtendedType(const QString &typeId)
    {
        m_ExtendedTypes.remove(typeId);
    }

    ExtendedTypesList Scope::extendedTypes() const
    {
        return m_ExtendedTypes.values();
    }

    SharedScope Scope::getChildScope(const QString &typeId)
    {
        return (m_Scopes.contains(typeId) ? m_Scopes[typeId] : nullptr);
    }

    SharedScope Scope::takeChildScope(const QString &typeId)
    {
        SharedScope result(nullptr);

        if (m_Scopes.contains(typeId)) {
            result = m_Scopes[typeId];
            m_Scopes.remove(typeId);
        }

        return result;
    }

    SharedScope Scope::addChildScope(const QString &name)
    {
        SharedScope scope = std::make_shared<Scope>(name, m_Id);
        m_Scopes.insert(scope->id(), scope);
        return scope;
    }

    bool Scope::containsChildScope(const QString &typeId)
    {
        return m_Scopes.contains(typeId);
    }

    bool Scope::hasChildScopes() const
    {
        return !m_Scopes.isEmpty();
    }

    void Scope::removeChildScope(const QString &typeId)
    {
        m_Scopes.remove(typeId);
    }

    ScopesList Scope::scopes() const
    {
        return m_Scopes.values();
    }

    QString Scope::id() const
    {
        return m_Id;
    }
    
    void Scope::setId(const QString &id)
    {
        m_Id = id;
    }

    QString Scope::parentScopeId() const
    {
        return m_ParentScopeId;
    }
    
    void Scope::setParentScopeId(const QString &parentScopeId)
    {
        m_ParentScopeId = parentScopeId;
    }

    QJsonObject Scope::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("ID", m_Id);
        result.insert("Parent ID", m_ParentScopeId);

        QJsonArray scopes;
        for (auto scope : m_Scopes.values()) scopes.append(scope->toJson());
        result.insert("Scopes", scopes);

        QJsonArray types;
        for (auto type : m_Types.values()) types.append(type->toJson());
        result.insert("Types", types);

        QJsonArray extTypes;
        for (auto extType : m_ExtendedTypes.values()) extTypes.append(extType->toJson());
        result.insert("Extended types", extTypes);

        return result;
    }

    void Scope::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){ m_Name = src["Name"].toString(); });
        utility::checkAndSet(src, "ID", errorList, [&src, this](){ m_Id = src["ID"].toString(); });
        utility::checkAndSet(src, "Parent ID", errorList, [&src, this](){ m_ParentScopeId = src["Parent ID"].toString(); });

        m_Scopes.clear();
        utility::checkAndSet(src, "Scopes", errorList, [&src, &errorList, this](){
            if (src["Scopes"].isArray()) {
                SharedScope scope;
                for (auto val : src["Scopes"].toArray()) {
                    scope = std::make_shared<Scope>();
                    scope->fromJson(val.toObject(), errorList);
                    m_Scopes.insert(scope->id(), scope);
                }
            } else {
                errorList << "Error: \"Scopes\" is not array";
            }
        });

        m_Types.clear();
        utility::checkAndSet(src, "Types", errorList, [&src, &errorList, this](){
            if (src["Scopes"].isArray()) {
                SharedType type;
                QJsonObject obj;
                for (auto val : src["Scopes"].toArray()) {
                    obj = val.toObject();
                    utility::checkAndSet(obj, "Kind of type", errorList,
                                         [&obj, &type, &errorList, this](){
                        type = makeType(static_cast<UserType>(obj["Kind of type"].toInt()));
                        type->fromJson(obj, errorList);
                        m_Types.insert(type->id(), type);
                    });
                }
            } else {
                errorList << "Error: \"Types\" is not array";
            }
        });

        m_ExtendedTypes.clear();
        utility::checkAndSet(src, "Extended types", errorList, [&src, &errorList, this](){
            if (src["Extended types"].isArray()) {
                SharedExtendedType type;
                for (auto val : src["Extended types"].toArray()) {
                    type = std::make_shared<ExtendedType>();
                    type->fromJson(val.toObject(), errorList);
                    m_ExtendedTypes.insert(type->id(), type);
                }
            } else {
                errorList << "Error: \"Extended types\" is not array";
            }
        });
    }

    SharedType Scope::makeType(UserType type) const
    {
        switch (type) {
            case BasicType:
                return std::make_shared<Type>();
            case UserClassType:
                return std::make_shared<Class>();
            case TemplateClassType:
                return std::make_shared<TemplateClass>();
            case UnionType:
                return std::make_shared<Union>();
            case EnumType:
                return std::make_shared<Enum>();
            default:
                return std::make_shared<Type>();
        }
    }
    
} // namespace entity
