import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: passwordManagerScreen

    property int cornerRadius: 5

    Button {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        onClicked: bottomDrawer.open()
        anchors.rightMargin: 12
        anchors.bottomMargin: 12
        height: 32
        width: addRecordButtonIcon.width + addRecordButtonText.width + 15

        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
            radius:5
            border.width: 1
            border.color: "#0C011C"
        }
        RowLayout {
            anchors.fill: parent
            spacing: 5
            Image {
                id: addRecordButtonIcon

                source: "qrc:/images/add.svg"
                Layout.leftMargin: 5
                Layout.fillHeight: true
                fillMode: Image.PreserveAspectFit
                ColorOverlay {
                    anchors.fill: addRecordButtonIcon
                    source: addRecordButtonIcon
                    color: "#0C011C"
                }
            }

            Text {
                id: addRecordButtonText
                text: "Add record"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                Layout.rightMargin: 5
                Layout.fillHeight: true
                font.bold: false
                color: "#0C011C"
            }
        }
    }

    Button {
        id: settingsButton
        anchors.rightMargin: 12
        anchors.topMargin: 12
        anchors.top: parent.top
        anchors.right: parent.right
        height:28
        width: settingsButtonText.width + settingsButtonIcon.width + 20
        onClicked: rightDrawer.open()

        background: Rectangle{
            anchors.fill: parent
            color: "transparent"
            radius:5
            border.width: 1
            border.color: "#0C011C"
        }
        RowLayout {
            anchors.fill: parent
            spacing: 5
            Image {
                id: settingsButtonIcon
                source: "qrc:/images/setting.svg"
                Layout.leftMargin: 5
                Layout.fillHeight: true
                fillMode: Image.PreserveAspectFit
                ColorOverlay {
                    anchors.fill: settingsButtonIcon
                    source: settingsButtonIcon
                    color: "#0C011C"
                }
            }

            Text {
                id: settingsButtonText
                text: "Settings"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                Layout.rightMargin: 5
                Layout.fillHeight: true
                font.bold: false
                color: "#0C011C"
            }
        }
    }

    Drawer {
        id: bottomDrawer
        width: parent.width
        height: parent.height / 5 * 4
        edge: Qt.BottomEdge
        Column {
            id: formColumn
            anchors.centerIn: parent
            spacing: 5
            anchors.fill: parent
            leftPadding: (parent.width - formWidth) / 2
            rightPadding: (parent.width - formWidth) / 2

            property int formWidth: formColumn.width - parent.width / 5 * 2
            property int settingWidth: 40 + Math.max(majusculeGenPassChBox.width, minusculeGenPassChBox.width) + Math.max(numberGenPassChBox.width, specialGenPassChBox.width)

            Rectangle {
                width: formColumn.formWidth
                height: (parent.height - formHeadlineField.height - formUsernameField.height - formPasswordField.height - formConfirmButton.height - formAfterPasswordSpace.height - headlineText.height - headllineSeparator.height - 8 * formColumn.spacing)/2
                color: "transparent"
            }
            Text {
                id: headlineText
                text: "Create record"
            }
            Rectangle {
                id: headllineSeparator
                color: "#0C011C"
                width: formColumn.formWidth
                height: 1
            }
            Row {
                width: formColumn.formWidth
                height: Math.max(settingsGenPass.height, formHeadlineField.height + formUsernameField.height + formPasswordField.height)
                Column {

                    ModernTextField {
                        id: formHeadlineField
                        width: formColumn.formWidth - formColumn.settingWidth
                        placeholderText: "Headline"
                        placeholderTextColor: "#0C011C"
                        fillUnderlineColor: "#0C011C"
                        color: "transparent"
                    }

                    ModernTextField {
                        id: formUsernameField
                        width: formColumn.formWidth - formColumn.settingWidth
                        placeholderText: "Username"
                        placeholderTextColor: "#0C011C"
                        fillUnderlineColor: "#0C011C"
                        color: "transparent"
                    }

                    ModernTextField {
                        id: formPasswordField
                        width: formColumn.formWidth - formColumn.settingWidth
                        placeholderText: "Password"
                        passwordMode: true
                        placeholderTextColor: "#0C011C"
                        fillUnderlineColor: "#0C011C"
                        color: "transparent"
                        /*Connections {
                            target:formPasswordField
                            function onTextChanged() {
                                backgroundLoginRegister.canConfirmForm = login_register_manager.CheckFields(formUsernameField.text,
                                    formPasswordField.text,
                                    formPasswordAgain.text)
                                backgroundLoginRegister.passwordProgress = login_register_manager.PasswordStrength(formPasswordField.text)
                            }
                            function onIsFocusedChanged() {
                                if (backgroundLoginRegister.state !== "loginState") {
                                    if (formPasswordField.isFocused) {
                                        backgroundLoginRegister.state = "registerStateProgress"
                                    } else {
                                        backgroundLoginRegister.state = "registerState"

                                    }
                                }
                            }
                        }*/
                    }
                }

                Rectangle {
                    height: parent.height
                    width: 10
                    color: "transparent"
                }

                Column {
                    id: settingsGenPass
                    width: formColumn.settingWidth
                    Row {
                        Column {
                            width: Math.max(majusculeGenPassChBox.width, minusculeGenPassChBox.width)
                            CustomCheckBox {
                                id: majusculeGenPassChBox
                                text: "Upper latters"
                            }
                            CustomCheckBox {
                                id: minusculeGenPassChBox
                                text: "Lower latters"
                            }
                        }
                        Rectangle {
                            width: 30
                            height: majusculeGenPassChBox.height + numberGenPassChBox.height
                            color: "transparent"

                        }
                        Column {
                            width: Math.max(numberGenPassChBox.width, specialGenPassChBox.width)
                            CustomCheckBox {
                                id: numberGenPassChBox
                                text: "Numbers"
                            }
                            CustomCheckBox {
                                id: specialGenPassChBox
                                text: "Special chars"
                            }
                        }
                    }

                    Rectangle {
                        height: 10
                        width: formColumn.settingWidth
                        color: "transparent"
                    }
                    Row {
                        Rectangle {
                            width: 30
                            height: 10
                            color: "transparent"
                        }
                        Button {
                            id: buttonGenPass
                            width: formColumn.settingWidth - 60
                            enabled: majusculeGenPassChBox.checked || minusculeGenPassChBox.checked || numberGenPassChBox.checked || specialGenPassChBox.checked
                            text: "Generate password"
                            height: 25
                            background: Rectangle {
                                color: buttonGenPass.enabled ? (buttonGenPass.hovered? "#ffc89f" : "#ffddc2") : "#a69c94"
                                radius: passwordManagerScreen.cornerRadius * 2
                                border.width: 1
                                border.color: buttonGenPass.enabled ? (buttonGenPass.hovered ? (buttonGenPass.clicked ? "#e58701" : "#ef8f00") : "#FF9800") : "#A19595"
                            }

                            contentItem: Text {
                                text: buttonGenPass.text
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font: buttonGenPass.font
                                color: buttonGenPass.enabled ? "#0C011C" : "#021F3C"
                                anchors.centerIn: parent
                            }
                            //enabled: backgroundLoginRegister.canConfirmForm
                            //onClicked: login_register_manager.ConfirmFormUser(formUsernameField.text, formPasswordField.text)
                        }
                    }
                }
            }

            Rectangle {
                id: formAfterPasswordSpace
                height: 12
                width: formColumn.formWidth
                color: "transparent"
            }


            Button {
                id: formConfirmButton
                width: formColumn.formWidth
                text: "Save record"
                height: 40
                enabled: formHeadlineField.text !== "" && formUsernameField.text !== "" && formPasswordField.text !== ""
                background: Rectangle {
                    id: buttonBackground
                    color: formConfirmButton.enabled ? (formConfirmButton.hovered ? (formConfirmButton.clicked ? "#e58701" : "#ef8f00") : "#FF9800") : "#A19595"
                    radius: passwordManagerScreen.cornerRadius * 2
                    border.width: formConfirmButton.pressed ? 2 : 0
                    border.color: formConfirmButton.pressed ? "#734500" : "transparent"
                }

                contentItem: Text {
                    text: formConfirmButton.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font: formConfirmButton.font
                    color: formConfirmButton.enabled ? "#0C011C" : "#021F3C"
                    anchors.centerIn: parent
                }
                //enabled: backgroundLoginRegister.canConfirmForm
                //onClicked: login_register_manager.ConfirmFormUser(formUsernameField.text, formPasswordField.text)
            }
        }

    }

    Drawer {
        id: rightDrawer
        width: parent.width / 3
        height: parent.height
        edge: Qt.RightEdge
        Column {
            spacing: 10
            anchors.fill: parent

            Switch {
                text: "Nastavení 1"
            }

            Switch {
                text: "Nastavení 2"
            }

            // Další nastavení...
        }
        // Přidat obsah pro pravé menu
    }

    Rectangle {
        id: overlay
        color: "#77000000"
        anchors.fill: parent
        visible: bottomDrawer.opened || rightDrawer.opened
        MouseArea {
            anchors.fill: parent
            onClicked: {
                bottomDrawer.close()
                rightDrawer.close()
            }
        }
    }
}
