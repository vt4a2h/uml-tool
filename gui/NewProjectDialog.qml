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
    height: 93
    minimumWidth: 300
    minimumHeight: 93
    flags: Qt.Dialog
    modality: Qt.ApplicationModal

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
        placeholderText: qsTr("Enter project name...")
    }

    FileDialog {
        id: pathDialog
        title: qsTr("Choose project folder")
        selectMultiple: false
        selectFolder: true
        onAccepted: {
            if (Qt.resolvedUrl(fileUrl))
                projectPathEdit.text = fileUrl.toString().replace("file://", "")
        }
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
        anchors.right: btnPathDialog.left
        anchors.rightMargin: 8
        anchors.left: projectPathLabel.right
        anchors.leftMargin: 14
        anchors.verticalCenter: projectPathLabel.verticalCenter
        placeholderText: qsTr("Choose project path...")
    }
    Button {
        id: btnPathDialog
        text: qsTr("...")
        height: 20
        width: 20
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.verticalCenter: projectPathEdit.verticalCenter
        onClicked: pathDialog.open()
    }

    Button {
        id: acceptButton
        height: 20
        width: 100
        text: qsTr("Create")
        tooltip: qsTr("Press to create a new project")
        anchors.top: projectPathEdit.bottom
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        onClicked: {
            if (createProject(projectNameEdit.text, projectPathEdit.text)) {
                clear()
                close()
            }
        }
    }

    function createProject(name, path) {
        if (name && name.length !== 0 && path && path.lenght !== 0) {
            application.createProject(name, path)
            return true;
        }

        return false;
    }

    function clear() {
        projectNameEdit.text = ""
        projectPathEdit.text = ""
    }
}
