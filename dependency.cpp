#include "dependency.h"
#include "classmethod.h"
#include "class.h"
#include "enums.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QJsonObject>

namespace relationship {

    Dependency::Dependency()
        : Dependency(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    Dependency::Dependency(const QString &tailTypeId, const QString &headTypeId, const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase)
        : Relation(tailTypeId, headTypeId, globalDatabase, projectDatabase)
    {
        m_RelationType = DependencyRelation;
    }

    void Dependency::make()
    {
        m_TailClass->addMethod(m_Method);
    }

    void Dependency::clear()
    {
        m_TailClass->removeMethod(m_Method);
    }

    entity::SharedMethod Dependency::method() const
    {
        return m_Method;
    }

    void Dependency::setMethod(const entity::SharedMethod &method)
    {
        m_Method = method;
    }

    QJsonObject Dependency::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Method", m_Method->toJson());

        return result;
    }

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

} // namespace relationship
