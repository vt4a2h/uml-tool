#include "classmethod.h"
#include "extendedtype.h"
#include "field.h"
#include "enums.h"

namespace entity {

    ClassMethod::ClassMethod()
        : ClassMethod("noname")
    {
    }

    ClassMethod::ClassMethod(const QString &name)
        : m_Name(name)
        , m_Kind(Public)
        , m_Identificator(None)
        , m_ConstStatus(false)
        , m_ReturnType(nullptr)
    {
    }

    QString ClassMethod::name() const
    {
        return m_Name;
    }

    void ClassMethod::setName(const QString &name)
    {
        m_Name = name;
    }

    Section ClassMethod::kind() const
    {
        return m_Kind;
    }

    void ClassMethod::setKind(Section &kind)
    {
        m_Kind = kind;
    }

    ExtendedType *ClassMethod::returnType() const
    {
        return m_ReturnType;
    }

    void ClassMethod::setReturnType(ExtendedType *returnType)
    {
        m_ReturnType = returnType;
    }

    bool ClassMethod::isConst() const
    {
        return m_ConstStatus;
    }

    void ClassMethod::setConstStatus(bool newStatus)
    {
        m_ConstStatus = newStatus;
    }

    Identificator ClassMethod::identificator() const
    {
        return m_Identificator;
    }

    void ClassMethod::setIdentificator(const Identificator &identificator)
    {
        m_Identificator = identificator;
    }

    SharedField ClassMethod::getParameter(const QString &name)
    {
        SharedField result = nullptr;

        auto it = m_Parameters.find(name);
        if ( it != m_Parameters.end()) result = *it;

        return result;
    }

    SharedField ClassMethod::addParameter(const QString &name, ExtendedType *type)
    {
        return *m_Parameters.insert(name, std::make_shared<Field>(name, type));
    }

    bool ClassMethod::containsParameter(const QString &name)
    {
        return m_Parameters.contains(name);
    }

    void ClassMethod::removeParameter(const QString &name)
    {
        m_Parameters.remove(name);
    }

    FieldsList ClassMethod::parameters() const
    {
        return m_Parameters.values();
    }

} // namespace entity
