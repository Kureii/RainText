import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: borderedButton

    property color normalColor: "#f00" //"#ECDFD4"
    property color normalBackgroundColor: "transparent" //"#ECDFD4"
    property color hoveredColor: "#0f0" //"#D1C7BE"
    property color hoveredBackgroundColor: "transparent" //"#ECDFD4"
    property color pressedColor: "#00f" //"#ECDED3"
    property color pressedBackgroundColor: "transparent" //"#ECDFD4"


    background:
        Rectangle {
            color: borderedButton.hovered ? (borderedButton.pressed ? borderedButton.pressedBackgroundColor : borderedButton.hoveredBackgroundColor) : borderedButton.normalBackgroundColor
            anchors.fill: borderedButton
            radius: 5
            border.color: borderedButton.hovered ? (borderedButton.pressed ? borderedButton.pressedColor : borderedButton.hoveredColor) : borderedButton.normalColor
            border.width: 2


    }

    contentItem: Text {
        text: borderedButton.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font: borderedButton.font
        color: borderedButton.hovered ? (borderedButton.pressed ? borderedButton.pressedColor : borderedButton.hoveredColor) : borderedButton.normalColor
        anchors.centerIn: parent
    }
}