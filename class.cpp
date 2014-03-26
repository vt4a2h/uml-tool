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

    Parent Class::addParent(SharedClass cl, Section section)
    {
        return *m_Parents.insertMulti(cl->name(), std::make_pair(cl, section));
    }

    ParentsList Class::getParents(const QString &name)
    {
        return m_Parents.values(name);
    }

    bool Class::containsParent(const QString &name)
    {
        return m_Parents.contains(name);
    }

    void Class::removeParents(const QString &name)
    {
        m_Parents.remove(name);
    }

    void Class::removeParent(const Parent &parent)
    {
        m_Parents.remove(parent.first->name(), parent);
    }

    ParentsList Class::parents() const
    {
        return m_Parents.values();
    }

    SharedMethod Class::addMethod(const QString &name)
    {
        return *m_Methods.insertMulti(name, std::make_shared<ClassMethod>(name));
    }

    MethodsList Class::getMethod(const QString &name)
    {
        return m_Methods.values(name);
    }

    bool Class::containsMethod(const QString &name)
    {
        return m_Methods.contains(name);
    }

    void Class::removeMethods(const QString &name)
    {
        m_Methods.remove(name);
    }

    void Class::removeMethod(SharedMethod method)
    {
        m_Methods.remove(method->name(), method);
    }

    MethodsList Class::methods() const
    {
        return m_Methods.values();
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
