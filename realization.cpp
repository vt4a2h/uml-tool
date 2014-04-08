#include "realization.h"
#include "classmethod.h"
#include "class.h"
#include "enums.h"
#include "constants.cpp"

namespace relationship {

    Realization::Realization()
        : Realization(STUB_ID, STUB_ID)
    {
    }

    Realization::Realization(const QString &tailTypeId, const QString &headTypeId)
        : Generalization(tailTypeId, headTypeId)
    {
    }

    void Realization::addMethods(const entity::MethodsList &methods)
    {
        m_Methods = methods;
    }

    entity::MethodsList Realization::methods() const
    {
        return m_Methods;
    }

    void Realization::make()
    {
        entity::SharedMethod m;
        for (auto method : m_Methods) {
            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::PureVirtual);
            m_HeadClass->addMethod(m);

            m = std::make_shared<entity::ClassMethod>(*method.get());
            m->setRhsIdentificator(entity::Override);
            m_TailClass->addMethod(m);
        }
    }

    void Realization::clear()
    {
        for (auto method : m_Methods) {
            m_HeadClass->removeMethods(method->name());
            m_TailClass->removeMethods(method->name());
        }
    }

} // namespace relationship
