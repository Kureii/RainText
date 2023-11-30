import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Timeline 1.0
import Qt5Compat.GraphicalEffects

Rectangle {
    id: modernTextField
    width: parent
    height: 22
    state: "Collapsed"
    clip: true


    property string text: ""
    property string placeholderText: ""
    property color textColor: "#111"
    property color placeholderTextColor: "#6b0f0f"
    property color fillUnderlineColor: "#070a93"
    property color selectionBlockColor: "#bada55"
    property color selectionTextColor: "#000"
    property bool passwordMode: false
    property bool passwordIsHidden: true
    property bool isFocused: false


    Column {
        width: parent.width
        Rectangle {
            width: parent.width
            height:2
            color: "transparent"
        }
        Text {
            id: placeholderTextObject
            width: parent.width
            text: modernTextField.placeholderText
            font.pointSize: 12
            color: modernTextField.placeholderTextColor

        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            textAreaObject.forceActiveFocus()
            modernTextField.isFocused = true
        }
    }

    TextField {
        id: textAreaObject
        y: 16
        color: modernTextField.textColor
        focus: false
        selectByMouse: true
        text: modernTextField.text
        selectedTextColor: modernTextField.selectionTextColor
        selectionColor: modernTextField.selectionBlockColor
        width: passwordMode?  parent.width - 22 :parent.width
        echoMode: passwordMode && modernTextField.passwordIsHidden ? TextField.Password : TextField.Normal

        background: Rectangle {
               color: "transparent"
           }

        Connections {
            target: textAreaObject
            function onTextChanged() {
                modernTextField.text = textAreaObject.text
            }
            function onFocusChanged() {
                modernTextField.isFocused = true
            }
            function onEditingFinished() {
                modernTextField.isFocused = false
            }
        }
    }

    Button {
        id: showHideButton
        width: 21
        height: 21
        y: 22
        visible: passwordMode
        anchors.right: parent.right
        background: Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.width: 0
        }

        Image {
            id: showHideIcon
            anchors.fill: parent
            anchors.margins: 2
            source: modernTextField.passwordIsHidden ? "qrc:/images/eye.svg" : "qrc:/images/eye-slash.svg"
            fillMode: Image.PreserveAspectFit
            ColorOverlay {
                anchors.fill: showHideIcon
                source: showHideIcon
                color: modernTextField.textColor
            }
        }

        onClicked: {
            textAreaObject.forceActiveFocus()
            modernTextField.passwordIsHidden = !modernTextField.passwordIsHidden
        }
    }


    Rectangle {
        id: fillUnderline
        width: 0
        height: 1
        color: modernTextField.fillUnderlineColor
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 2

    }

    Connections {
        target: modernTextField
        function onActiveFocusChanged() {
            if (textAreaObject.focus || textAreaObject.text > 0) {
                modernTextField.state = "Extend"
            } else {
                modernTextField.state = "Collapsed"
            }
        }
    }

    states: [
        State {
            name: "Extend"
            when: textAreaObject.focus || textAreaObject.text.length > 0

            PropertyChanges {
                target: modernTextField
                height: 46
            }

            PropertyChanges {
                target: placeholderTextObject
                font.pointSize: 8
            }

            PropertyChanges {
                target: fillUnderline
                width: modernTextField.width
            }

        },
        State {
            name: "Collapsed"
            when: !textAreaObject.focus && textAreaObject.text.length === 0
        }
    ]
    transitions: [
        Transition {
            id: transition
            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 100
                    }
                    PropertyAnimation {
                        target: modernTextField
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 150
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 100
                    }
                    PropertyAnimation {
                        target: placeholderTextObject
                        property: "font.pointSize"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 150
                    }
                }
                SequentialAnimation {

                    PauseAnimation {
                        duration: 100
                    }
                    PropertyAnimation {
                        target: fillUnderline
                        property: "width"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 150
                    }
                }

            }
            to: "*"
            from: "*"
        }
    ]




}
/*##^##
Designer {
    D{i:0}D{i:11}D{i:17;transitionDuration:2000}
}
##^##*/
