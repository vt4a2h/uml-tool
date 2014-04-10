#include "association.h"
#include "class.h"
#include "classmethod.h"
#include "enums.h"
#include "types.h"
#include "field.h"
#include "constants.cpp"

namespace relationship {

    Association::Association()
        : Association(STUB_ID, STUB_ID)
    {
    }

    Association::Association(const QString &tailTypeId, const QString &headTypeId)
        : Relation(tailTypeId, headTypeId)
        , m_GetSetTypeId(headTypeId)
    {
        m_RelationType = AssociationRelation;
    }

    void Association::make()
    {
        makeField();
        makeGetter();
        makeSetter();
    }

    void Association::clear()
    {
        removeField();
        removeGetter();
        removeSetter();
    }

    void Association::makeGetter()
    {
        QString getterName(m_HeadClass->name().begin()->toLower());

        auto getter = m_TailClass->makeMethod(getterName);
        getter->setReturnTypeId(m_GetSetTypeId);
        getter->setConstStatus(true);
    }

    void Association::makeSetter()
    {
        QString setterName(QString("set%1").arg(m_HeadClass->name()));

        auto setter = m_TailClass->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(m_HeadClass->name().toLower()), m_GetSetTypeId);
        param->setPrefix("");
        // TODO: if (m_GetterReturnTypeId == head->id()... make and add new type to DB
    }

    void Association::removeGetter()
    {
        m_TailClass->removeMethods(m_HeadClass->name().begin()->toLower());
    }

    void Association::removeSetter()
    {
        m_TailClass->removeMethods(QString("src_%1").arg(m_HeadClass->name().toLower()));
    }

    QString Association::fieldtypeId() const
    {
        return m_FieldtypeId;
    }

    void Association::setFieldtypeId(const QString &fieldtypeId)
    {
        m_FieldtypeId = fieldtypeId;
    }

    void Association::makeField()
    {
        m_TailClass->addField(m_HeadClass->name(), m_FieldtypeId);
    }

    void Association::removeField()
    {
        m_TailClass->removeField("m_" + m_HeadClass->name());
    }

    QString Association::getGetSetTypeId() const
    {
        return m_GetSetTypeId;
    }

    void Association::setGetSetTypeId(const QString &getterReturnTypeId)
    {
        m_GetSetTypeId = getterReturnTypeId;
    }


} // namespace relationship
