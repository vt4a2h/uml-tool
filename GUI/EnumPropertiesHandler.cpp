/*****************************************************************************
**
** Copyright (C) 2017 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 24.
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
#include "EnumPropertiesHandler.hpp"

#include <Entity/Enum.h>

#include "Section.h"

static const QString enumDeclarationHelp =
    QT_TRANSLATE_NOOP("EntityProperties",
                      "Enter enum declaration in the following format:<br>"
                      "enum-key<sub>opt</sub> identifier type<sub>opt</sub><br>"
                      "For example: class Foo int");
static const QString enumEnumeratorsHelp =
    QT_TRANSLATE_NOOP("EntityProperties",
                      "Enter enumerators in the following format:<br>"
                      "enumerator constexpr<br>"
                      "enumerator constexpr<br>"
                      "...<br>"
                      "enumerator constexpr<br><br>"
                      "For example:<br>"
                      "a 1<br>"
                      "b 2<br>");

namespace GUI {

    /**
     * @brief EnumPropHandler::setEntity
     * @param type
     */
    void EnumPropHandler::setEntity(const Entity::SharedType &type)
    {
        if (type && type->kindOfType() == Entity::KindOfType::Enum) {
            m_Enum = std::static_pointer_cast<Entity::Enum>(type);
            if (sections().isEmpty())
                m_EnumDeclaration = addSection(tr("Declaration"), enumDeclarationHelp);
        } else
            m_Enum.reset();
    }


    Entity::SharedType EnumPropHandler::entity() const
    {
        return m_Enum;
    }

} // namespace GUI
