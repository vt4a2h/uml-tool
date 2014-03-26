#include "class.h"
#include "enums.h"
#include "field.h"
#include "classmethod.h"
#include "enums.h"

#include <utility>
#include <algorithm>

namespace entity {

    Class::Class()
        : Class("noname", nullptr)
    {
    }

    Class::Class(const QString &name, Scope *scope)
        : Type(name, scope)
    {
    }

    ClassParent Class::addParent(SharedClass cl, Section section)
    {
        return *m_Parents.insert(m_Parents.end(), std::make_pair(cl, section));
    }

    ClassParent Class::getParent(const QString &name, Scope *scope)
    {
        ClassParent result(nullptr, Public);

        auto it = std::find_if(m_Parents.begin(), m_Parents.end(),
                               [&](const ClassParent &p) {return p.first->name() == name && (scope ? p.first->scope() == scope : true);});
        if (it != m_Parents.end()) result = *it;

        return result;
    }

    bool Class::containsParent(const QString &name, Scope *scope)
    {
        return getParent(name, scope).first != nullptr;
    }

    void Class::removeParent(const ClassParent &parent)
    {
        if (parent.first) m_Parents.removeOne(parent);
    }

    void Class::removeParent(const QString &name, Scope *scope)
    {
        removeParent(getParent(name, scope));
    }

    ClassParents Class::parents() const
    {
        return m_Parents;
    }

    Kind Class::kind() const
    {
        return m_Kind;
    }

    void Class::setKind(Kind kind)
    {
        m_Kind = kind;
    }

    bool Class::isFinal() const
    {
        return m_FinalStatus;
    }

    void Class::setFinalStatus(bool status)
    {
        m_FinalStatus = status;
    }


} // namespace entity
