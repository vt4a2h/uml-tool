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

Window {
    id: warningMessage
    minimumWidth: 300
    minimumHeight: 100
    flags: Qt.Dialog
    modality: Qt.ApplicationModal

    title: qsTr("Found some problems")

    Button {
        id: btnOk
        height: 20
        width: 100
        text: qsTr("Ok")
        anchors.horizontalCenterOffset: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: close()
        focus: true
    }

    Label {
        id: lblMsg
        verticalAlignment: "AlignVCenter"
        horizontalAlignment: "AlignJustify"
        wrapMode: Text.WordWrap
        anchors.bottom: btnOk.top
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
    }

    function addErrors(errorList) {
        lblMsg.text = errorList.join("\n")
    }
}
