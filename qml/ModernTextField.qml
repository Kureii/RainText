import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Timeline 1.0
import QtGraphicalEffects 1.0


Rectangle {
    id: modernTextField
    width: parent
    height: 22
    clip: true


    property string placeholderText: ""
    property string text: ""
    property color textColor: "#111"
    property color placeholderTextColor: "#6b0f0f"
    property color fillUnderlineColor: "#070a93"
    property color selectionBlockColor: "#bada55"
    property color selectionTextColor: "#000"
    property bool passwordMode: false
    property bool passwordIsHidden: true


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
            font.pixelSize: 16
            color: modernTextField.placeholderTextColor

        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: textAreaObject.forceActiveFocus()
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
        width: parent.width
        echoMode: passwordMode && modernTextField.passwordIsHidden ? TextField.Password : TextField.Normal

        background: Rectangle {
               color: "transparent"
           }

        Connections {
            target: textAreaObject
            onTextChanged: modernTextField.text = textAreaObject.text
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
            anchors.fill: parent
            anchors.margins: 2
            source: modernTextField.passwordIsHidden ? "qrc:/assets/eye.svg" : "qrc:/assets/eye-slash.svg"
            fillMode: Image.PreserveAspectFit
            ColorOverlay {
                anchors.fill: svgImage
                source: svgImage
                color: "#00ffff"
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
        onActiveFocusChanged: if (textAreaObject.focus || textAreaObject.text > 0) {
                                  modernTextField.state = "Extend"
                              } else {
                                  modernTextField.state = "Collapsed"
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
                font.pixelSize: 10
                font.pointSize: 10
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
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 100
                    }
                    PropertyAnimation {
                        target: placeholderTextObject
                        property: "font.pixelSize"
                        duration: 150
                        easing.type: Easing.InOutQuad
                    }
                }

            }

            ParallelAnimation {
                SequentialAnimation {

                    PauseAnimation {
                        duration: 100
                    }
                    PropertyAnimation {
                        target: fillUnderline
                        property: "width"
                        duration: 150
                        easing.type: Easing.InOutQuad
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
    D{i:0}D{i:17;transitionDuration:2000}
}
##^##*/
