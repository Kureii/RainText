import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs

Rectangle {
    id: loginDbScreen
    height: 150
    width: 300

    property color textColor: "#0C011C"
    property color progressBarBackground: "#c3b9af"
    property color progressBarForeground: "#FF9800"
    property real value: 0.0
    property string text: ""

    Rectangle {
        anchors.centerIn: parent
        width: loginDbScreen.width / 3 * 2
        height: 30

        Text {
            anchors.top: parent.top
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
            anchors.horizontalCenter: parent.horizontalCenter
            text: loginDbScreen.text
        }

        Rectangle {
            id: loadProgressBar
            anchors.bottom: parent.bottom
            height: 4
            width: parent.width
            clip: true

            radius: height / 2
            color: loginDbScreen.progressBarBackground

            Rectangle {
                id: contentItemloadProgressBar
                width: loginDbScreen.value * loadProgressBar.width
                height: loadProgressBar.height
                radius: height / 2
                color: loginDbScreen.progressBarForeground
            }
        }
    }
}
