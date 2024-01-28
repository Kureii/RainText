import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs

Item {
    id: passwordRecordListItem


    property color backgroundColor: "#ECDFD4"
    property color textColor: "#0C011C"
    property color focusColor: "#FF9800"
    property color borderButtonColor: "#1C0F01"
    property color borderButtonFocusColor: "#FFA000"
    property color notEnabledBorderButtonColor: "#75614B"
    property color borderButtonHoveredColor: "#78562C"
    property color borderButtonPressedColor: "#9b703b"
    property color lineColor: "#695981"
    property string headlineText: "Headline"
    property string usernameText: "Username"
    property string passwordText: "Password"
    property string passwordDotText: "⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁"

    signal requestDeletion(int index)

    signal requestMoveUp(int index)

    signal requestMoveDown(int index)

    signal requestEdit(int index, string headline, string username, string password)

    Popup {
        id: toastPopup
        visible: false
        background: Rectangle {
            color: passwordRecordListItem.backgroundColor
            radius: 8
            border.color: passwordRecordListItem.focusColor
            height: toastPopup.height + 4
            width: toastPopup.width + 12
            border.width: 2
        }

        Label {
            text: toastPopup.message
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        property string message: ""

        Timer {
            id: toastTimer
            interval: 2000
            onTriggered: toastPopup.close()
        }

        function showToast(msg) {
            message = msg;
            toastPopup.open();
            toastTimer.restart();
        }
    }


    DropShadow {
        id: passwordRecordListItemShadow
        height: passwordRecordListRectangleToShadow.height
        width: passwordRecordListRectangleToShadow.width
        horizontalOffset: 0
        verticalOffset: 0
        radius: 0
        color: "#55000000"
        source: passwordRecordListRectangleToShadow
    }

    Rectangle {
        id: passwordRecordListRectangleToShadow
        height: parent.height - 5
        width: parent.width - 5
        scale: 1
    }
    Rectangle {
        id: passwordRecordListRectangle
        height: passwordRecordListRectangleToShadow.height
        width: passwordRecordListRectangleToShadow.width
        scale: 1
        color: passwordRecordListItem.backgroundColor
        clip: true
        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.preferredHeight: 1
                color: passwordRecordListItem.lineColor
                Layout.fillWidth: true
            }

            RowLayout {
                spacing: 0


                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                ColumnLayout {
                    Layout.preferredHeight: passwordRecordListRectangle.height
                    Layout.preferredWidth: passwordRecordListRectangle.width / 12
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    IconButton {
                        enabled: index !== 0
                        text: ""
                        height: 18
                        image: "qrc:/images/arrow-up.svg"
                        Layout.maximumWidth: passwordRecordListRectangle.width / 12
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        onClicked: requestMoveUp(index)
                        textColor: passwordRecordListItem.textColor
                        focusColor: passwordRecordListItem.focusColor
                        inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                        pressedColor: passwordRecordListItem.borderButtonPressedColor
                        hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                    }

                    IconButton {
                        enabled: passwordListModel.count - 1 !== index
                        text: ""
                        height: 18
                        image: "qrc:/images/arrow-down.svg"
                        Layout.maximumWidth: passwordRecordListRectangle.width / 12
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                        onClicked: requestMoveDown(index)
                        textColor: passwordRecordListItem.textColor
                        focusColor: passwordRecordListItem.focusColor
                        inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                        pressedColor: passwordRecordListItem.borderButtonPressedColor
                        hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                    }
                }

                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Rectangle {
                    id: rectangle1
                    Layout.preferredWidth: 1
                    Layout.preferredHeight: passwordRecordListRectangle.height - 2
                    color: passwordRecordListItem.lineColor
                    Layout.bottomMargin: 2
                    Layout.topMargin: 2
                    Layout.fillHeight: true
                }

                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.preferredWidth: passwordRecordListRectangle.width / 24 * 18

                    RowLayout {

                        TextArea {
                            id: copyHeadlineText
                            text: passwordRecordListItem.headlineText
                            Layout.fillWidth: true
                            selectByMouse: true
                            readOnly: true
                        }

                        Timer {
                            id: copyHeadlineTimer
                            interval: 5000
                            onTriggered: {
                                copyHeadlineButton.image = "qrc:/images/copy.svg"
                            }
                        }

                        IconButton {
                            id: copyHeadlineButton
                            text: ""
                            image: "qrc:/images/copy.svg"
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            textColor: passwordRecordListItem.textColor
                            focusColor: passwordRecordListItem.focusColor
                            inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                            pressedColor: passwordRecordListItem.borderButtonPressedColor
                            hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                            onClicked: {
                                copy_to_clipboard.copyToClipboard(copyHeadlineText.text)
                                copyHeadlineButton.image = "qrc:/images/copy-success.svg"
                                copyHeadlineTimer.restart()
                                toastPopup.showToast(qsTr("Headline was copied to clipboard successfully"))
                                copyHeadlineButton.focus = false
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredHeight: 1
                        color: passwordRecordListItem.lineColor
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        TextArea {
                            id: copyUsernameItemText
                            text: passwordRecordListItem.usernameText
                            Layout.fillWidth: true
                            selectByMouse: true
                            readOnly: true
                        }

                        Timer {
                            id: copyUsernameItemTimer
                            interval: 5000
                            onTriggered: {
                                copyUsernameItemButton.image = "qrc:/images/copy.svg"
                            }
                        }

                        IconButton {
                            id: copyUsernameItemButton
                            text: ""
                            image: "qrc:/images/copy.svg"
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            textColor: passwordRecordListItem.textColor
                            focusColor: passwordRecordListItem.focusColor
                            inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                            pressedColor: passwordRecordListItem.borderButtonPressedColor
                            hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                            onClicked: {
                                copy_to_clipboard.copyToClipboard(copyUsernameItemText.text)
                                copyUsernameItemButton.image = "qrc:/images/copy-success.svg"
                                copyUsernameItemTimer.restart()
                                toastPopup.showToast(qsTr("Username was copied to clipboard successfully"))
                                copyUsernameItemText.focus = false
                            }
                        }
                    }


                    RowLayout {
                        Layout.fillWidth: true

                        TextArea {
                            id: passwordRecordText
                            text: passwordRecordListItem.passwordDotText
                            property string tempText: passwordRecordListItem.passwordText
                            Layout.fillWidth: true
                            selectByMouse: true
                            readOnly: true
                            Keys.onPressed: (event) => {
                                if (event.key === Qt.Key_C && (event.modifiers & Qt.ControlModifier)) {
                                    event.accepted = true;
                                    if (passwordRecordText.text === passwordRecordListItem.passwordDotText) {
                                        copy_to_clipboard.copyToClipboard(passwordRecordText.tempText)
                                    } else {
                                        copy_to_clipboard.copyToClipboard(passwordRecordText.text)
                                    }
                                }
                            }
                        }

                        Timer {
                            id: copyPasswordItemTimer
                            interval: 5000
                            onTriggered: {
                                passwordRecordButton.image = "qrc:/images/copy.svg"
                            }
                        }

                        IconButton {
                            image: "qrc:/images/eye-slash.svg"
                            changeImage: true
                            imageOff: "qrc:/images/eye.svg"
                            text: ""
                            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            textColor: passwordRecordListItem.textColor
                            focusColor: passwordRecordListItem.focusColor
                            inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                            pressedColor: passwordRecordListItem.borderButtonPressedColor
                            hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                            onClicked: {
                                var textA = passwordRecordText.text
                                var textB = passwordRecordText.tempText
                                passwordRecordText.text = textB
                                passwordRecordText.tempText = textA
                            }
                        }

                        IconButton {
                            id: passwordRecordButton
                            image: "qrc:/images/copy.svg"
                            text: ""
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            textColor: passwordRecordListItem.textColor
                            focusColor: passwordRecordListItem.focusColor
                            inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                            pressedColor: passwordRecordListItem.borderButtonPressedColor
                            hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                            onClicked: {
                                if (passwordRecordText.text === passwordRecordListItem.passwordDotText) {
                                    copy_to_clipboard.copyToClipboard(passwordRecordText.tempText)
                                } else {
                                    copy_to_clipboard.copyToClipboard(passwordRecordText.text)
                                }
                                passwordRecordButton.image = "qrc:/images/copy-success.svg"
                                copyPasswordItemTimer.restart()
                                toastPopup.showToast(qsTr("Password was copied to clipboard successfully"))
                                passwordRecordButton.focus = false
                            }
                        }
                    }
                }

                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Rectangle {
                    width: 1
                    height: passwordRecordListRectangle.height - 2
                    color: passwordRecordListItem.lineColor
                }

                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                RowLayout {
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48 * 7 - 4
                    Layout.maximumWidth: passwordRecordListRectangle.width / 48 * 7 - 4

                    IconButton {
                        id: editRecord
                        image: "qrc:/images/edit.svg"
                        text: ""
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 24
                        textColor: passwordRecordListItem.textColor
                        focusColor: passwordRecordListItem.focusColor
                        inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                        pressedColor: passwordRecordListItem.borderButtonPressedColor
                        hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                        onClicked: {
                            if (passwordRecordText.text === "⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁ ⦁") {
                                requestEdit(index, copyHeadlineText.text, copyUsernameItemText.text, passwordRecordText.tempText)
                            } else {
                                requestEdit(index, copyHeadlineText.text, copyUsernameItemText.text, passwordRecordText.text)
                            }
                        }
                    }

                    IconButton {
                        id: deleteRecord
                        image: "qrc:/images/trash.svg"
                        text: ""
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: 24
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        onClicked: requestDeletion(index)
                        textColor: passwordRecordListItem.textColor
                        focusColor: passwordRecordListItem.focusColor
                        inactiveColor: passwordRecordListItem.notEnabledBorderButtonColor
                        pressedColor: passwordRecordListItem.borderButtonPressedColor
                        hoveredColor: passwordRecordListItem.borderButtonHoveredColor
                    }
                }

                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.preferredWidth: passwordRecordListRectangle.width / 48
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
            }
        }
    }
}
