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
import QUMLCore 1.0

ApplicationWindow {
    id: appWindow
    visible: true
    width: 800
    height: 600
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    title: qsTr("Q-UML")

    property int projectStatus

    signal projectModified

    menuBar: MenuBar {
        Menu {
            id: menuFile
            title: qsTr("&File")
            MenuItem {
                id: newProjectItem
                action: newProjectAction
            }
            MenuItem {
                id: openProjectItem
                action: openProjectAction
            }
            MenuItem {
                id: saveProjectItem
                action: saveProjectAction
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
                shortcut: "Ctrl+S"
                onTriggered: openNewProjectDialog.open()
            }
            Action {
                id: saveProjectAction
                text: qsTr("&Save project")
                tooltip: qsTr("Press to save current project")
                shortcut: "Ctrl+O"
                onTriggered: saveProject()
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
        id: tbEntityActions
        RowLayout {
            id: rowLayout
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
            informativeText = errorList
        }
    }

    NewProjectDialog {
        id: newProjectDialog
    }

    Connections {
        target: application
        onErrors: handleErrors(message, errorlist)
        onProjectCreated: handleOpenProject(project)
        onProjectOpened: handleOpenProject(project)
        onCurrentProjectSaved: makeTitle(currentProject.name, false)
        onCurrentProjectModified: makeTitle(currentProject.name, true)
    }

    Connections {
        target: appWindow
        onProjectStatusChanged: changeProjectStatus(projectStatus)
    }

    Component.onCompleted: {
        projectStatus = QUMLApplication.NoProject
    }

    function changeProjectStatus(newStatus) {
        // NOTE: add dictionary with callbacks for more statuses
        switch (newStatus) {
            case QUMLApplication.ProjectOpened:
                setOpenProjectStatus();
                break;
            case QUMLApplication.NoProject:
                setNoProjectStatus();
                break;
            default:
                console.log("Strange project status: %1!".arg(newStatus))
        }
    }

    function setOpenProjectStatus() {
        mainScene.activeStatus = true
        tbEntityActions.enabled = true
    }

    function setNoProjectStatus() {
        mainScene.activeStatus = false
        tbEntityActions.enabled = false
    }

    function handleErrors(msg, errors) {
        warningMessage.addErrors(msg, errors)
        warningMessage.open()
    }

    function handleOpenProject(project) {
        makeTitle(project.Name, false)
        projectStatus = QUMLApplication.ProjectOpened
    }

    function saveProject() {
    }

    function handleSuccess(msg, details) {
        stub.text = msg + " " + details
    }

    function makeTitle(projectName, withAsterisks) {
        var title = "%1%2 - Q-UML"
        appWindow.title = title.arg(projectName).arg(withAsterisks ? " *" : "")
    }

    function createNewEntity() {
        var component = Qt.createComponent("EntityItem.qml")
        if (component.status === Component.Ready) {
            var entity = component.createObject(mainScene, {"x": 100, "y": 100})
            if (entity !== null) {
                // TODO: add enumeration parameter for type
                var jsObj = currentProjectDatabase.createEntity(application.currentScopeID);
                entity.dataFromJson(jsObj)
                entity.visualStatesFromJson(jsObj)
                print("Created.")
            } else {
                print("Creation error.")
            }
        } else if (component.status === Component.Error) {
            handleErrors(qsTr("Component creation error"), qsTr("Creation QML component faild."))
        }
    }
}
