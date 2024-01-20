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
    property color textColor: "#0C011C"
    property color placeholderTextColor: "#0C011C"
    property color fillUnderlineColor: "#0C011C"
    property color selectionBlockColor: "#FF9800"
    property color selectionTextColor: "#0C011C"
    property bool passwordMode: false
    property bool passwordIsHidden: true
    property bool isFocused: false

    onTextChanged: {
        textAreaObject.text = modernTextField.text
    }

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
                modernTextField.isFocused = !modernTextField.isFocused
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
            id: showHideIconFocus
            visible: showHideButton.focus
            anchors.fill: parent
            anchors.margins: 0
            source: modernTextField.passwordIsHidden ? "qrc:/images/eye.svg" : "qrc:/images/eye-slash.svg"
            fillMode: Image.PreserveAspectFit
            ColorOverlay {
                anchors.fill: showHideIconFocus
                source: showHideIconFocus
                color: modernTextField.selectionBlockColor
            }
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

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Space || event.key === Qt.Key_Enter) {
                event.accepted = true
                modernTextField.passwordIsHidden = !modernTextField.passwordIsHidden
                showHideButton.focus = true
                modernTextField.isFocused = true

            }
        }

    }


    Rectangle {
        id: fillUnderline
        width: 0
        height: 1
        color: textAreaObject.focus? modernTextField.selectionBlockColor : modernTextField.fillUnderlineColor
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
            when: textAreaObject.focus || textAreaObject.text.length > 0 || showHideButton.focus

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
            when: !textAreaObject.focus && textAreaObject.text.length === 0 && !showHideButton.focus
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
