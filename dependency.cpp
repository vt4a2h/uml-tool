#include "dependency.h"
#include "classmethod.h"
#include "class.h"
#include "enums.h"
#include "constants.cpp"

namespace relationship {

    Dependency::Dependency()
        : Dependency(STUB_ID, STUB_ID)
    {
    }

    Dependency::Dependency(const QString &tailTypeId, const QString &headTypeId)
        : Relation(tailTypeId, headTypeId)
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


} // namespace relationship
