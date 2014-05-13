#include "realization.h"
#include "classmethod.h"
#include "class.h"
#include "enums.h"
#include "constants.cpp"
#include "helpfunctions.h"

#include <QJsonObject>
#include <QJsonArray>

namespace relationship {

    Realization::Realization()
        : Realization(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    Realization::Realization(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
        : Generalization(tailTypeId, headTypeId, globalDatabase, projectDatabase)
    {
        m_RelationType = RealizationRelation;
    }

    void Realization::addMethods(const entity::MethodsList &methods)
    {
        m_Methods = methods;
    }

    entity::MethodsList Realization::methods() const
    {
        return m_Methods;
    }

    QJsonObject Realization::toJson() const
    {
        auto result = Generalization::toJson();

        QJsonArray methods;
        for (auto method : m_Methods) methods.append(method->toJson());
        result.insert("Methods", methods);

        return result;
    }

    void Realization::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Generalization::fromJson(src, errorList);

        m_Methods.clear();
        utility::checkAndSet(src, "Methods", errorList, [&src, &errorList, this](){
            if (src["Methods"].isArray()) {
                entity::SharedMethod method;
                for (auto value : src["Methods"].toArray()) {
                    method = std::make_shared<entity::ClassMethod>();
                    method->fromJson(value.toObject(), errorList);
                    m_Methods.append(method);
                }
            } else {
                errorList << "Error: \"Methods\" is not array";
            }
        });
    }

    void Realization::make()
    {
        entity::SharedMethod m;
        entity::SharedClass head = std::dynamic_pointer_cast<entity::Class>(m_HeadClass);
        Q_ASSERT_X(head, "Realization::make", "head class not found or not Class");
        for (auto method : m_Methods) {
            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::PureVirtual);
            head->addMethod(m);

            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::Override);
            m_TailClass->addMethod(m);
        }
    }

    void Realization::clear()
    {
        entity::SharedClass head = std::dynamic_pointer_cast<entity::Class>(m_HeadClass);
        Q_ASSERT_X(head, "Realization::clear", "head class not found or not Class");
        for (auto method : m_Methods) {
            head->removeMethods(method->name());
            m_TailClass->removeMethods(method->name());
        }
    }

} // namespace relationship
