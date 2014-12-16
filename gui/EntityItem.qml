/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 27/11/2014.
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

import QtQuick 2.4

Rectangle {
    id: parent
    radius: 10
    width: 100
    height: 50
    color: "#78e0fe"
    antialiasing: true
    border {
        width: 1
        color: "black"
    }

    property alias entityName: name.text
    property string entityID

    Text {
        id: name
        horizontalAlignment: Text.AlignHCenter
        anchors {
            left: parent.left
            right: buttonArea.left
            top: parent.top
            topMargin: 5
            leftMargin: 5
        }
    }

    Rectangle {
        id: buttonArea
        width: 10
        height: 10
        color: "#78e0fe"
        anchors {
            right:parent.right
            top: parent.top
            rightMargin: 5
            topMargin: 5
        }

        Text {
            text: "^"
        }
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag {
            axis: Drag.XandYAxis
            target: parent
            minimumX: 0
            minimumY: 0
            maximumX: mainScene.width - parent.width
            maximumY: mainScene.height - parent.height
        }
    }

    Connections {
        target: parent
        onXChanged: appWindow.projectModified()
        onYChanged: appWindow.projectModified()
    }

    Component.onCompleted: {
        entityName = "New entity"
        entityID = "stub_id"
    }

    function dataFromJson(obj) {
        entityName = obj.Name
        entityID = obj.ID
    }

    function visualStatesFromJson(obj) {
//        x = obj.X
//        y = obj.Y
    }

    function dataToJson() {
        return {
            "ID": entityID,
            "Name": entityName
        }
    }

    function visualStatesToJson() {
        return {
            "X": x,
            "Y": y
        }
    }
}
