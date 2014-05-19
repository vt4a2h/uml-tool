#include "multiplyassociation.h"
#include "database.h"
#include "enums.h"
#include "class.h"
#include "classmethod.h"
#include "field.h"
#include "helpfunctions.h"
#include "constants.cpp"

#include <QJsonObject>

namespace relationship {

    MultiplyAssociation::MultiplyAssociation()
        : MultiplyAssociation(STUB_ID, STUB_ID, nullptr, nullptr)
    {
    }

    MultiplyAssociation::MultiplyAssociation(const QString &tailTypeId, const QString &headTypeId, db::Database *globalDatabase, db::Database *projectDatabase)
        : Association(tailTypeId, headTypeId, globalDatabase, projectDatabase)
        , m_ContainerClass(nullptr)
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
        m_ContainerClass = m_GlobalDatabase->depthTypeSearch(containerTypeId);
        if (!m_ContainerClass) m_ProjectDatabase->depthTypeSearch(containerTypeId);
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::setContainerTypeId",
                   "container class not found");

        m_ContainerTypeId = containerTypeId;
    }

    void MultiplyAssociation::makeGetter()
    {
        QString getterName(QString("get%1").arg(m_HeadClass->name()));

        auto getter = m_TailClass->makeMethod(getterName);
        getter->setReturnTypeId(m_GetSetTypeId);
        getter->setConstStatus(true);

        auto parameter = getter->addParameter("key", m_KeyTypeId);
        parameter->setPrefix("");
    }

    void MultiplyAssociation::makeSetter()
    {
        QString setterName(QString("add%1").arg(m_HeadClass->name()));

        auto setter = m_TailClass->makeMethod(setterName);
        auto param = setter->addParameter(QString("src_%1").arg(m_HeadClass->name().toLower()), m_GetSetTypeId);
        param->setPrefix("");
    }

    void MultiplyAssociation::makeField()
    {
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::makeField",
                   "container class not found");
        m_TailClass->addField(m_ContainerClass->name(), containerTypeId());
    }

    void MultiplyAssociation::makeDeleter()
    {
        QString deleterName(QString("remove%1").arg(m_HeadClass->name()));

        auto deleter = m_TailClass->makeMethod(deleterName);
        auto parameter = deleter->addParameter("key", m_KeyTypeId);
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
        Q_ASSERT_X(m_ContainerClass,
                   "MultiplyAssociation::removeField",
                   "container class not found");
        m_TailClass->removeField(m_ContainerClass->name());
    }

    void MultiplyAssociation::removeDeleter()
    {
        m_TailClass->removeMethods(QString("remove%1").arg(m_HeadClass->name()));
    }

    void MultiplyAssociation::removeGroupGetter()
    {
        m_TailClass->removeMethods(QString("%1s").arg(m_HeadClass->name().toLower()));
    }

    QString MultiplyAssociation::keyTypeId() const
    {
        return m_KeyTypeId;
    }

    void MultiplyAssociation::setKeyTypeId(const QString &indexTypeId)
    {
        m_KeyTypeId = indexTypeId;
    }

    QJsonObject MultiplyAssociation::toJson() const
    {
        QJsonObject result = Association::toJson();

        result.insert("Conatiner ID", m_ContainerTypeId);
        result.insert("Key ID", m_KeyTypeId);

        return result;
    }

    void MultiplyAssociation::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        utility::checkAndSet(src, "Conatiner ID", errorList, [&src, this](){
            m_ContainerTypeId = src["Conatiner ID"].toString();
            m_ContainerClass = tryToFindType(m_ContainerTypeId);
            Q_ASSERT_X(m_ContainerClass,
                       "MultiplyAssociation::fromJson",
                       "container class is not found");
        });

        utility::checkAndSet(src, "Key ID", errorList, [&src, this](){
            m_KeyTypeId = src["Key ID"].toString();
        });
    }

} // namespace relationship
