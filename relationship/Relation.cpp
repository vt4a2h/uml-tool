/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 29/10/2014.
**
** This file is part of Q-UML (UML tool for Qt).
**
** Q-UML is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Q-UML is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.

** You should have received a copy of the GNU Lesser General Public License
** along with Q-UML.  If not, see <http://www.gnu.org/licenses/>.
**
*****************************************************************************/

#include "Relation.h"
#include "node.h"
#include "enums.h"
#include "Constants.h"

#include <QJsonObject>
#include <QDebug>

#include <db/Database.h>

#include <entity/Class.h>
#include <entity/TemplateClass.h>

#include <utility/helpfunctions.h>

#include <helpers/GeneratorID.h>

#include "QtHelpers.h"

namespace relationship {

    namespace {

        const QString relMark = "RelType";
        const QString headMark = "Head node";
        const QString tailMark = "Tail node";

        void readNode(const SharedNode &node, const QString& mark, const QJsonObject &src,
                      QStringList &errorList, Relation &r)
        {
            Util::checkAndSet(src, mark, errorList, [&](){
                // Read node itself
                auto jsonObject = src[mark].toObject();
                node->fromJson(jsonObject, errorList);

                // Set type for node
                common::ID id;
                id.fromJson(jsonObject[Node::typeIDMark()], errorList);
                r.setType(node, id);
            });
        }

        inline entity::SharedClass castToClass(const entity::SharedType &type)
        {
            if (type->hashType() == entity::Class::staticHashType() ||
                type->hashType() == entity::TemplateClass::staticHashType())
                return std::static_pointer_cast<entity::Class>(type);

            return entity::SharedClass();
        }

    }

    /**
     * @brief Relation::Relation
     */
    Relation::Relation()
        : Relation(common::ID::nullID(), common::ID::nullID(), db::WeakTypeSearchers())
    {
    }

    /**
     * @brief Relation::Relation
     * @param src
     */
    Relation::Relation(const Relation &src)
        : common::BasicElement(src)
    {
        copyFrom(src);
    }

