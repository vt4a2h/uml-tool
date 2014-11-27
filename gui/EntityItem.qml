import QtQuick 2.0

Rectangle {
    id: parentID
    radius: 10
    width: 100
    height: 50
    color: "#78e0fe"
    antialiasing: true
    border {
        width: 1
        color: "black"
    }

    property bool dragActive: dragArea.drag.active

    Text {
        id: entityName
        text: qsTr("Stub")
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
        drag {
            axis: Drag.XandYAxis
            target: parent
            minimumX: 0
            minimumY: 0
            maximumX: mainScene.width - parent.width
            maximumY: mainScene.height - parent.height
        }
    }

}
