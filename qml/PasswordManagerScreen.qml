import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

Rectangle {
    id: passwordManagerScreen

    property int cornerRadius: 5

    property color primaryColor: "#FF9800"
    property color primaryPressedColor: "#8A5300"
    property color primaryHoveredColor: "#b56c00"
    property color notEnablePrimaryColor: "#F0B762"
    property color textColor: "#0C011C"
    property color textColorOnPrimyry: "#150133"
    property color notEnableTextColor: "#695981"
    property color focusColor: "#FFA000"
    property color textOnPrimaryColor: "#1C1001"
    property color focusOnPrimaryColor: "#E3C59A"
    property color backgroundColor: "#ECDFD4"
    property color drawerBackgroundColor: "#FAF6F2"
    property color borderButtonColor: "#1C0F01"
    property color borderButtonFocusColor: "#FFA000"
    property color notEnabledBorderButtonColor: "#75614B"
    property color borderButtonHoveredColor: "#78562C"
    property color borderButtonPressedColor: "#9b703b"
    property bool firstLoadOfLang: true

    color: passwordManagerScreen.backgroundColor

    focus: true
    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_S) {
            event.accepted = true;
            if (rightDrawer.opened) {
                rightDrawer.close();
            } else {
                rightDrawer.open();
            }
        }
        if (event.key === Qt.Key_A || (event.key === Qt.Key_N && event.modifiers & Qt.ControlModifier)) {
            event.accepted = true;
            if (bottomDrawer.opened) {
                bottomDrawer.close();
            } else {
                bottomDrawer.open();
            }
        }
    }

    IconButton {
        id: addButton
        anchors.rightMargin: 12
        anchors.bottomMargin: 12
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: 32
        addToWidth: 25
        imageSize: 25
        textColor: passwordManagerScreen.textColor
        focusColor: passwordManagerScreen.focusColor
        inactiveColor: passwordManagerScreen.notEnabledBorderButtonColor
        pressedColor: passwordManagerScreen.borderButtonPressedColor
        hoveredColor: passwordManagerScreen.borderButtonHoveredColor
        onClicked: bottomDrawer.open()
        image: "qrc:/images/add.svg"
        spacing: 5
        radius: 5
        text: qsTr("Add record", "", mainWindow.languageVersion)
    }

    IconButton {
        id: settingsButton
        anchors.rightMargin: 12
        anchors.topMargin: 12
        anchors.top: parent.top
        anchors.right: parent.right
        height: 28
        addToWidth: 20
        textColor: passwordManagerScreen.textColor
        focusColor: passwordManagerScreen.focusColor
        inactiveColor: passwordManagerScreen.notEnabledBorderButtonColor
        pressedColor: passwordManagerScreen.borderButtonPressedColor
        hoveredColor: passwordManagerScreen.borderButtonHoveredColor
        onClicked: rightDrawer.open()
        image: "qrc:/images/setting.svg"
        spacing: 5
        radius: 5
        text: qsTr("Settings", "", mainWindow.languageVersion)
    }

    Drawer {
        id: bottomDrawer
        width: parent.width
        height: parent.height / 5 * 4
        edge: Qt.BottomEdge

        onOpenedChanged: {
            if (bottomDrawer.opened) {
                overlay.visible = true
            } else {
                overlay.visible = false
            }
        }
        focus: true
        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Tab) {
                event.accepted = true;
                formHeadlineField.focus = true
            }
            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                event.accepted = true;
                formConfirmButton.focus = true;
            }
        }

        background: Rectangle {
            anchors.fill: parent
            color: passwordManagerScreen.drawerBackgroundColor
        }

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
                height: (parent.height - formHeadlineField.height - formUsernameField.height - formPasswordField.height - formConfirmButton.height - formAfterPasswordSpace.height - headlineText.height - headllineSeparator.height - 8 * formColumn.spacing) / 2
                color: "transparent"
            }
            Text {
                id: headlineText
                color: passwordManagerScreen.textColor
                text: qsTr("Create record", "", mainWindow.languageVersion)
            }
            Rectangle {
                id: headllineSeparator
                color: passwordManagerScreen.textColor
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
                        placeholderText: qsTr("Headline", "", mainWindow.languageVersion)
                        textColor: passwordManagerScreen.textColor
                        placeholderTextColor: passwordManagerScreen.textColor
                        fillUnderlineColor: passwordManagerScreen.textColor
                        selectionTextColor: passwordManagerScreen.textColor
                        selectionBlockColor: passwordManagerScreen.focusColor
                        color: "transparent"
                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                formUsernameField.focus = true
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                bottomDrawer.focus = true;
                            }
                        }
                    }

                    ModernTextField {
                        id: formUsernameField
                        width: formColumn.formWidth - formColumn.settingWidth
                        placeholderText: qsTr("Username", "", mainWindow.languageVersion)
                        placeholderTextColor: passwordManagerScreen.textColor
                        fillUnderlineColor: passwordManagerScreen.textColor
                        textColor: passwordManagerScreen.textColor
                        selectionTextColor: passwordManagerScreen.textColor
                        selectionBlockColor: passwordManagerScreen.focusColor
                        color: "transparent"

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                formPasswordField.focus = true
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                formHeadlineField.focus = true;
                            }
                        }
                    }

                    ModernTextField {
                        id: formPasswordField
                        width: formColumn.formWidth - formColumn.settingWidth
                        placeholderText: qsTr("Password", "", mainWindow.languageVersion)
                        passwordMode: true
                        placeholderTextColor: passwordManagerScreen.textColor
                        fillUnderlineColor: passwordManagerScreen.textColor
                        textColor: passwordManagerScreen.textColor
                        selectionTextColor: passwordManagerScreen.textColor
                        selectionBlockColor: passwordManagerScreen.focusColor
                        color: "transparent"

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                if (formConfirmButton.enabled) {
                                    formConfirmButton.focus = true
                                } else {
                                    majusculeGenPassChBox.focus = true
                                }
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                formUsernameField.focus = true;
                            }
                        }
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
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Upper letters", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        minusculeGenPassChBox.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        formPasswordField.focus = true;
                                    }
                                }
                            }
                            CustomCheckBox {
                                id: minusculeGenPassChBox
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Lower letters", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        numberGenPassChBox.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        majusculeGenPassChBox.focus = true;
                                    }
                                }
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
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Numbers", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        specialGenPassChBox.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        minusculeGenPassChBox.focus = true;
                                    }
                                }
                            }
                            CustomCheckBox {
                                id: specialGenPassChBox
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Special chars", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        passGenLength.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        numberGenPassChBox.focus = true;
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        height: 10
                        width: formColumn.settingWidth
                        color: "transparent"
                    }

                    SizePicker {
                        id: passGenLength
                        width: formColumn.settingWidth
                        to: 64
                        from: 8
                        value: 12
                        stepSize: 1
                        textColor: passwordManagerScreen.textColor
                        backgroundColor: passwordManagerScreen.backgroundColor
                        primaryHexColor: passwordManagerScreen.primaryColor

                        text: qsTr("Password length", "", mainWindow.languageVersion)

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                if (buttonGenPass.enabled) {
                                    buttonGenPass.focus = true
                                } else if (formConfirmButton.enabled) {
                                    formConfirmButton.focus = true
                                } else if (formHeadlineField.text === "") {
                                    formHeadlineField.focus = true
                                } else if (formUsernameField.text === "") {
                                    formUsernameField.focus = true
                                } else {
                                    formPasswordField.focus = true
                                }

                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                specialGenPassChBox.focus = true;
                            }
                        }
                    }

                    Rectangle {
                        height: 10
                        width: formColumn.settingWidth
                        color: "transparent"
                    }


                    FlatButton {
                        id: buttonGenPass
                        width: formColumn.settingWidth - 60
                        height: 25
                        primaryColor: passwordManagerScreen.primaryColor
                        primaryPressedColor: passwordManagerScreen.primaryPressedColor
                        primaryHoveredColor: passwordManagerScreen.primaryHoveredColor
                        notEnablePrimaryColor: passwordManagerScreen.notEnablePrimaryColor
                        textColorOnPrimyry: passwordManagerScreen.textColorOnPrimyry
                        notEnableTextColor: passwordManagerScreen.notEnableTextColor
                        textOnPrimaryColor: passwordManagerScreen.textOnPrimaryColor
                        focusOnPrimaryColor: passwordManagerScreen.focusOnPrimaryColor
                        enabled: majusculeGenPassChBox.checked || minusculeGenPassChBox.checked || numberGenPassChBox.checked || specialGenPassChBox.checked
                        text: qsTr("Generate password", "", mainWindow.languageVersion)
                        anchors.horizontalCenter: parent.horizontalCenter
                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                if (formConfirmButton.enabled) {
                                    formConfirmButton.focus = true
                                } else if (formHeadlineField.text === "") {
                                    formHeadlineField.focus = true
                                } else if (formUsernameField.text === "") {
                                    formUsernameField.focus = true
                                } else {
                                    formPasswordField.focus = true
                                }
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                passGenLength.focus = true;
                            }
                            if (event.key === Qt.Key_Enter) {
                                event.accepted = true;
                                generate_password.ClickToGenerateButton(majusculeGenPassChBox.checked, minusculeGenPassChBox.checked, numberGenPassChBox.checked, specialGenPassChBox.checked, passGenLength.value)
                            }
                            if (event.key === Qt.Key_S || event.key === Qt.Key_H) {
                                event.accepted = true
                                formPasswordField.passwordIsHidden = !formPasswordField.passwordIsHidden
                            }
                        }
                        onClicked: generate_password.ClickToGenerateButton(majusculeGenPassChBox.checked, minusculeGenPassChBox.checked, numberGenPassChBox.checked, specialGenPassChBox.checked, passGenLength.value)
                    }
                }
            }

            Rectangle {
                id: formAfterPasswordSpace
                height: 12
                width: formColumn.formWidth
                color: "transparent"
            }

            FlatButton {
                id: formConfirmButton
                width: formColumn.formWidth
                primaryColor: passwordManagerScreen.primaryColor
                primaryPressedColor: passwordManagerScreen.primaryPressedColor
                primaryHoveredColor: passwordManagerScreen.primaryHoveredColor
                notEnablePrimaryColor: passwordManagerScreen.notEnablePrimaryColor
                textColorOnPrimyry: passwordManagerScreen.textColorOnPrimyry
                notEnableTextColor: passwordManagerScreen.notEnableTextColor
                textOnPrimaryColor: passwordManagerScreen.textOnPrimaryColor
                focusOnPrimaryColor: passwordManagerScreen.focusOnPrimaryColor
                text: qsTr("Save record", "", mainWindow.languageVersion)
                height: 40
                enabled: formHeadlineField.text !== "" && formUsernameField.text !== "" && formPasswordField.text !== ""

                onClicked: {
                    login_register_manager.recordListModel.addRecordItem(formHeadlineField.text, formUsernameField.text, formPasswordField.text, iterations.value)
                    bottomDrawer.close();
                    formHeadlineField.text = "";
                    formUsernameField.text = "";
                    formPasswordField.text = "";
                }

                Keys.onPressed: (event) => {
                    if (event.key === Qt.Key_Enter) {
                        login_register_manager.recordListModel.addRecordItem(formHeadlineField.text,formUsernameField.text,formPasswordField.text, iterations.value)

                        bottomDrawer.close();
                        formHeadlineField.text = "";
                        formUsernameField.text = "";
                        formPasswordField.text = "";
                    }
                    if (event.key === Qt.Key_Tab) {
                        event.accepted = true;
                        bottomDrawer.focus = true;
                    }
                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                        event.accepted = true;
                        passGenLength.focus = true;
                    }
                }
            }
        }
    }

    Drawer {
        id: editDrawer
        width: parent.width
        height: parent.height / 5 * 4
        edge: Qt.TopEdge

        property int index: -1;

        onOpenedChanged: {
            if (bottomDrawer.opened) {
                overlay.visible = true
            } else {
                overlay.visible = false
            }
        }
        focus: true
        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Tab) {
                event.accepted = true;
                editFormHeadlineField.focus = true
            }
            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                event.accepted = true;
                editFormConfirmButton.focus = true;
            }
        }

        background: Rectangle {
            anchors.fill: parent
            color: passwordManagerScreen.drawerBackgroundColor
        }

        Column {
            id: editFormColumn
            anchors.centerIn: parent
            spacing: 5
            anchors.fill: parent
            leftPadding: (parent.width - formWidth) / 2
            rightPadding: (parent.width - formWidth) / 2

            property int formWidth: formColumn.width - parent.width / 5 * 2
            property int settingWidth: 40 + Math.max(majusculeGenPassChBox.width, minusculeGenPassChBox.width) + Math.max(numberGenPassChBox.width, specialGenPassChBox.width)

            Rectangle {
                width: editFormColumn.formWidth
                height: (editFormColumn.height - (Math.max(editSettingsGenPass.height, editFormHeadlineField.height + editFormUsernameField.height + editFormPasswordField.height + editOldPasswordText.height + oldPassSpace2.height + oldPassSpace1.height + editOldPasswordHeadline.height) + editFormConfirmButton.height)) / 3
                color: "transparent"
            }
            Text {
                id: editHeadlineText
                color: passwordManagerScreen.textColor
                text: qsTr("Edit record", "", mainWindow.languageVersion)
            }
            Rectangle {
                id: editHeadllineSeparator
                color: passwordManagerScreen.notEnableTextColor
                width: formColumn.formWidth
                height: 1
            }
            Row {
                width: formColumn.formWidth
                height: Math.max(editSettingsGenPass.height, editFormHeadlineField.height + editFormUsernameField.height + editFormPasswordField.height + editOldPasswordText.height + oldPassSpace2.height + oldPassSpace1.height+ editOldPasswordHeadline.height)
                Column {

                    ModernTextField {
                        id: editFormHeadlineField
                        width: editFormColumn.formWidth - editFormColumn.settingWidth
                        placeholderText: qsTr("Headline", "", mainWindow.languageVersion)
                        placeholderTextColor: passwordManagerScreen.textColor
                        fillUnderlineColor: passwordManagerScreen.textColor
                        textColor: passwordManagerScreen.textColor
                        selectionTextColor: passwordManagerScreen.textColor
                        selectionBlockColor: passwordManagerScreen.focusColor
                        color: "transparent"
                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                editFormUsernameField.focus = true
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                editFottomDrawer.focus = true;
                            }
                        }
                    }

                    ModernTextField {
                        id: editFormUsernameField
                        width: editFormColumn.formWidth - editFormColumn.settingWidth
                        placeholderText: qsTr("Username", "", mainWindow.languageVersion)
                        placeholderTextColor: passwordManagerScreen.textColor
                        fillUnderlineColor: passwordManagerScreen.textColor
                        textColor: passwordManagerScreen.textColor
                        selectionTextColor: passwordManagerScreen.textColor
                        selectionBlockColor: passwordManagerScreen.focusColor
                        color: "transparent"

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                editFormPasswordField.focus = true
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                editFormHeadlineField.focus = true;
                            }
                        }
                    }

                    Rectangle {
                        id: oldPassSpace1
                        color: "transparent"
                        height: 5
                        width: editFormColumn.formWidth - editFormColumn.settingWidth
                    }

                    Text {
                        id: editOldPasswordHeadline
                        text: qsTr("Old password", "", mainWindow.languageVersion)
                        font.pointSize: 8
                        color: passwordManagerScreen.notEnableTextColor
                    }

                    TextArea {
                        id: editOldPasswordText
                        color: passwordManagerScreen.notEnableTextColor
                        selectByMouse: true
                        readOnly: true
                    }
                    Rectangle {
                        color: passwordManagerScreen.notEnableTextColor
                        height: 1
                        width: editFormColumn.formWidth - editFormColumn.settingWidth
                    }

                    Rectangle {
                        id: oldPassSpace2
                        color: "transparent"
                        height: 5
                        width: editFormColumn.formWidth - editFormColumn.settingWidth
                    }

                    ModernTextField {
                        id: editFormPasswordField
                        width: editFormColumn.formWidth - editFormColumn.settingWidth
                        placeholderText: qsTr("Password", "", mainWindow.languageVersion)
                        passwordMode: true
                        placeholderTextColor: passwordManagerScreen.textColor
                        fillUnderlineColor: passwordManagerScreen.textColor
                        textColor: passwordManagerScreen.textColor
                        selectionTextColor: passwordManagerScreen.textColor
                        selectionBlockColor: passwordManagerScreen.focusColor
                        color: "transparent"

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                if (editFormConfirmButton.enabled) {
                                    editFormConfirmButton.focus = true
                                } else {
                                    editMajusculeGenPassChBox.focus = true
                                }
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                editFormUsernameField.focus = true;
                            }
                        }
                    }
                }

                Rectangle {
                    height: parent.height
                    width: 10
                    color: "transparent"
                }

                Column {
                    id: editSettingsGenPass
                    width: editFormColumn.settingWidth
                    Row {
                        Column {
                            width: Math.max(majusculeGenPassChBox.width, minusculeGenPassChBox.width)
                            CustomCheckBox {
                                id: editMajusculeGenPassChBox
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Upper letters", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        editMinusculeGenPassChBox.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        editFormPasswordField.focus = true;
                                    }
                                }
                            }
                            CustomCheckBox {
                                id: editMinusculeGenPassChBox
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Lower letters", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        editNumberGenPassChBox.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        editMajusculeGenPassChBox.focus = true;
                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: 30
                            height: editMajusculeGenPassChBox.height + editNumberGenPassChBox.height
                            color: "transparent"
                        }
                        Column {
                            width: Math.max(editNumberGenPassChBox.width, editSpecialGenPassChBox.width)
                            CustomCheckBox {
                                id: editNumberGenPassChBox
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Numbers", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        editSpecialGenPassChBox.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        editMinusculeGenPassChBox.focus = true;
                                    }
                                }
                            }
                            CustomCheckBox {
                                id: editSpecialGenPassChBox
                                borderColor: passwordManagerScreen.textColor
                                checkedColor: passwordManagerScreen.primaryColor
                                backgroundColor: passwordManagerScreen.drawerBackgroundColor
                                text: qsTr("Special chars", "", mainWindow.languageVersion)
                                Keys.onPressed: (event) => {
                                    if (event.key === Qt.Key_Tab) {
                                        event.accepted = true;
                                        editPassGenLength.focus = true
                                    }
                                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                        event.accepted = true;
                                        editNumberGenPassChBox.focus = true;
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        height: 10
                        width: editFormColumn.settingWidth
                        color: "transparent"
                    }

                    SizePicker {
                        id: editPassGenLength
                        width: editFormColumn.settingWidth
                        to: 64
                        from: 8
                        value: 12
                        stepSize: 1
                        textColor: passwordManagerScreen.textColor
                        backgroundColor: passwordManagerScreen.backgroundColor
                        primaryHexColor: passwordManagerScreen.primaryColor

                        text: qsTr("Password length", "", mainWindow.languageVersion)

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                if (editButtonGenPass.enabled) {
                                    editButtonGenPass.focus = true
                                } else if (editFormConfirmButton.enabled) {
                                    editFormConfirmButton.focus = true
                                } else if (editFormHeadlineField.text === "") {
                                    editFormHeadlineField.focus = true
                                } else if (editFormUsernameField.text === "") {
                                    editFormUsernameField.focus = true
                                } else {
                                    editFormPasswordField.focus = true
                                }

                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                editSpecialGenPassChBox.focus = true;
                            }
                        }
                    }

                    Rectangle {
                        height: 10
                        width: editFormColumn.settingWidth
                        color: "transparent"
                    }


                    FlatButton {
                        id: editButtonGenPass
                        primaryColor: passwordManagerScreen.primaryColor
                        primaryPressedColor: passwordManagerScreen.primaryPressedColor
                        primaryHoveredColor: passwordManagerScreen.primaryHoveredColor
                        notEnablePrimaryColor: passwordManagerScreen.notEnablePrimaryColor
                        textColorOnPrimyry: passwordManagerScreen.textColorOnPrimyry
                        notEnableTextColor: passwordManagerScreen.notEnableTextColor
                        textOnPrimaryColor: passwordManagerScreen.textOnPrimaryColor
                        focusOnPrimaryColor: passwordManagerScreen.focusOnPrimaryColor
                        width: editFormColumn.settingWidth - 60
                        height: 25
                        enabled: editMajusculeGenPassChBox.checked || editMinusculeGenPassChBox.checked || editNumberGenPassChBox.checked || editSpecialGenPassChBox.checked
                        text: qsTr("Generate password", "", mainWindow.languageVersion)
                        anchors.horizontalCenter: parent.horizontalCenter

                        Keys.onPressed: (event) => {
                            if (event.key === Qt.Key_Tab) {
                                event.accepted = true;
                                if (editFormConfirmButton.enabled) {
                                    editFormConfirmButton.focus = true
                                } else if (editFormHeadlineField.text === "") {
                                    editFormHeadlineField.focus = true
                                } else if (editFormUsernameField.text === "") {
                                    editFormUsernameField.focus = true
                                } else {
                                    editFormPasswordField.focus = true
                                }
                            }
                            if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                                event.accepted = true;
                                editPassGenLength.focus = true;
                            }
                            if (event.key === Qt.Key_Enter) {
                                event.accepted = true;
                                generate_password.ClickToGenerateButton(editMajusculeGenPassChBox.checked, editMinusculeGenPassChBox.checked, editNumberGenPassChBox.checked, editSpecialGenPassChBox.checked, editPassGenLength.value)
                            }
                            if (event.key === Qt.Key_S || event.key === Qt.Key_H) {
                                event.accepted = true
                                formPasswordField.passwordIsHidden = !formPasswordField.passwordIsHidden
                            }
                        }

                        onClicked: generate_password.ClickToGenerateButton(editMajusculeGenPassChBox.checked, editMinusculeGenPassChBox.checked, editNumberGenPassChBox.checked, editSpecialGenPassChBox.checked, editPassGenLength.value)

                    }
                }
            }

            Rectangle {
                id: editFormAfterPasswordSpace
                height: 12
                width: editFormColumn.formWidth
                color: "transparent"
            }

            FlatButton {
                id: editFormConfirmButton
                primaryColor: passwordManagerScreen.primaryColor
                primaryPressedColor: passwordManagerScreen.primaryPressedColor
                primaryHoveredColor: passwordManagerScreen.primaryHoveredColor
                notEnablePrimaryColor: passwordManagerScreen.notEnablePrimaryColor
                textColorOnPrimyry: passwordManagerScreen.textColorOnPrimyry
                notEnableTextColor: passwordManagerScreen.notEnableTextColor
                textOnPrimaryColor: passwordManagerScreen.textOnPrimaryColor
                focusOnPrimaryColor: passwordManagerScreen.focusOnPrimaryColor
                width: editFormColumn.formWidth
                text: qsTr("Save changes", "", mainWindow.languageVersion)
                height: 40
                enabled: editFormHeadlineField.text !== "" && editFormUsernameField.text !== "" && editFormPasswordField.text !== ""

                onClicked: {
                    login_register_manager.recordListModel.editRecordItem(editDrawer.index, editFormHeadlineField.text, editFormUsernameField.text, editFormPasswordField.text, iterations.value)
                    editDrawer.close()
                }

                Keys.onPressed: (event) => {
                    if (event.key === Qt.Key_Tab) {
                        event.accepted = true;
                        editEditDrawer.focus = true;
                    }
                    if (event.key === Qt.Key_Tab && event.modifiers & Qt.ShiftModifier) {
                        event.accepted = true;
                        editPassGenLength.focus = true;
                    }
                }
            }
        }
    }

    ListView {
        id: gridViewPasswordsRecords
        width: 500
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        model: login_register_manager.recordListModel
        anchors.horizontalCenter: parent.horizontalCenter

        delegate: PasswordRecordListItem {
            id: passwordRecordListItem
            width: 500
            height: 110
            headlineText: model.headlineText
            usernameText: model.usernameText
            passwordText: model.passwordText
            backgroundColor: passwordManagerScreen.backgroundColor
            textColor: passwordManagerScreen.textColor
            focusColor: passwordManagerScreen.focusColor
            borderButtonColor: passwordManagerScreen.borderButtonColor
            borderButtonFocusColor: passwordManagerScreen.borderButtonFocusColor
            notEnabledBorderButtonColor: passwordManagerScreen.notEnabledBorderButtonColor
            borderButtonHoveredColor: passwordManagerScreen.borderButtonHoveredColor
            borderButtonPressedColor: passwordManagerScreen.borderButtonPressedColor
            lineColor: passwordManagerScreen.notEnableTextColor

            Connections {
                target: passwordRecordListItem

                function onRequestDeletion(index) {
                    login_register_manager.recordListModel.removeRecordItem(index, iterations.value)
                }

                function onRequestMoveUp(index) {
                    login_register_manager.recordListModel.moveRecordItem(index, index - 1, iterations.value)
                }

                function onRequestMoveDown(index) {
                    login_register_manager.recordListModel.moveRecordItem(index, index + 1, iterations.value)
                }

                function onRequestEdit(index, headline, username, password) {
                    editFormHeadlineField.text = headline
                    editFormUsernameField.text = username
                    editFormPasswordField.text = password
                    editOldPasswordText.text = password
                    editDrawer.open()
                    editDrawer.index = index
                }
            }
        }
    }

    Drawer {
        id: rightDrawer
        width: 305
        height: parent.height
        edge: Qt.RightEdge
        onOpenedChanged: {
            if (rightDrawer.opened) {
                overlay.visible = true
            } else {
                overlay.visible = false
            }
        }

        background: Rectangle {
            anchors.fill: parent
            color: passwordManagerScreen.drawerBackgroundColor
        }

        RowLayout {

            spacing: 0
            anchors.fill: parent
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            Rectangle {
                id: settingsLeftSpace
                color: "transparent"
                Layout.preferredWidth: 10
                Layout.fillHeight: true

            }
            ColumnLayout {
                spacing: 20
                SizePicker {
                    id: iterations
                    from: 3
                    to: 30
                    stepSize: 3
                    text: qsTr("Encrypting iterations", "", mainWindow.languageVersion)
                    Layout.fillWidth: true
                    Layout.preferredWidth: rightDrawer.width - settingsLeftSpace.width - settingsRightSpace.width
                    value: login_register_manager.iterations
                    textColor: passwordManagerScreen.textColor
                    backgroundColor: passwordManagerScreen.backgroundColor
                    primaryHexColor: passwordManagerScreen.primaryColor
                    onValueChanged: {
                        login_register_manager.IterationsChanged(iterations.value);
                    }

                }
                CustomComboBox{
                    id: themeSelect
                    model: appValidThemes
                    radius: passwordManagerScreen.cornerRadius
                    implicitHeight: 30
                    Layout.fillWidth: true
                    backgroundColor: passwordManagerScreen.backgroundColor
                    textColor: passwordManagerScreen.textColor
                    primaryColor: passwordManagerScreen.primaryColor
                    textColorOnPrimyry: passwordManagerScreen.textColorOnPrimyry
                    drawerBackgroundColor: passwordManagerScreen.drawerBackgroundColor

                    Component.onCompleted: {
                        var defaultText = appCurrentColorMode;
                        for (var i = 0; i < model.length; ++i) {
                            if (model[i] === defaultText) {
                                themeSelect.currentIndex = i;
                                break;
                            }
                        }
                    }

                    onCurrentIndexChanged: {
                        settings_manager.ChangeCurrentColorMode(model[themeSelect.currentIndex])
                    }
                }

                CustomComboBox{
                    id: languageSelect
                    model: settings_manager.GetLangModel()
                    radius: passwordManagerScreen.cornerRadius
                    implicitHeight: 30
                    Layout.fillWidth: true
                    backgroundColor: passwordManagerScreen.backgroundColor
                    textColor: passwordManagerScreen.textColor
                    primaryColor: passwordManagerScreen.primaryColor
                    textColorOnPrimyry: passwordManagerScreen.textColorOnPrimyry
                    drawerBackgroundColor: passwordManagerScreen.drawerBackgroundColor

                    Component.onCompleted: {
                        var defaultText = settings_manager.GetCurrentLangName();
                        for (var i = 0; i < model.length; ++i) {
                            if (model[i] === defaultText) {
                                languageSelect.currentIndex = i;
                                break;
                            }
                        }
                    }

                    onCurrentIndexChanged: {
                        if (passwordManagerScreen.firstLoadOfLang) {
                            passwordManagerScreen.firstLoadOfLang = false
                        } else {
                            settings_manager.ChangeLang(model[languageSelect.currentIndex])
                        }
                    }
                }
            }
            Rectangle {
                id: settingsRightSpace
                color: "transparent"
                Layout.preferredWidth: 10
                Layout.fillHeight: true

            }
        }
    }

    Rectangle {
        id: overlay
        color: "#77000000"
        anchors.fill: parent
        visible: false
        focus: true
        MouseArea {
            anchors.fill: parent
            onClicked: {
                bottomDrawer.close()
                rightDrawer.close()
            }
        }

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Escape ||
                event.key === Qt.Key_A ||
                (event.key === Qt.Key_N && event.modifiers & Qt.ControlModifier) ||
                event.key === Qt.Key_S) {
                event.accepted = true;
                bottomDrawer.close()
                rightDrawer.close()
            }
        }
    }

    Connections {
        target: generate_password
        function onPasswordGenerated(password) {
            formPasswordField.text = password
            editFormPasswordField.text = password
        }
    }
}