    /**
     * @brief Relation::Relation
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Relation::Relation(const common::ID &tailTypeId, const common::ID &headTypeId,
                       const db::WeakTypeSearchers &typeSearchers)
        : common::BasicElement("" /*name*/, helpers::GeneratorID::instance().genID())
        , m_TailNode(std::make_shared<Node>())
        , m_HeadNode(std::make_shared<Node>())
        , m_RelationType(SimpleRelation)
        , m_TypeSearchers(typeSearchers)
    {
        if (headTypeId != common::ID::nullID() && !typeSearchers.isEmpty())
            setType(m_HeadNode, headTypeId);
        if (tailTypeId != common::ID::nullID() && !typeSearchers.isEmpty() )
            setType(m_TailNode, tailTypeId);
    }

    /**
     * @brief Relation::operator =
     * @param rhs
     * @return
     */
    Relation &Relation::operator =(const Relation &rhs)
    {
        if (this != &rhs) {
            common::BasicElement::operator =(rhs);
            copyFrom(rhs);
        }

        return *this;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Relation &lhs, const Relation &rhs)
    {
        return static_cast<const common::BasicElement&>(lhs)  ==
               static_cast<const common::BasicElement&>(rhs)  &&
               lhs.m_TypeSearchers == rhs.m_TypeSearchers     &&
               (lhs.m_HeadNode  == rhs.m_HeadNode  || *lhs.m_HeadNode  == *rhs.m_HeadNode ) &&
               (lhs.m_TailNode  == rhs.m_TailNode  || *lhs.m_TailNode  == *rhs.m_TailNode );
    }

    /**
     * @brief Relation::makeRelation
     */
    void Relation::makeRelation()
    {
        check();
        make();
    }

    /**
     * @brief Relation::removeRelation
     */
    void Relation::removeRelation()
    {
        check();
        clear();
    }

    /**
     * @brief Relation::make
     */
    void Relation::make()
    {
    }

    /**
     * @brief Relation::clear
     */
    void Relation::clear()
    {
    }

    /**
     * @brief Relation::headClass
     * @return
     */
    entity::SharedClass Relation::headClass() const
    {
        return castToClass(m_HeadNode->type());
    }

    /**
     * @brief Relation::tailClass
     * @return
     */
    entity::SharedClass Relation::tailClass() const
    {
        return castToClass(m_TailNode->type());
    }

    /**
     * @brief Relation::copyFrom
     * @param src
     */
    void Relation::copyFrom(const Relation &src)
    {
        m_TailNode = std::make_shared<Node>(*src.m_TailNode);
        m_HeadNode = std::make_shared<Node>(*src.m_HeadNode);

        m_RelationType = src.m_RelationType;

        m_TypeSearchers = src.m_TypeSearchers;
    }

    /**
     * @brief Relation::check
     */
    void Relation::check()
    {
        Q_ASSERT_X(m_HeadNode->type(), Q_FUNC_INFO, "head class not found");
        Q_ASSERT_X(m_TailNode->type(), Q_FUNC_INFO, "tail class not found");

#ifdef QT_DEBUG
        for (auto &&ts : m_TypeSearchers)
            Q_ASSERT_X(!!ts.lock(), Q_FUNC_INFO, "type searcher is not valid");
#endif
    }

    /**
     * @brief Relation::addType
     * @param node
     * @param typeId
     */
    void Relation::setType(const SharedNode &node, const common::ID &typeId)
    {
        node->setType(G_ASSERT(findType(typeId)));
    }

    /**
     * @brief Relation::relationType
     * @return
     */
    RelationType Relation::relationType() const
    {
        return m_RelationType;
    }

    /**
     * @brief Relation::setRelationType
     * @param relationType
     */
    void Relation::setRelationType(const RelationType &relationType)
    {
        m_RelationType = relationType;
    }

    /**
     * @brief Relation::headClass
     * @return
     */
    entity::SharedType Relation::headType() const
    {
        return m_HeadNode->type();
    }

    /**
     * @brief Relation::setHeadType
     * @param c
     */
    void Relation::setHeadType(const entity::SharedType &type)
    {
        m_HeadNode->setType(type);
    }

    /**
     * @brief Relation::tailType
     * @return
     */
    entity::SharedType Relation::tailType() const
    {
        return m_TailNode->type();
    }

    /**
     * @brief Relation::setTailType
     * @param c
     */
    void Relation::setTailType(const entity::SharedType &type)
    {
        m_TailNode->setType(type);
    }

    /**
     * @brief Relation::toJson
     * @return
     */
    QJsonObject Relation::toJson() const
    {
        QJsonObject result = common::BasicElement::toJson();

        result.insert(relMark, static_cast<int>(m_RelationType));
        result.insert(headMark, m_HeadNode->toJson());
        result.insert(tailMark, m_TailNode->toJson());

        return result;
    }

    /**
     * @brief Relation::fromJson
     * @param src
     * @param errorList
     */
    void Relation::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        common::BasicElement::fromJson(src, errorList);
        Util::checkAndSet(src, relMark, errorList, [&src, this](){
            m_RelationType = static_cast<RelationType>(src[relMark].toInt());
        });
        readNode(m_HeadNode, headMark, src, errorList, *this);
        readNode(m_TailNode, tailMark, src, errorList, *this);
    }

    /**
     * @brief Relation::isEqual
     * @param rhs
     * @return
     */
    bool Relation::isEqual(const Relation &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Relation::tryToFindType
     * @param typeId
     * @return
     */
    entity::SharedType Relation::findType(const common::ID &typeId) const
    {
        for (auto &&weakTypeSearcher : m_TypeSearchers)
            if (auto ts = weakTypeSearcher.lock())
                if (auto result = ts->typeByID(typeId))
                    return result;

        qWarning() << "Type is not found." << Q_FUNC_INFO;
        return entity::SharedType();
    }

    /**
     * @brief Relation::typeSearchers
     * @return
     */
    db::WeakTypeSearchers Relation::typeSearchers() const
    {
        return m_TypeSearchers;
    }

    /**
     * @brief Relation::setTypeSearchers
     * @param typeSearchers
     */
    void Relation::setTypeSearchers(const db::WeakTypeSearchers &typeSearchers)
    {
        m_TypeSearchers = typeSearchers;
    }

    QString Relation::typeMarker()
    {
        return relMark;
    }

} // namespace relationship
