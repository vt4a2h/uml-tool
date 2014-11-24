import QtQuick 2.0

Rectangle {
    id: parentID
    width: 100
    height: 50

    Text {
        id: entityName
        text: qsTr("Stub")
        horizontalAlignment: Text.AlignHCenter
        anchors {
            left: parent.left
            right: buttonArea.left
            top: parent.top
        }
    }

    Rectangle {
        id: buttonArea
        width: 10
        height: 10
        anchors {
            right:parent.right
            top: parent.top
        }

        Text {
            text: "^"
        }
    }
}
