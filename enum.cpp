#include "enum.h"
#include "constants.cpp"
#include "helpfunctions.h"

#include <algorithm>
#include <utility>

#include <QJsonObject>
#include <QStringList>
#include <QJsonArray>

namespace entity {

    Enum::Enum()
        : Enum(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Enum::Enum(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
        , m_EnumTypeId(STUB_ID)
        , m_StrongStatus(false)
    {
    }

    bool Enum::isStrong() const
    {
        return m_StrongStatus;
    }

    void Enum::setStrongStatus(bool status)
    {
        m_StrongStatus = status;
    }

    Enum::Variable &Enum::addVariable(const QString &name)
    {
        return *m_Variables.insert(name, std::make_pair(name, m_Variables.size()));
    }

    Enum::Variable Enum::getVariable(const QString &name) const
    {
        return m_Variables.value(name);
    }

    void Enum::removeVariable(const QString &name)
    {
        m_Variables.remove(name);
    }

    bool Enum::containsVariable(const QString &name)
    {
        return m_Variables.contains(name);
    }

    Enum::VariablesList Enum::variables() const
    {
        return m_Variables.values();
    }

    QString Enum::enumTypeId() const
    {
        return m_EnumTypeId;
    }

    void Enum::setEnumTypeId(const QString &enumTypeId)
    {
        m_EnumTypeId = enumTypeId;
    }

    QJsonObject Enum::toJson() const
    {
        QJsonObject result(Type::toJson());

        result.insert("Enum type id", m_EnumTypeId);
        result.insert("Strong status", m_StrongStatus);

        QJsonArray variables;
        QJsonObject variable;
        for (auto name : m_Variables.keys()) {
            variable.insert("Name", m_Variables[name].first);
            variable.insert("Number", m_Variables[name].second);
            variables.append(variable);
        }
        result.insert("Variables", variables);

        return result;
    }

    void Enum::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Type::fromJson(src, errorList);

        utility::checkAndSet(src, "Enum type id",  errorList, [&src, this](){ m_EnumTypeId   = src["Enum type id"].toString(); });
        utility::checkAndSet(src, "Strong status", errorList, [&src, this](){ m_StrongStatus = src["Strong status"].toBool();  });

        m_Variables.clear();
        utility::checkAndSet(src, "Variables", errorList, [&src, &errorList, this](){
            if (src["Variables"].isArray()) {
                QJsonObject obj;
                Variable var;
                for (auto value : src["Variables"].toArray()) {
                    obj = value.toObject();
                    utility::checkAndSet(obj, "Name",   errorList, [&obj, &var, this](){ var.first  = obj["Name"].toString(); });
                    utility::checkAndSet(obj, "Number", errorList, [&obj, &var, this](){ var.second = obj["Number"].toInt();  });
                    m_Variables.insert(var.first, var);
                  }
            } else {
                errorList << "Error: \"Varibles\" is not array";
            }
        });
    }


} // namespace entity
