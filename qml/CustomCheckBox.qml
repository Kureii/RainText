import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

CheckBox {
    id: customCheckBox
    text: "Check box text"

    property color borderColor: "#0C011C"
    property color checkedColor: "#FF9800"

    indicator: Rectangle {
        id: rectangle
        width: 18
        height: 18
        border.width: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8


        Rectangle {
            visible: customCheckBox.checked
            height: parent.height - 3
            width: parent.width - 3
            anchors.centerIn: parent
            color: customCheckBox.checkedColor
            radius: 4
        }
        Image {
            id: uncheckedIconImage
            visible: !customCheckBox.checked
            source: "qrc:/images/unchecked-square.svg"
            anchors.fill: parent
            ColorOverlay {
                anchors.fill: uncheckedIconImage
                source: uncheckedIconImage
                color: customCheckBox.borderColor
            }
        }

        Image {
            id: checkedIconImage
            visible: customCheckBox.checked
            source: "qrc:/images/checked-square.svg"
            anchors.fill: parent
            ColorOverlay {
                anchors.fill: checkedIconImage
                source: checkedIconImage
                color: customCheckBox.borderColor
            }
        }


    }
}
