/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 23/11/2014.
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

import QtQuick 2.0

Column {
   id: itemsList
   spacing: 8
   anchors {
       margins: 8
       left: parent.left
       top: parent.top
       bottom: parent.bottom
   }

   // stub's
   Rectangle {
       height: 50
       width: 50
       color: "red"

       property bool dragActive: dragArea.drag.active

       Drag.dragType: Drag.Automatic

       onDragActiveChanged: {
           if (dragActive) {
               print("drag started")
               Drag.start();
           } else {
               print("drag finished")
               Drag.drop();
           }
       }

       MouseArea {
           id: dragArea
           anchors.fill: parent
           drag.target: parent
       }

   }

   Rectangle {
       height: 50
       width: 50
       color: "green"
   }
}
