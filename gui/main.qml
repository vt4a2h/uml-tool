/*****************************************************************************
**
** Copyright (C) 2014 Fanaskov Vitaly (vt4a2h@gmail.com)
**
** Created 03/11/2014.
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

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

ApplicationWindow {
    id: appWindow
    visible: true
    width: 1024
    height: 768
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    title: qsTr("Q-UML")

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                id: newProjectItem
                action: newProjectAction
            }
            MenuSeparator {}
            MenuItem {
                id: exitItem
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
    }

    Text {
        id: stub
        text: qsTr("Work area")
        anchors.centerIn: parent
    }

    WarningMessage {
        id: warningMessage
    }

    NewProjectDialog {
        id: newProjectDialog
    }

    Action {
        id: newProjectAction
        text: qsTr("Create &new project")
        shortcut: "Ctrl+N"
        onTriggered:  newProjectDialog.show()
    }

    Connections {
        target: application;
        onErrors: handleErrors(message, errorlist)
        onProjectCreated: handleNewProject(newProject)
    }

    function handleErrors(msg, errors) {
        warningMessage.addErrors(msg, errors)
        warningMessage.show()
    }

    function handleNewProject(project) {
        stub.text = project.get().name()
    }

    function handleSuccess(msg, details) {
        // TODO: add some
        stub.text = msg + " " + details
    }
}
