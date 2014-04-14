#include "multiplyassociation.h"
#include "enums.h"
#include "class.h"
#include "classmethod.h"
#include "field.h"
#include "constants.cpp"

namespace relationship {

    MultiplyAssociation::MultiplyAssociation()
        : MultiplyAssociation(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    MultiplyAssociation::MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId, const db::SharedDatabase &globalDatabase, const db::SharedDatabase &projectDatabase)
        : Association(tailTypeId, headTypeId, globalDatabase, projectDatabase)
    {
        m_RelationType = MultiRelation;
    }

    void MultiplyAssociation::make()
    {
        Association::make();
        makeDeleter();
        makeGroupGetter();
    }

    void MultiplyAssociation::clear()
    {
        Association::clear();
        removeDeleter();
        removeGroupGetter();
    }

    QString MultiplyAssociation::containerTypeId() const
    {
        return m_ContainerTypeId;
    }

    void MultiplyAssociation::setContainerTypeId(const QString &containerTypeId)
    {
        m_ContainerTypeId = containerTypeId;
    }

    void MultiplyAssociation::makeGetter()
    {
        QString getterName(QString("get%1").arg(m_HeadClass->name()));

        auto getter = m_TailClass->makeMethod(getterName);
        getter->setReturnTypeId(m_GetSetTypeId);
        getter->setConstStatus(true);

        auto parameter = getter->addParameter("index", m_IndexTypeId);
        parameter->setPrefix("");
    }

    void MultiplyAssociation::makeSetter()
    {
        QString setterName(QString("add%1").arg(m_HeadClass->name()));

        auto setter = m_TailClass->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(m_HeadClass->name().toLower()), m_GetSetTypeId);
        param->setPrefix("");
        // TODO: if (m_GetterReturnTypeId == head->id()... make and add new type to DB
        // TODO: if (container type == hash then... add two parameters)
    }

    void MultiplyAssociation::makeField()
    {
       m_TailClass->addField(m_HeadClass->name(), containerTypeId());
    }

    void MultiplyAssociation::makeDeleter()
    {
        QString deleterName(QString("remove%1").arg(m_HeadClass->name()));

        auto deleter = m_TailClass->makeMethod(deleterName);
        auto parameter = deleter->addParameter("index", m_IndexTypeId);
        parameter->setPrefix("");
    }

    void MultiplyAssociation::makeGroupGetter()
    {
        QString groupGetterName(QString("%1s").arg(m_HeadClass->name().toLower()));

        auto groupGetter = m_TailClass->makeMethod(groupGetterName);
        groupGetter->setReturnTypeId(m_ContainerTypeId);
        groupGetter->setConstStatus(true);
    }

    void MultiplyAssociation::removeGetter()
    {
        m_TailClass->removeMethods(QString("get%1").arg(m_HeadClass->name()));
    }

    void MultiplyAssociation::removeSetter()
    {
        m_TailClass->removeMethods(QString("add%1").arg(m_HeadClass->name()));
    }

    void MultiplyAssociation::removeField()
    {
        m_TailClass->removeField("m_" + m_HeadClass->name());
    }

    void MultiplyAssociation::removeDeleter()
    {
        m_TailClass->removeMethods(QString("remove%1").arg(m_HeadClass->name()));
    }

    void MultiplyAssociation::removeGroupGetter()
    {
        m_TailClass->removeMethods(QString("%1s").arg(m_HeadClass->name().toLower()));
    }

    QString MultiplyAssociation::indexTypeId() const
    {
        return m_IndexTypeId;
    }

    void MultiplyAssociation::setIndexTypeId(const QString &indexTypeId)
    {
        m_IndexTypeId = indexTypeId;
    }

} // namespace relationship
