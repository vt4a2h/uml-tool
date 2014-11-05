/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 05/11/2014.
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
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

Window {
    id: newProjectWindow
    title: "New project"
    width: 400
    height: 100
    minimumWidth: 300
    minimumHeight: 100

    Label {
        id: projectNameLabel
        text: qsTr("Project name")
        anchors.leftMargin: 8
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.top: parent.top
    }
    TextField{
        id: projectNameEdit
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: projectNameLabel.right
        anchors.leftMargin: 8
        anchors.verticalCenter: projectNameLabel.verticalCenter
        focus: true
    }

    Label {
        id: projectPathLabel
        text: qsTr("Project path")
        anchors.leftMargin: 8
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.top: projectNameLabel.top
    }
    TextField {
        id: projectPathEdit
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: projectPathLabel.right
        anchors.leftMargin: 14
        anchors.verticalCenter: projectPathLabel.verticalCenter
    }

    Button {
        id: acceptButton
        width: 30
        height: 20
        text: qsTr("Ok")
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.top: projectPathEdit.bottom
        anchors.topMargin: 10
    }
}
