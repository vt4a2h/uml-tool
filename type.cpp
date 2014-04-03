#include "type.h"
#include "scope.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QJsonObject>

namespace entity {

    Type::Type()
        : Type(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Type::Type(const QString &name, const QString &scopeId)
        : m_Name(name)
        , m_Id(utility::genId())
        , m_ScopeId(scopeId)
    {
    }

    Type::~Type()
    {
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

    QJsonObject Type::toJson() const
    {
        QJsonObject result;

        result.insert("Name", m_Name);
        result.insert("Scope ID", m_ScopeId);
        result.insert("ID", m_Id);

        return result;
    }

    void Type::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Name", errorList, [&src, this](){ m_Name = src["Name"].toString(); });
        utility::checkAndSet(src, "Scope ID", errorList, [&src, this](){ m_ScopeId = src["Scope ID"].toString(); });
        utility::checkAndSet(src, "ID", errorList, [&src, this](){ m_Id = src["ID"].toString(); });
    }
        
} // namespace entity
