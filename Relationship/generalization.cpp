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

#include "generalization.h"
#include "enums.h"
#include "Constants.h"

#include <QJsonObject>

#include <Entity/Class.h>
#include <Utility/helpfunctions.h>

#include "QtHelpers.h"

namespace Relationship {

    /**
     * @brief Generalization::Generalization
     */
    Generalization::Generalization()
        : Generalization(Common::ID::nullID(), Common::ID::nullID(), DB::WeakTypeSearchers())
    {
    }

    /**
     * @brief Generalization::Generalization
     * @param tailTypeId
     * @param headTypeId
     * @param globalDatabase
     * @param projectDatabase
     */
    Generalization::Generalization(const Common::ID &tailTypeId, const Common::ID &headTypeId,
                                   const DB::WeakTypeSearchers &typeSearchers)
        :  Relation(tailTypeId, headTypeId, typeSearchers)
        ,  m_Section(Entity::Public)
    {
        m_RelationType = GeneralizationRelation;
    }

    /**
     * @brief operator ==
     * @param lhs
     * @param rhs
     * @return
     */
    bool operator ==(const Generalization &lhs, const Generalization &rhs)
    {
        return static_cast<const Relation&>(lhs).isEqual(rhs) &&
               lhs.m_Section == rhs.m_Section;
    }

    /**
     * @brief Generalization::section
     * @return
     */
    Entity::Section Generalization::section() const
    {
        return m_Section;
    }

    /**
     * @brief Generalization::setSection
     * @param section
     */
    void Generalization::setSection(const Entity::Section &section)
    {
        m_Section = section;
    }

    /**
     * @brief Generalization::toJson
     * @return
     */
    QJsonObject Generalization::toJson() const
    {
        auto result = Relation::toJson();

        result.insert("Section", m_Section);

        return result;
    }

    /**
     * @brief Generalization::fromJson
     * @param src
     * @param errorList
     */
    void Generalization::fromJson(const QJsonObject &src, QStringList &errorList)
    {
        Relation::fromJson(src, errorList);

        Util::checkAndSet(src, "Section", errorList, [this, &src](){
            m_Section = static_cast<Entity::Section>(src["Section"].toInt());
        });
    }

    /**
     * @brief Generalization::isEqual
     * @param rhs
     * @return
     */
    bool Generalization::isEqual(const Generalization &rhs) const
    {
        return *this == rhs;
    }

    /**
     * @brief Generalization::make
     */
    void Generalization::make()
    {
        G_ASSERT(tailClass())->addParent(G_ASSERT(headClass())->id(), m_Section);
    }

    /**
     * @brief Generalization::clear
     */
    void Generalization::clear()
    {
        G_ASSERT(tailClass())->removeParent(G_ASSERT(headClass())->id());
    }

} // namespace relationship
