#include "type.h"
#include "scope.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

#include <utility/helpfunctions.h>

namespace entity {

    /**
     * @brief Type::Type
     */
    Type::Type()
        : Type(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    /**
     * @brief Type::Type
     * @param src
     */
    Type::Type(Type &&src)
    {
       moveFrom(src);
    }

    /**
     * @brief Type::Type
     * @param src
     */
    Type::Type(const Type &src)
    {
        copyFrom(src);
    }

    /**
     * @brief Type::Type
     * @param name
     * @param scopeId
     * @param typeId
     */
    Type::Type(const QString &name, const QString &scopeId, const QString &typeId)
        : m_KindOfType(BasicType)
        , m_Name(name)
        , m_Id(typeId.isEmpty() ? utility::genId() : typeId)
        , m_ScopeId(scopeId)
    {
    }

    /**
     * @brief Type::~Type
     */
    Type::~Type()
    {
    }

    /**
     * @brief Type::operator =
     * @param rhs
     * @return
     */
    Type &Type::operator =(Type &&rhs)
    {
       if (this != &rhs)
          moveFrom(rhs);

       return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Type &lhs, const Type &rhs)
    {
        return lhs.m_KindOfType == rhs.m_KindOfType &&
               lhs.m_Name       == rhs.m_Name       &&
               lhs.m_Id         == rhs.m_Id         &&
               lhs.m_ScopeId    == rhs.m_ScopeId;
    }

    /**
     * @brief Type::operator =
     * @param rhs
     * @return
     */
    Type &Type::operator =(Type rhs)
    {
        moveFrom(rhs);

        return *this;
    }

    /**
     * @brief Type::name
     * @return
     */
    QString Type::name() const
    {
        return m_Name;
    }

    /**
     * @brief Type::setName
     * @param name
     */
    void Type::setName(const QString &name)
    {
        m_Name = name;
    }

    /**
     * @brief Type::id
     * @return
     */
    QString Type::id() const
    {
        return m_Id;
    }

    /**
     * @brief Type::setId
     * @param id
     */
    void Type::setId(const QString &id)
    {
        m_Id = id;
    }

    /**
     * @brief Type::scopeId
     * @return
     */
    QString Type::scopeId() const
    {
        return m_ScopeId;
    }

    /**
     * @brief Type::setScopeId
     * @param scopeId
     */
    void Type::setScopeId(const QString &scopeId)
    {
        m_ScopeId = scopeId;
    }

    /**
     * @brief Type::type
     * @return
     */
    UserType Type::type() const
    {
        return m_KindOfType;
    }

    /**
     * @brief Type::toJson
     * @return
     */
    QJsonObject Type::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("Scope ID", m_ScopeId);
        result.insert("ID", m_Id);
        result.insert("Kind of type", m_KindOfType);

        return result;
    }

    /**
     * @brief Type::fromJson
     * @param src
     * @param errorList
     */
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

    /**
     * @brief Type::writeToFile
     * @param fileName
     */
    void Type::writeToFile(const QString &fileName) const
    {
        utility::writeToFile(*this, fileName);
    }

    /**
     * @brief Type::readFromFile
     * @param fileName
     * @return
     */
    bool Type::readFromFile(const QString &fileName)
    {
        return utility::readFromFile(*this, fileName);
    }

    /**
     * @brief Type::clone
     * @return
     */
    Type *Type::clone() const
    {
        return new Type(*this);
    }

    /**
     * @brief Type::isEqual
     * @param rhs
     * @return
     */
    bool Type::isEqual(const Type &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Type::moveFrom
     * @param src
     */
    void Type::moveFrom(Type &src)
    {
        m_KindOfType = std::move(src.m_KindOfType); // not necessary
        m_Name       = std::move(src.m_Name);
        m_Id         = std::move(src.m_Id);
        m_ScopeId    = std::move(src.m_ScopeId);
    }

    /**
     * @brief Type::copyFrom
     * @param src
     */
    void Type::copyFrom(const Type &src)
    {
        m_KindOfType = src.m_KindOfType;
        m_Name = src.m_Name;
        m_Id = src.m_Id;
        m_ScopeId = src.m_ScopeId;
    }

} // namespace entity
