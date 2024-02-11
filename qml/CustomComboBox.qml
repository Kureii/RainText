import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

ComboBox {
    id:customComboBox
    model:["one","two","three","four"]
    implicitHeight: 55
    implicitWidth: 300

    property real radius: 12
    property color backgroundColor: "#ECDFD4"
    property color checkedColor: "#Fa7700"
    property color textColor: "#0C011C"
    property color primaryColor: "#FF9800"
    property color textColorOnPrimyry: "#150133"
    property color drawerBackgroundColor: "#FAF6F2"


    delegate: ItemDelegate{
        id:itemDelegate
        width: customComboBox.width - 10
        height: customComboBox.implicitHeight
        hoverEnabled: true
        focus: true

        background: Rectangle{
            anchors.fill: parent
            radius: customComboBox.radius
            color: itemDelegate.hovered ? customComboBox.primaryColor : customComboBox.backgroundColor
        }

        RowLayout{
            Layout.alignment: Qt.AlignVCenter
            width: parent.width
            height: parent.height
            anchors.fill: parent
            spacing: 10
            Layout.leftMargin: 5
            Layout.rightMargin: 5

            Label{
                opacity: 0.9
                text: modelData
                color: itemDelegate.hovered ? customComboBox.textColorOnPrimyry : customComboBox.textColor
                Layout.fillWidth: true
                verticalAlignment: Image.AlignVCenter
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 5
            }

                       Image{
                id: selected
                           visible: customComboBox.currentIndex === index
                           source: "qrc:/images/checked.svg"
                           verticalAlignment: Image.AlignVCenter
                           horizontalAlignment: Image.AlignRight
                           Layout.alignment: Qt.AlignVCenter
                           Layout.rightMargin: 10
                           ColorOverlay {
                               anchors.fill: selected
                               source: selected
                               color: itemDelegate.hovered ? customComboBox.textColorOnPrimyry : customComboBox.textColor
                           }
                       }
        }
    }

    contentItem:Item{
        width: customComboBox.background.width - customComboBox.indicator.width - 10
        height: customComboBox.background.height
        RowLayout{
            anchors.fill: parent
            spacing: 10
            Label{
                opacity: 0.9
                text: customComboBox.displayText
                Layout.fillWidth: true
                verticalAlignment: Image.AlignVCenter
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 5
                color: customComboBox.textColor
            }
        }
    }


    background: Rectangle{
        width: customComboBox.width
        height: customComboBox.height
        color: customComboBox.down ? customComboBox.backgroundColor : customComboBox.drawerBackgroundColor
        radius: customComboBox.radius
        border.width: customComboBox.activeFocus ? 2 : 0.6
        border.color: customComboBox.textColor
    }

    popup: Popup{
        y:customComboBox.height
        width: customComboBox.width
        height: Math.min(customComboBox.model.count * 40, mainWindow.height / 3)
        padding: 4
        contentItem: ListView{
            //leftMargin: 2
            implicitHeight: contentHeight
            keyNavigationEnabled: true
            model:customComboBox.popup.visible ? customComboBox.delegateModel : null
            clip: true
            focus: true
            currentIndex: customComboBox.highlightedIndex
        }

        background: Rectangle{
            anchors.fill: parent
            color: customComboBox.backgroundColor
            radius: customComboBox.radius
            border.width: 0.6
            border.color: customComboBox.textColor
            clip: true
        }
    }

    indicator: Image {
        id: customIndicator
        source: "qrc:/images/arrow-down.svg"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        width: customComboBox.height / 2
        height: customComboBox.height / 2
        ColorOverlay {
            anchors.fill: customIndicator
            source: customIndicator
            color: customComboBox.textColor
        }
    }

}