#include "dependency.h"
#include "enums.h"
#include "constants.cpp"

#include <QJsonObject>

#include <entity/class.h>
#include <entity/classmethod.h>
#include <utility/helpfunctions.h>

namespace relationship {

    /**
     * @brief Dependency::Dependency
     */
    Dependency::Dependency()
        : Dependency(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    /**
     * @brief Dependency::Dependency
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Dependency::Dependency(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
        : Relation(tailTypeId, headTypeId, globalDatabase, projectDatabase)
    {
        m_RelationType = DependencyRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Dependency &lhs, const Dependency &rhs)
    {
        return static_cast<const Relation&>(lhs).isEqual(rhs) &&
               (lhs.m_Method == rhs.m_Method || *lhs.m_Method == *rhs.m_Method);
    }

    /**
     * @brief Dependency::make
     */
    void Dependency::make()
    {
        m_TailClass->addMethod(m_Method);
    }

    /**
     * @brief Dependency::clear
     */
    void Dependency::clear()
    {
        m_TailClass->removeMethod(m_Method);
    }

    /**
     * @brief Dependency::method
     * @return
     */
    entity::SharedMethod Dependency::method() const
    {
        return m_Method;
    }

    /**
     * @brief Dependency::setMethod
     * @param method
     */
    void Dependency::setMethod(const entity::SharedMethod &method)
    {
        m_Method = method;
    }

    /**
     * @brief Dependency::toJson
     * @return
     */
    QJsonObject Dependency::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Method", m_Method->toJson());

        return result;
    }

    /**
     * @brief Dependency::fromJson
     * @param src
     * @param errorList
     */
    void Dependency::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Relation::fromJson(src, errorList);

        utility::checkAndSet(src, "Method", errorList, [this, &src, &errorList](){
            QJsonObject obj = src["Method"].toObject();
            utility::checkAndSet(obj, "Type", errorList, [this, &obj, &errorList]{
                m_Method = utility::makeMethod(static_cast<entity::ClassMethodType>(obj["Type"].toInt()));
                m_Method->fromJson(obj, errorList);
            });
        });
    }

    /**
     * @brief Dependency::isEqual
     * @param rhs
     * @return
     */
    bool Dependency::isEqual(const Dependency &rhs) const
    {
        return *this == rhs;
    }

} // namespace relationship
