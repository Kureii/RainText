import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Timeline 1.0

Rectangle {
    id: backgroundLoginRegister
    anchors.centerIn: parent
    width: 780
    height: 380
    radius: cornerRadius

    property int cornerRadius: 5
    property int sideMargin: 30

    Row {
        id: rowContainer
        anchors.fill: parent
        anchors.leftMargin: parent.sideMargin
        anchors.rightMargin: parent.sideMargin

        Rectangle {
            id: backgroundRegister
            height: backgroundLoginRegister.height
            width: backgroundLoginRegister.width / 2 - 30
            color: "transparent"
            Button {
                id: backgroundRegisterButton
                anchors.centerIn: parent
                text: "Register"
                onClicked: backgroundFormLoginRegister.state = "registerState"

                Connections {
                    target: backgroundRegisterButton
                    onClicked: backgroundLoginRegister.state = "registerState"
                }

                background: Rectangle {
                    color: "transparent"
                    radius: 5
                    border.color: backgroundRegisterButton.hovered ? "#777" : "1d1d1d"
                    border.width: 1

                    Rectangle {
                        visible: backgroundRegisterButton.pressed
                        color: "#111"
                        anchors.fill: parent
                        radius: 5 // Zaoblení rohů
                    }
                }

                contentItem: Text {
                    text: backgroundRegisterButton.text
                    font: backgroundRegisterButton.font
                    color: backgroundRegisterButton.enabled ? "#222" : "#888"
                    anchors.centerIn: parent
                }
            }
        }
        Rectangle {
            id: backgroundLogin
            height: backgroundLoginRegister.height
            width: backgroundLoginRegister.width / 2 - 30
            color: "transparent"
            z:5

            Button {
                id: backgroundLoginButton
                anchors.centerIn: parent
                text: "Login"
                onClicked: backgroundFormLoginRegister.state = "loginState"
                z:6

                Connections {
                    target: backgroundLoginButton
                    onClicked: backgroundLoginRegister.state = "loginState"
                }
            }

        }
    }
    Rectangle {
        id: backgroundFormLoginRegister
        anchors.centerIn: parent
        width: (backgroundLoginRegister.width - parent.sideMargin * 2) / 2
        height: backgroundLoginRegister.height *1.2
        color: "#e1a478"
        anchors.horizontalCenterOffset: width / 2
        radius: backgroundLoginRegister.cornerRadius

        Rectangle {
            id: backgroundForm
            anchors.centerIn: parent
            height: formUpperSpace.height + formHeadline.height + formUsernameField.height + formUsernamePasswordSpace.height + formPasswordField.height + formAfterPasswordSpace.height + formConfirmButton.height + 2 * formColumn.padding + formPasswordAgain.height + formAfterPasswordAgainSpace.height
            width: parent.width - 50
            color: "transparent"

            Column {
                id: formColumn
                anchors.fill: parent
                anchors.centerIn: parent
                padding: 12

                property int formWidth: formColumn.width - formColumn.padding * 2

                Rectangle {
                    id: formUpperSpace
                    height: 15
                    width: formColumn.formWidth
                    color: "transparent"

                }

                Text {
                    id: formHeadline
                    text: "Login"
                    width: formColumn.formWidth
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    bottomPadding: 8
                    font.pointSize: 14
                }

                ModernTextField {
                    id: formUsernameField
                    width: formColumn.formWidth
                    placeholderText: "Username"
                    placeholderTextColor: "#ff0000"
                    fillUnderlineColor: "#ff0000"
                    color: "transparent"
                }

                Rectangle {
                    id: formUsernamePasswordSpace
                    height: 12
                    width: formColumn.formWidth
                    color: "transparent"
                }

                ModernTextField {
                    id: formPasswordField
                    width: formColumn.formWidth
                    placeholderText: "Password"
                    passwordMode: true
                    placeholderTextColor: "#ff0000"
                    fillUnderlineColor: "#ff0000"
                    color: "transparent"
                }

                Rectangle {
                    id: formAfterPasswordSpace
                    height: 12
                    width: formColumn.formWidth
                    color: "transparent"
                }

                ModernTextField {
                    id: formPasswordAgain
                    width: formColumn.formWidth
                    height: 0
                    placeholderText: "Password again"
                    passwordMode: true
                    placeholderTextColor: "#ff0000"
                    fillUnderlineColor: "#ff0000"
                    color: "transparent"

                }

                Rectangle {
                    id: formAfterPasswordAgainSpace
                    height: 0
                    width: formColumn.formWidth
                    color: "transparent"
                }

                Button {
                    id: formConfirmButton
                    width: formColumn.formWidth
                    text: "login"
                   /* background: Rectangle {
                        id: buttonBackground
                        color: formConfirmButton.hovered ? "lightblue" : "blue"
                        radius: 5
                        border.color: "black"
                        border.width: 1

                        Rectangle {
                            visible: formConfirmButton.pressed
                            color: "darkblue"
                            anchors.fill: parent
                            radius: 5 // Zaoblení rohů
                        }
                    }

                    contentItem: Text {
                        text: formConfirmButton.text
                        font: formConfirmButton.font
                        color: formConfirmButton.enabled ? "white" : "lightgray"
                        anchors.centerIn: parent
                    }*/
                }

            }
        }
    }

    Timeline {
        id: loginTimeline
        animations: [
            TimelineAnimation {
                id: loginAnimation
                running: false
                loops: 1
                duration: 250
                to: 250
                from: 0
            }
        ]
        startFrame: 0
        endFrame: 250
        enabled: false

        KeyframeGroup {
            target: backgroundFormLoginRegister
            property: "anchors.horizontalCenterOffset"
            Keyframe {
                value: -1 * (backgroundFormLoginRegister.width / 2)
                frame: 0
            }

            Keyframe {
                value: backgroundFormLoginRegister.width / 2
                frame: 250
            }
        }


    }
    
    Timeline {
        id: registrationTimeline
        animations: [
            TimelineAnimation {
                id: registrationAnimation
                running: false
                loops: 1
                duration: 250
                to: 250
                from: 0
            }
        ]
        startFrame: 0
        endFrame: 250
        enabled: false

        KeyframeGroup {
            target: backgroundFormLoginRegister
            property: "anchors.horizontalCenterOffset"
            Keyframe {
                value: backgroundFormLoginRegister.width / 2
                frame: 0
            }

            Keyframe {
                value: -1 * (backgroundFormLoginRegister.width / 2)
                frame: 250
            }
        }
    }



    states: [
        State {
            name: "loginState"

            PropertyChanges {
                target: formAfterPasswordAgainSpace
                height: 0
            }

            PropertyChanges {
                target: formPasswordAgain
                height: 0
            }

            PropertyChanges {
                target: registrationTimeline
                enabled: false
            }
            
            PropertyChanges {
                target: loginTimeline
                enabled: true
            }
            
            PropertyChanges {
                target: loginAnimation
                running: true
            }
            
            
        },
        State {
            name: "registerState"
            PropertyChanges {
                target: formAfterPasswordAgainSpace
                height: 12
            }

            PropertyChanges {
                target: formPasswordAgain
                height: formPasswordAgain.text.length === 0 ? 22 : 40
            }
            PropertyChanges {
                target: loginTimeline
                enabled: false
            }
            
            PropertyChanges {
                target: registrationTimeline
                enabled: true
            }
            
            PropertyChanges {
                target: registrationAnimation
                running: true
            }

        }
    ]
    transitions: [
        Transition {
            id: transition
            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }

                    PropertyAnimation {
                        target: formPasswordAgain
                        property: "height"
                        duration: 150
                    }
                }
            }

            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }

                    PropertyAnimation {
                        target: formAfterPasswordAgainSpace
                        property: "height"
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
    D{i:0}D{i:42;transitionDuration:2000}
}
##^##*/
