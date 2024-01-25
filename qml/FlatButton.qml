import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Button {
    id: flatButton
    text: "Flat button"
    width: 350
    height: 60

    property int cornerRadius: 5

    property color primaryColor: "#FF9800"
    property color primaryPressedColor: "#8A5300"
    property color primaryHoveredColor: "#b56c00"
    property color notEnablePrimaryColor: "#F0B762"
    property color textColorOnPrimyry: "#150133"
    property color notEnableTextColor: "#453957"
    property color textOnPrimaryColor: "#1C1001"
    property color focusOnPrimaryColor: "#E3C59A"

    background: Rectangle {
        id: buttonBackground
        anchors.fill: parent
        color: flatButton.enabled ? (flatButton.hovered ? flatButton.primaryHoveredColor : flatButton.primaryColor) : flatButton.notEnablePrimaryColor
        radius: flatButton.cornerRadius * 2
        border.width: flatButton.pressed ? 2 : 0
        border.color: flatButton.primaryPressedColor
        Rectangle {
            visible: flatButton.focus
            anchors.centerIn: parent
            height: parent.height - 4
            width: parent.width - 4
            color: "transparent"
            border.width: 2
            border.color: flatButton.pressed? flatButton.primaryPressedColor : flatButton.focusOnPrimaryColor
            radius: flatButton.cornerRadius * 2 - 2
        }
    }

    contentItem: Text {
        id: flatButtonText
        text: flatButton.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font: flatButton.font
        color: flatButton.enabled ? flatButton.textColorOnPrimyry : flatButton.notEnableTextColor
        anchors.centerIn: parent
    }
}