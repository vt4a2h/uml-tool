#include "class.h"
#include "enums.h"
#include "field.h"
#include "classmethod.h"
#include "enums.h"
#include "extendedtype.h"
#include "constants.cpp"

#include <utility>
#include <algorithm>

namespace entity {

    Class::Class()
        : Class(DEFAULT_NAME, GLOBAL_SCOPE_ID)
    {
    }

    Class::Class(const QString &name, const QString &scopeId)
        : Type(name, scopeId)
    {
    }

    Parent Class::addParent(const QString &typeId, Section section)
    {
        return *m_Parents.insert(typeId, std::make_pair(typeId, section));
    }

    ParentsList Class::getParents(const QString &typeId)
    {
        return m_Parents.values(typeId);
    }

    bool Class::containsParent(const QString &typeId)
    {
        return m_Parents.contains(typeId);
    }

    void Class::removeParents(const QString &typeId)
    {
        m_Parents.remove(typeId);
    }

    void Class::removeParent(const QString &typeId)
    {
        m_Parents.remove(typeId);
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

    SharedField Class::addField(const QString &name, const QString &typeId)
    {
        return *m_Fields.insert(name, std::make_shared<Field>(name, typeId));
    }

    SharedField Class::getField(const QString &name)
    {
        return m_Fields.value(name);
    }

    bool Class::containsField(const QString &name)
    {
        return m_Fields.contains(name);
    }

    void Class::removeField(const QString &name)
    {
        m_Fields.remove(name);
    }

    FieldsList Class::fields() const
    {
        return m_Fields.values();
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
