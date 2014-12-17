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

// scene stub

Item {
    id: mainScene
    anchors.margins: 8

    property var activeStatus
    signal clicked(int x, int y)

    DropArea {
        id: dragTarget
        anchors.fill: parent
        anchors.centerIn: parent

        Rectangle {
            id: dropScene
            anchors.fill: parent
            antialiasing: true
            radius: 3
            border.width: 1

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: mainScene.clicked(mouseX, mouseY)
            }
        }

    }

    Connections {
        target: mainScene
        onActiveStatusChanged: setActiveStatus(activeStatus)
    }

    Component.onCompleted: {
    }

    function setActiveStatus(newActiveStatus) {
        dropScene.enabled = newActiveStatus
        dropScene.color = newActiveStatus ? "white" : "lightgray"
        dropScene.border.color = newActiveStatus ? "black" : "gray"
    }
}
