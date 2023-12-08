import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Rectangle {
    id: closeButton
    color: "transparent"

    property bool visibleLeftUpRadius: false
    property bool visibleRightUpRadius: false
    property bool visibleLeftDownRadius: false
    property bool visibleRightDownRadius: false

    Rectangle {
        visible: !closeButton.visibleLeftUpRadius
        anchors.top: parent.top
        anchors.left: parent.left
        color: parent.color
        height: parent.radius
        width: parent.radius
    }

    Rectangle {
        visible: !closeButton.visibleLeftDownRadius
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        color: parent.color
        height: parent.radius
        width: parent.radius
    }

    Rectangle {
        visible: !closeButton.visibleRightDownRadius
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        color: parent.color
        height: parent.radius
        width: parent.radius
    }

    Rectangle {
        visible: !closeButton.visibleRightUpRadius
        anchors.top: parent.top
        anchors.right: parent.right
        color: parent.color
        height: parent.radius
        width: parent.radius
    }

    MouseArea {
        id: closeButtonMouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: Qt.quit()
        onContainsMouseChanged: {
            if (closeButtonMouseArea.containsMouse) {
                closeButton.color= "#c98c8c"
            } else {
                closeButton.color= "transparent"

            }
        }
    }

    Image {
        id: close
        anchors.centerIn: parent
        width:14
        height:14
        source: "qrc:/images/close.svg"
        fillMode: Image.PreserveAspectFit
        ColorOverlay {
            anchors.fill: close
            source: close
            color: "#0C011C"
        }
    }
}