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

import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: appWindow
    visible: true
    width: 800
    height: 600
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
            MenuItem {
                id: openProjectItem
                action: openProjectAction
            }
            MenuSeparator {}
            MenuItem {
                id: exitItem
                action: exitAction
            }

            Action {
                id: newProjectAction
                text: qsTr("Create &new project")
                tooltip: qsTr("Press to create new project")
                shortcut: "Ctrl+N"
                onTriggered: newProjectDialog.show()
            }
            Action {
                id: openProjectAction
                text: qsTr("&Open project")
                tooltip: qsTr("Press to open existing project")
                shortcut: "Ctrl+O"
                onTriggered: openNewProjectDialog.open()
            }
            Action {
                id: exitAction
                text: qsTr("&Exit")
                tooltip: qsTr("Press to close application")
                shortcut: "Ctrl+Q"
                onTriggered: Qt.quit()
            }

            FileDialog {
                id: openNewProjectDialog
                title: qsTr("Open new project")
                nameFilters: "Q-UML project files (*.qut)"
                onAccepted: {
                    if (Qt.resolvedUrl(fileUrl))
                        application.openProject(fileUrl.toString().replace("file://", ""))
                    else
                      handleErrors(qsTr("Path: %1").arg(fileUrl), qsTr("is not resolved."))
                }
            }
        }
    }

    // simple stub for test class creation mechanism
    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: btnCreateClass
                action: actionCreateClass
            }
            Action {
               id: actionCreateClass
               text: qsTr("Add class")
               tooltip: qsTr("Press to add class")
               shortcut: "Ctrl+Shift+C"
//               checkable: true TODO should be checkable
               onTriggered: createNewEntity()
            }
        }
    }

    MainScene {
        id: mainScene
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
    }

    MessageDialog {
        id: warningMessage
        modality: Qt.ApplicationModal
        icon: StandardIcon.Warning
        title: qsTr("Found a few problems")
        standardButtons: StandardButton.Ok

        function addErrors(msg, errorList) {
            text = msg
            informativeText = errorList.join("\n")
        }
    }

    NewProjectDialog {
        id: newProjectDialog
    }

    Connections {
        target: application;
        onErrors: handleErrors(message, errorlist)
        onProjectCreated: handleOpenProject(project)
        onProjectOpened: handleOpenProject(project)
    }

    function handleErrors(msg, errors) {
        warningMessage.addErrors(msg, errors)
        warningMessage.open()
    }

    function handleOpenProject(project) {
        appWindow.title = makeTitle(project.Name)
        stub.text = currentProject.name
    }

    function handleSuccess(msg, details) {
        // TODO: add some
        stub.text = msg + " " + details
    }

    function makeTitle(projectName) {
        return qsTr("%1 - Q-UML").arg(projectName)
    }

    // TODO: add enumeration parameter
    // TODO: add current scope ID parameter for insertation
    function createNewEntity() {
        var component = Qt.createComponent("EntityItem.qml")
        if (component.status === Component.Ready) {
            var entity = component.createObject(mainScene, {"x": 100, "y": 100})
            if (entity !== null) {
                print("Created.")
                // TODO: call method of database for create object
            } else {
                print("Creation error.")
            }
        } else if (component.status === Component.Error) {
            // TODO: add messge which depends on entity type
            handleErrors(qsTr("Component creation error"), qsTr("Creation QML component faild."))
        }
    }
}
