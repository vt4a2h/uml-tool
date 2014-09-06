#include "type.h"
#include "scope.h"
#include "helpfunctions.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

namespace entity {

    Type::Type()
        : Type(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Type::Type(Type &&src)
    {
       moveFrom(src);
    }

    Type::Type(const Type &src)
    {
        copyFrom(src);
    }

    Type::Type(const QString &name, const QString &scopeId)
        : m_KindOfType(BasicType)
        , m_Name(name)
        , m_Id(utility::genId())
        , m_ScopeId(scopeId)
    {
    }

    Type::~Type()
    {
    }

    Type &Type::operator =(Type &&rhs)
    {
       if (this != &rhs)
          moveFrom(rhs);

       return *this;
    }

    bool operator ==(const Type &lhs, const Type &rhs)
    {
        return lhs.m_KindOfType == rhs.m_KindOfType &&
               lhs.m_Name       == rhs.m_Name       &&
               lhs.m_Id         == rhs.m_Id         &&
               lhs.m_ScopeId    == rhs.m_ScopeId;
    }

    Type &Type::operator =(Type rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    QString Type::name() const
    {
        return m_Name;
    }

    void Type::setName(const QString &name)
    {
        m_Name = name;
    }

    QString Type::id() const
    {
        return m_Id;
    }

    void Type::setId(const QString &id)
    {
        m_Id = id;
    }

    QString Type::scopeId() const
    {
        return m_ScopeId;
    }

    void Type::setScopeId(const QString &scopeId)
    {
        m_ScopeId = scopeId;
    }

    UserType Type::type() const
    {
        return m_KindOfType;
    }

    QJsonObject Type::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("Scope ID", m_ScopeId);
        result.insert("ID", m_Id);
        result.insert("Kind of type", m_KindOfType);

        return result;
    }

    void Type::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){
            m_Name = src["Name"].toString();
        });
        utility::checkAndSet(src, "Scope ID", errorList, [&src, this](){
            m_ScopeId = src["Scope ID"].toString();
        });
        utility::checkAndSet(src, "ID", errorList, [&src, this](){
            m_Id = src["ID"].toString();
        });
        utility::checkAndSet(src, "Kind of type", errorList, [&src, this](){
            m_KindOfType = static_cast<UserType>(src["Kind of type"].toInt());
        });
    }

    void Type::writeToFile(const QString &fileName) const
    {
        QFile jsonFile(fileName);

        if (jsonFile.open(QIODevice::WriteOnly)) {
            QJsonDocument jdoc(toJson());
            QTextStream st(&jsonFile);
            st << jdoc.toJson();
        }
    }

    bool Type::readFromFile(const QString &fileName)
    {
        QFile jsonFile(fileName);
        if (jsonFile.open(QIODevice::ReadOnly)) {
            QJsonParseError errorMessage;
            auto jdoc = QJsonDocument::fromJson(jsonFile.readAll(), &errorMessage);

            if (errorMessage.error == QJsonParseError::NoError) {
                ErrorList errorList;

                if (jdoc.isObject()) {
                    QJsonObject object = jdoc.object();

                    fromJson(object, errorList);

                    if (errorList.isEmpty())
                        return true;
                }
            }
        }

        return false;
    }

    Type *Type::clone() const
    {
        return new Type(*this);
    }

    bool Type::isEqual(const Type &rhs) const
    {
        return *this == rhs;
    }

    void Type::moveFrom(Type &src)
    {
        m_KindOfType = std::move(src.m_KindOfType); // not necessary
        m_Name       = std::move(src.m_Name);
        m_Id         = std::move(src.m_Id);
        m_ScopeId    = std::move(src.m_ScopeId);
    }

    void Type::copyFrom(const Type &src)
    {
        m_KindOfType = src.m_KindOfType;
        m_Name = src.m_Name;
        m_Id = src.m_Id;
        m_ScopeId = src.m_ScopeId;
    }

} // namespace entity
