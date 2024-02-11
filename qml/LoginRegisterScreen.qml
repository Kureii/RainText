import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Timeline 1.0
import Qt5Compat.GraphicalEffects


Rectangle {
    id: backgroundLoginRegister
    anchors.centerIn: parent
    width: 780
    height: 380
    radius: cornerRadius
    color: backgroundLoginRegister.loginRegisterBackgroundColor
    state: "loginState"

    property bool canConfirmForm
    property int cornerRadius: 5
    property int sideMargin: 30
    property real passwordProgress: 0.001
    property color loginRegisterBackgroundColor: "#021F3C"
    property color registerLoginSwapButtonBorderColor: "#ECDFD4"
    property color registerLoginSwapButtonFillColor: "#00ECDFD4"
    property color registerLoginSwapButtonFillHoveredColor: "#11D1C7BE"
    property color registerLoginSwapButtonFillPressedColor: "#33ECDED3"
    property color backgroundColor: "#ECDFD4"
    property color textColor: "#0C011C"
    property color primaryColor: "#FF9800"
    property color primaryPressedColor: "#8A5300"
    property color primaryHoveredColor: "#b56c00"
    property color notEnablePrimaryColor: "#F0B762"
    property color notEnableTextColor: "#453957"
    property color textOnPrimaryColor: "#1C1001"
    property color focusOnPrimaryColor: "#E3C59A"
    property color focusColor: "#FFA000"

    DropShadow {
        anchors.fill: backgroundFormLoginRegister
        horizontalOffset: 0
        verticalOffset: 0
        radius: backgroundLoginRegister.cornerRadius * 2
        color: "#aa000000"
        source: backgroundFormLoginRegister
    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        property point dragPosition

        onPressed: {
            dragPosition = Qt.point(mouseX, mouseY)
        }

        onPositionChanged: {
            if (dragArea.pressed) {
                mainWindow.x += (mouseX - dragPosition.x)
                mainWindow.y += (mouseY - dragPosition.y)
            }
        }
    }

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

            BorderedButton {
                id: backgroundRegisterButton
                text: qsTr("Register")
                width: backgroundLoginRegister.width / 5
                height: 35
                anchors.centerIn: parent

                normalColor: backgroundLoginRegister.registerLoginSwapButtonBorderColor
                normalBackgroundColor: backgroundLoginRegister.registerLoginSwapButtonFillColor
                hoveredColor: backgroundLoginRegister.registerLoginSwapButtonBorderColor
                hoveredBackgroundColor: backgroundLoginRegister.registerLoginSwapButtonFillHoveredColor
                pressedColor: backgroundLoginRegister.registerLoginSwapButtonBorderColor
                pressedBackgroundColor: backgroundLoginRegister.registerLoginSwapButtonFillPressedColor

                onClicked: backgroundFormLoginRegister.state = "registerState"

                Connections {
                    target: backgroundRegisterButton

                    function onClicked() {
                        backgroundLoginRegister.state = "registerState"
                        formConfirmButton.text = formHeadline.text = qsTr("Register")
                        login_register_manager.StateChange(backgroundLoginRegister.state)
                        backgroundLoginRegister.canConfirmForm =
                            login_register_manager.CheckFields(formUsernameField.text,
                                formPasswordField.text,
                                formPasswordAgain.text)

                        backgroundLoginRegister.passwordProgress =
                            login_register_manager.PasswordStrength(formPasswordField.text)
                    }
                }

            }
        }
        Rectangle {
            id: backgroundLogin
            height: backgroundLoginRegister.height
            width: backgroundLoginRegister.width / 2 - 30
            color: "transparent"
            z: 5

            BorderedButton {
                id: backgroundLoginButton
                text: qsTr("Login")
                width: backgroundLoginRegister.width / 5
                height: 35
                anchors.centerIn: parent

                normalColor: backgroundLoginRegister.registerLoginSwapButtonBorderColor
                normalBackgroundColor: backgroundLoginRegister.registerLoginSwapButtonFillColor
                hoveredColor: backgroundLoginRegister.registerLoginSwapButtonBorderColor
                hoveredBackgroundColor: backgroundLoginRegister.registerLoginSwapButtonFillHoveredColor
                pressedColor: backgroundLoginRegister.registerLoginSwapButtonBorderColor
                pressedBackgroundColor: backgroundLoginRegister.registerLoginSwapButtonFillPressedColor

                onClicked: backgroundFormLoginRegister.state = "loginState"

                Connections {
                    target: backgroundLoginButton

                    function onClicked() {
                        backgroundLoginRegister.state = "loginState"
                        formConfirmButton.text = formHeadline.text = qsTr("Login")
                        login_register_manager.StateChange(backgroundLoginRegister.state)
                        backgroundLoginRegister.canConfirmForm =
                            login_register_manager.CheckFields(formUsernameField.text,
                                formPasswordField.text,
                                formPasswordAgain.text)
                    }
                }

            }
        }
    }
    Rectangle {
        id: backgroundFormLoginRegister
        anchors.centerIn: parent
        width: (backgroundLoginRegister.width - parent.sideMargin * 2) / 2
        height: backgroundLoginRegister.height * 1.2
        color: backgroundLoginRegister.backgroundColor
        anchors.horizontalCenterOffset: width / 2
        radius: backgroundLoginRegister.cornerRadius
        clip: true

        MouseArea {
            id: dragArea2
            anchors.fill: parent
            property point dragPosition

            onPressed: {
                dragPosition = Qt.point(mouseX, mouseY)
            }

            onPositionChanged: {
                if (dragArea2.pressed) {
                    mainWindow.x += (mouseX - dragPosition.x)
                    mainWindow.y += (mouseY - dragPosition.y)
                }
            }
        }

        CloseButton {
            anchors.top: parent.top
            anchors.right: parent.right
            height: 32
            width: 32
            radius: backgroundLoginRegister.cornerRadius - 1
            visibleRightUpRadius: true
        }

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
                    text: qsTr("Login")
                    width: formColumn.formWidth
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    bottomPadding: 8
                    color: backgroundLoginRegister.textColor
                    font.pointSize: 14
                }

                ModernTextField {
                    id: formUsernameField
                    width: formColumn.formWidth
                    placeholderText: qsTr("Username")
                    textColor: backgroundLoginRegister.textColor
                    placeholderTextColor: backgroundLoginRegister.textColor
                    fillUnderlineColor: backgroundLoginRegister.textColor
                    selectionTextColor: backgroundLoginRegister.textColor
                    selectionBlockColor: backgroundLoginRegister.focusColor
                    color: "transparent"
                    onTextChanged: {
                        backgroundLoginRegister.canConfirmForm =
                            login_register_manager.CheckFields(formUsernameField.text,
                                formPasswordField.text,
                                formPasswordAgain.text)
                    }
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
                    placeholderText: qsTr("Password")
                    passwordMode: true
                    textColor: backgroundLoginRegister.textColor
                    placeholderTextColor: backgroundLoginRegister.textColor
                    fillUnderlineColor: backgroundLoginRegister.textColor
                    selectionTextColor: backgroundLoginRegister.textColor
                    selectionBlockColor: backgroundLoginRegister.focusColor
                    color: "transparent"
                    Connections {
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
                    }
                }

                Rectangle {
                    id: formBeforePasswordProgressBarSpace
                    height: 0
                    width: formColumn.formWidth
                    color: "transparent"
                }

                MyProgressBar {
                    id: passwordProgressBar
                    height: 0.01
                    width: formColumn.formWidth
                    value: backgroundLoginRegister.passwordProgress
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
                    placeholderText: qsTr("Password again")
                    passwordMode: true
                    textColor: backgroundLoginRegister.textColor
                    placeholderTextColor: backgroundLoginRegister.textColor
                    fillUnderlineColor: backgroundLoginRegister.textColor
                    selectionTextColor: backgroundLoginRegister.textColor
                    selectionBlockColor: backgroundLoginRegister.focusColor
                    color: "transparent"
                    onTextChanged: {
                        backgroundLoginRegister.canConfirmForm =
                            login_register_manager.CheckFields(formUsernameField.text,
                                formPasswordField.text,
                                formPasswordAgain.text)
                    }
                }

                Rectangle {
                    id: formAfterPasswordAgainSpace
                    height: 0
                    width: formColumn.formWidth
                    color: "transparent"
                }

                FlatButton {
                    id: formConfirmButton
                    width: formColumn.formWidth
                    text: qsTr("Login")
                    height: 40
                    primaryColor: backgroundLoginRegister.primaryColor
                    primaryPressedColor: backgroundLoginRegister.primaryPressedColor
                    primaryHoveredColor: backgroundLoginRegister.primaryHoveredColor
                    notEnablePrimaryColor: backgroundLoginRegister.notEnablePrimaryColor
                    textColorOnPrimyry: backgroundLoginRegister.textOnPrimaryColor
                    notEnableTextColor: backgroundLoginRegister.notEnableTextColor
                    textOnPrimaryColor: backgroundLoginRegister.textOnPrimaryColor
                    focusOnPrimaryColor: backgroundLoginRegister.focusOnPrimaryColor
                    enabled: backgroundLoginRegister.canConfirmForm
                    onClicked: login_register_manager.ConfirmFormUser(formUsernameField.text, formPasswordField.text)
                }
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
                target: backgroundFormLoginRegister
                anchors.horizontalCenterOffset: backgroundFormLoginRegister.width / 2
            }

            PropertyChanges {
                target: passwordProgressBar
                height: 0
            }
            PropertyChanges {
                target: formBeforePasswordProgressBarSpace
                height: 0
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
                height: formPasswordAgain.text.length === 0 ? 22 : 46
            }

            PropertyChanges {
                target: backgroundFormLoginRegister
                anchors.horizontalCenterOffset: -1 * (backgroundFormLoginRegister.width / 2)
            }

            PropertyChanges {
                target: passwordProgressBar
                height: 0
            }
            PropertyChanges {
                target: formBeforePasswordProgressBarSpace
                height: 0
            }
        },
        State {
            name: "registerStateProgress"
            PropertyChanges {
                target: formAfterPasswordAgainSpace
                height: 12
            }

            PropertyChanges {
                target: passwordProgressBar
                height: 2
            }
            PropertyChanges {
                target: formBeforePasswordProgressBarSpace
                height: 2
            }

            PropertyChanges {
                target: formPasswordAgain
                height: formPasswordAgain.text.length === 0 ? 22 : 46
            }

            PropertyChanges {
                target: backgroundFormLoginRegister
                anchors.horizontalCenterOffset: -1 * (backgroundFormLoginRegister.width / 2)
            }
        }
    ]
    transitions: [
        Transition {
            id: loginToRegisterTransition
            from: "loginState,registerState"
            to: "loginState,registerState"
            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 25
                    }
                    PropertyAnimation {
                        target: formPasswordAgain
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 100
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 25
                    }
                    PropertyAnimation {
                        target: formAfterPasswordAgainSpace
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 100
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }
                    PropertyAnimation {
                        target:backgroundFormLoginRegister
                        property: "anchors.horizontalCenterOffset"
                        easing.bezierCurve: [0.0553,0.0385,0.0796,-0.0381,0.109,-0.0381,0.138,-0.0381,0.185,0.0276,0.222,0.00353,0.259,-0.0205,0.332,-0.25,0.385,-0.252,0.462,-0.14,0.543,1.18,0.617,1.24,0.676,1.24,0.738,1,0.778,0.977,0.817,0.953,0.855,1.02,0.889,1.03,0.924,1.03,0.951,0.964,1,1]
                        duration: 750
                    }
                }
            }
        },
        Transition {
            id: registrationToProgress
            from: "registerState,registerStateProgress"
            to: "registerState,registerStateProgress"
            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 10
                    }
                    PropertyAnimation {
                        target: passwordProgressBar
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 90
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 10
                    }
                    PropertyAnimation {
                        target: formBeforePasswordProgressBarSpace
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 90
                    }
                }
            }
        },
        Transition {
            id: progressToLogin
            ParallelAnimation {
                SequentialAnimation {
                    PauseAnimation {
                        duration: 10
                    }
                    PropertyAnimation {
                        target: formPasswordAgain
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 90
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }
                    PropertyAnimation {
                        target: backgroundFormLoginRegister
                        property: "anchors.horizontalCenterOffset"
                        easing.bezierCurve: [0.0553,0.0385,0.0796,-0.0381,0.109,-0.0381,0.138,-0.0381,0.185,0.0276,0.222,0.00353,0.259,-0.0205,0.332,-0.25,0.385,-0.252,0.462,-0.14,0.543,1.18,0.617,1.24,0.676,1.24,0.738,1,0.778,0.977,0.817,0.953,0.855,1.02,0.889,1.03,0.924,1.03,0.951,0.964,1,1]
                        duration: 750
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 10
                    }
                    PropertyAnimation {
                        target: formBeforePasswordProgressBarSpace
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 90
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }
                    PropertyAnimation {
                        target: passwordProgressBar
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 100
                    }
                }
                SequentialAnimation {
                    PauseAnimation {
                        duration: 50
                    }
                    PropertyAnimation {
                        target: formAfterPasswordAgainSpace
                        property: "height"
                        easing.bezierCurve: [0.175,0.885,0.32,1.27,1,1]
                        duration: 100
                    }
                }
            }
            to: "loginState"
            from: "registerStateProgress"
        }

    ]
}



