import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Column {
    id: sizePicker
    property string text: "Size picker"
    property int radius: 5
    property color primaryHexColor: "#FF9800"
    property color textColor: "#0C011C"
    property color backgroundColor: "#ECDFD4"
    property real from: 8.0
    property real to: 64.0
    property real stepSize: 1.0
    property real value: 12.0

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Up || event.key === Qt.Key_Right) {
            if (value < passGenLength.to) {
                sizePicker.value += sizePicker.stepSize;
            }
        }
        if (event.key === Qt.Key_Down || event.key === Qt.Key_Left) {
            if (value > passGenLength.from) {
                sizePicker.value -= sizePicker.stepSize;
            }
        }
    }

    function adjustColorBrightness(color, brightness) {
        // Normalizujte hodnotu jasu na rozsah 0.5 - 1
        brightness = (brightness * 0.5) + 0.5;

        // Pokud je barva objekt Color, převeďte ji na hex řetězec
        var hexColor = typeof color === 'string' ? color : color.toString().slice(0, 7);

        // Ořízněte případnou alfa hodnotu a '0x' prefix
        hexColor = hexColor.replace(/^(#|0x)/, '');
        if (hexColor.length > 6) { // ARGB format
            hexColor = hexColor.substring(2); // Odstraňte alfa kanál
        }

        var r = parseInt(hexColor.substring(0, 2), 16);
        var g = parseInt(hexColor.substring(2, 4), 16);
        var b = parseInt(hexColor.substring(4, 6), 16);

        var newR = Math.min(255, r * brightness);
        var newG = Math.min(255, g * brightness);
        var newB = Math.min(255, b * brightness);

        return Qt.rgba(newR / 255, newG / 255, newB / 255, 1);
    }


    Text {
        text: sizePicker.text
        color: sizePicker.textColor
    }
    Rectangle {
        height: 3
        width: formColumn.settingWidth
        color: "transparent"
    }
    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        Slider {
            id: sizePickerSlider
            touchDragThreshold: sizePicker.stepSize
            stepSize: sizePicker.stepSize
            to: sizePicker.to
            from: sizePicker.from
            value: sizePicker.value
            width: formColumn.settingWidth
            Layout.minimumWidth: formColumn.settingWidth - 80
            Layout.maximumWidth: formColumn.settingWidth - 80
            onValueChanged: sizePickerSpinBox.value = sizePickerSlider.value


            property real normalizedPosition: (sizePickerSliderHandle.x - sizePickerSlider.leftPadding) / ((sizePickerSlider.width - sizePickerSlider.rightPadding - sizePickerSliderHandle.width) - sizePickerSlider.leftPadding)

            background: Rectangle {
                x: sizePickerSlider.leftPadding
                y: sizePickerSlider.topPadding + (sizePickerSlider.availableHeight - height) / 2
                width: sizePickerSlider.availableWidth
                height: passwordManagerScreen.cornerRadius * 2
                radius: passwordManagerScreen.cornerRadius
                color: sizePicker.backgroundColor
                border.width: 2
                border.color: sizePicker.adjustColorBrightness(sizePicker.backgroundColor, 0.9)
                Rectangle {
                    width: sizePickerSliderHandle.x + sizePickerSliderHandle.width / 2
                    color: sizePicker.adjustColorBrightness(sizePicker.primaryHexColor, sizePickerSlider.normalizedPosition)
                    height: sizePicker.radius * 2
                    radius: sizePicker.radius
                }
            }

            handle: Rectangle {
                id: sizePickerSliderHandle
                x: sizePickerSlider.leftPadding + sizePickerSlider.visualPosition * (sizePickerSlider.availableWidth - width)
                y: sizePickerSlider.topPadding + (sizePickerSlider.availableHeight - height) / 2
                width: 20
                height: 20
                radius: passwordManagerScreen.cornerRadius
                color: handleMouseArea.pressed ? sizePicker.adjustColorBrightness(sizePicker.backgroundColor, 0.9) : sizePicker.backgroundColor
                border.width: sizePicker.focus ? 2 : 0
                border.color: sizePicker.adjustColorBrightness(sizePicker.primaryHexColor, 1.25)
                MouseArea {
                    id: handleMouseArea
                    anchors.fill: parent
                    drag.target: parent
                    drag.axis: Drag.XAxis
                    drag.minimumX: sizePickerSlider.leftPadding
                    drag.maximumX: sizePickerSlider.width - sizePickerSlider.rightPadding - sizePickerSliderHandle.width

                    onPositionChanged: sizePickerSlider.value = sizePickerSlider.from + sizePickerSlider.normalizedPosition * (sizePickerSlider.to - sizePickerSlider.from)
                }
            }

        }
        SpinBox {
            id: sizePickerSpinBox
            width: 60
            Layout.rightMargin: 10
            Layout.maximumWidth: 60
            Layout.minimumWidth: 60
            stepSize: sizePicker.stepSize
            to: sizePicker.to
            from: sizePicker.from
            value: sizePicker.value
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            onValueChanged: sizePicker.value = sizePickerSpinBox.value

            background: Rectangle {
                color: sizePicker.backgroundColor
                anchors.fill: parent
                radius: passwordManagerScreen.cornerRadius
                border.color: sizePicker.adjustColorBrightness(sizePicker.backgroundColor, 0.9)
                border.width: 1
            }

            contentItem: TextInput {
                text: sizePickerSpinBox.textFromValue(sizePickerSpinBox.value)
                horizontalAlignment: TextInput.AlignHCenter
                verticalAlignment: TextInput.AlignVCenter
                anchors.fill: parent
                anchors.rightMargin: parent.width / 5 * 2
                color: sizePicker.textColor
            }

            up.indicator: Rectangle {
                id: upIndicator
                anchors.right: parent.right
                anchors.top: parent.top
                width: parent.width / 5 * 2
                height: parent.height / 2
                color: upIndicator.enabled ? (upIndicator.hovered ? (upIndicator.clicked ? sizePicker.adjustColorBrightness(sizePicker.primaryHexColor, 0.8) : sizePicker.adjustColorBrightness(sizePicker.primaryHexColor, 0.9)) : sizePicker.primaryHexColor) : sizePicker.adjustColorBrightness(sizePicker.backgroundColor, 0.8)
                radius: passwordManagerScreen.cornerRadius

                Rectangle {
                    color: parent.color
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 0
                    width: parent.width
                    height: parent.radius + 1
                    anchors.bottomMargin: 0
                }

                Rectangle {
                    color: parent.color
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.leftMargin: 0
                    width:  parent.radius + 1
                    height: parent.radius + 1
                    anchors.topMargin: 0
                }
                Image {
                    id: sizePickerSpinBoxUpIndicatorImage
                    source: "qrc:/images/arrow-up.svg"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    ColorOverlay {
                        anchors.fill: sizePickerSpinBoxUpIndicatorImage
                        source: sizePickerSpinBoxUpIndicatorImage
                        color: sizePicker.textColor
                    }
                }
            }

            down.indicator: Rectangle {
                id: downIndicator
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: parent.width / 5 * 2
                height: parent.height / 2
                color: downIndicator.enabled ? (downIndicator.hovered ? (downIndicator.clicked ? sizePicker.adjustColorBrightness(sizePicker.primaryHexColor, 0.8) : sizePicker.adjustColorBrightness(sizePicker.primaryHexColor, 0.9)) : sizePicker.primaryHexColor) : sizePicker.adjustColorBrightness(sizePicker.backgroundColor, 0.8)
                radius: sizePicker.radius
                Rectangle {
                    color: parent.color
                    anchors.top: parent.top
                    width: parent.width
                    height: parent.radius + 1
                    anchors.topMargin: 0
                }

                Rectangle {
                    color: parent.color
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 0
                    width:  parent.radius + 1
                    height: parent.radius + 1
                    anchors.bottomMargin: 0
                }
                Image {
                    id: sizePickerSpinBoxDownIndicatorImage
                    source: "qrc:/images/arrow-down.svg"
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    ColorOverlay {
                        anchors.fill: sizePickerSpinBoxDownIndicatorImage
                        source: sizePickerSpinBoxDownIndicatorImage
                        color: sizePicker.textColor
                    }
                }
            }
        }
    }
}

