import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Item{
    id: iconButton

    property int addToWidth: 20
    width: iconButtonText.width + iconButtonIcon.width + addToWidth

    signal clicked()
    signal pressed()


    property string text: ""
    property url image: ""
    property url imageOff: ""
    property bool changeImage: false
    property color textColor: "#0C011C"
    property color focusColor: "#FF9800"
    property color inactiveColor: "#A19595"
    property color pressedColor: "#e58701"
    property color hoveredColor: "#78562C"
    property bool buttonOn: false
    property bool enabled: true
    property int borderWidth: 1
    property int radius: 5
    property int spacing: 5
    property int imageSize: 18

    Button {
        id: iconButtonButton
        anchors.fill: parent
        enabled: iconButton.enabled

        onClicked: {
            iconButton.clicked();
            iconButton.buttonOn = !iconButton.buttonOn;
        }

        onPressed: iconButton.pressed()

        property color changingColor: iconButtonButton.enabled ? (iconButtonButton.hovered ? (iconButtonButton.pressed ? iconButton.pressedColor : iconButton.hoveredColor) : iconButton.textColor) : iconButton.inactiveColor

        onPressedChanged: {
            if (iconButtonButton.pressedColor) {
                iconButtonButton.changingColor = iconButton.pressedColor
            } else {
                iconButtonButton.changingColor = iconButtonButton.enabled ? (iconButtonButton.hovered ? (iconButtonButton.pressed ? iconButton.pressedColor : iconButton.hoveredColor) : iconButton.textColor) : iconButton.inactiveColor
            }
        }

        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
            radius: iconButton.radius
            border.width: iconButton.borderWidth + 2
            border.color: iconButtonButton.focus ? iconButton.focusColor : "transparent"
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                radius: iconButton.radius
                border.width: iconButton.borderWidth + 1
                border.color: iconButtonButton.changingColor
            }
        }
        RowLayout {
            anchors.fill: parent
            spacing: iconButton.spacing
            Rectangle {
                Layout.preferredWidth: iconButton.imageSize
                visible: iconButton.image !== "";
                Layout.leftMargin: iconButton.text !== "" ? iconButton.spacing : (iconButton.width - iconButton.imageSize) / 2
                Layout.fillHeight: true
                color: "transparent"

                Image {
                    id: iconButtonIconFocus
                    anchors.fill: parent
                    visible: !changeImage && iconButtonButton.focus;
                    source: iconButton.image
                    fillMode: Image.PreserveAspectFit
                    anchors.margins: 0
                    ColorOverlay {
                        anchors.fill: iconButtonIconFocus
                        source: iconButtonIconFocus
                        color: iconButton.focusColor
                    }
                }

                Image {
                    id: iconButtonIcon
                    anchors.fill: parent
                    visible: !changeImage;
                    source: iconButton.image
                    fillMode: Image.PreserveAspectFit
                    anchors.margins: 2
                    ColorOverlay {
                        anchors.fill: iconButtonIcon
                        source: iconButtonIcon
                        color: iconButtonButton.changingColor
                    }
                }

                Image {
                    id: iconButtonIconFocusOnOff
                    anchors.fill: parent
                    visible: changeImage && iconButtonButton.focus;
                    source: iconButton.buttonOn ? iconButton.image : iconButton.imageOff
                    fillMode: Image.PreserveAspectFit
                    anchors.margins: 0
                    ColorOverlay {
                        anchors.fill: iconButtonIconFocusOnOff
                        source: iconButtonIconFocusOnOff
                        color: iconButton.focusColor
                    }
                }

                Image {
                    id: iconButtonIconOnOff
                    anchors.fill: parent
                    visible: changeImage;
                    source: iconButton.buttonOn ? iconButton.image : iconButton.imageOff
                    fillMode: Image.PreserveAspectFit
                    anchors.margins: 2
                    ColorOverlay {
                        anchors.fill: iconButtonIconOnOff
                        source: iconButtonIconOnOff
                        color: iconButtonButton.changingColor
                    }
                }
            }

            Text {
                id: iconButtonText
                text: iconButton.text
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                Layout.rightMargin: iconButton.spacing
                Layout.fillHeight: true
                font.bold: false
                color: iconButtonButton.changingColor
            }
        }
    }
}
