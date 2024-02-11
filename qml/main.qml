import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Dialogs


ApplicationWindow {
    id: mainWindow
    visible: true
    width: 840
    height: 500

    title: "RainText"
    color: "transparent"
    minimumHeight: 500
    minimumWidth: 800
    property bool hasColorError: false
    flags: Qt.Window | Qt.FramelessWindowHint

    Component.onCompleted: {
        if (settings_manager.hasError) {
            errorDialog.text = settings_manager.errorMessage
            errorDialog.title = settings_manager.errorHeadline
            errorDialog.open()
            mainWindow.hasColorError = true
        } else {
            mainWindow.hasColorError = false
        }
    }

    Connections {
        target: settings_manager
        function errorChanged(settings_manager) {
            if (settings_manager.hasError) {
                errorDialog.text = settings_manager.errorMessage
                errorDialog.title = settings_manager.errorHeadline
                errorDialog.open()
                mainWindow.hasColorError = true
            } else {
                mainWindow.hasColorError = false
            }
        }
    }

    MessageDialog {
        id: errorDialog
        modality: Qt.ApplicationModal
        buttons:  MessageDialog.Ok
        onAccepted: Qt.quit()
    }

    /*DropShadow {
        anchors.fill: passwordManagerScreen
        horizontalOffset: 0
        verticalOffset: 0
        radius: passwordManagerScreen.cornerRadius * 2
        color: "#aa000000"
        source: passwordManagerScreen
    }*/
    /*ColumnLayout {
        width: 200
        anchors.centerIn: parent
        FlatButton {
            Layout.preferredWidth: 200
            enabled: false
        }
        FlatButton {
            focus: true
        }
        FlatButton {
        }
    }*/
    LoginRegisterScreen {
        id: loginRegisterScreen
        visible: true
        loginRegisterBackgroundColor: mainWindow.hasColorError ? "#021F3C" : appColors["loginRegisterBackgroundColor"]
        registerLoginSwapButtonBorderColor: mainWindow.hasColorError ? "#ECDFD4" : appColors["registerLoginSwapButtonBorderColor"]
        registerLoginSwapButtonFillColor: mainWindow.hasColorError ? "#00ECDFD4" : appColors["registerLoginSwapButtonFillColor"]
        registerLoginSwapButtonFillHoveredColor: mainWindow.hasColorError ? "#11D1C7BE" : appColors["registerLoginSwapButtonFillHoveredColor"]
        registerLoginSwapButtonFillPressedColor: mainWindow.hasColorError ? "#33ECDED3" : appColors["registerLoginSwapButtonFillPressedColor"]
        backgroundColor: mainWindow.hasColorError ? "#ECDFD4" : appColors["backgroundColor"]
        textColor: mainWindow.hasColorError ? "#0C011C" : appColors["textColor"]
        primaryColor: mainWindow.hasColorError ? "#FF9800" : appColors["primaryColor"]
        primaryPressedColor: mainWindow.hasColorError ? "#8A5300" : appColors["primaryHoveredColor"]
        primaryHoveredColor: mainWindow.hasColorError ? "#b56c00" : appColors["primaryHoveredColor"]
        notEnablePrimaryColor: mainWindow.hasColorError ? "#F0B762" : appColors["notEnablePrimaryColor"]
        textColorOnPrimyry: mainWindow.hasColorError ? "#150133" : appColors["textColorOnPrimyry"]
        notEnableTextColor: mainWindow.hasColorError ? "#453957" : appColors["notEnableTextColor"]
        textOnPrimaryColor: mainWindow.hasColorError ? "#1C1001" : appColors["textOnPrimaryColor"]
        focusOnPrimaryColor: mainWindow.hasColorError ? "#E3C59A" : appColors["focusOnPrimaryColor"]

    }

    LoadDbScreen {
        id: loadDbScreen
        visible: false
        anchors.fill: parent
        textColor: mainWindow.hasColorError ? "#0C011C" : appColors["textColor"]
        progressBarBackground: mainWindow.hasColorError ? "#c3b9af" : appColors["loadProgressBarBackground"]
        progressBarForeground: mainWindow.hasColorError ? "#FF9800" : appColors["primaryColor"]
        color: mainWindow.hasColorError ? "#FAF6F2" : appColors["drawerBackgroundColor"]
        text: ""
        value: 0.5

        onValueChanged: {
            if (value >= 1) {
                loadDbScreen.visible = false
                passwordManagerScreen. visible = true
            }
        }

    }

    PasswordManagerScreen {
        id: passwordManagerScreen
        visible: false
        anchors.fill: parent
        primaryColor: mainWindow.hasColorError ? "#FF9800" : appColors["primaryColor"]
        primaryPressedColor: mainWindow.hasColorError ? "#8A5300" : appColors["primaryPressedColor"]
        primaryHoveredColor: mainWindow.hasColorError ? "#b56c00" : appColors["primaryHoveredColor"]
        notEnablePrimaryColor: mainWindow.hasColorError ? "#F0B762" : appColors["notEnablePrimaryColor"]
        textColor: mainWindow.hasColorError ? "#0C011C" : appColors["textColor"]
        textOnPrimaryColor: mainWindow.hasColorError ? "#1C1001" : appColors["textOnPrimaryColor"]
        notEnableTextColor: mainWindow.hasColorError ? "#695981" : appColors["notEnableTextColor"]
        focusColor: mainWindow.hasColorError ? "#FFA000" : appColors["focusColor"]
        focusOnPrimaryColor: mainWindow.hasColorError ? "#E3C59A" : appColors["focusOnPrimaryColor"]
        backgroundColor: mainWindow.hasColorError ? "#ECDFD4" : appColors["backgroundColor"]
        drawerBackgroundColor: mainWindow.hasColorError ? "#FAF6F2" : appColors["drawerBackgroundColor"]
        borderButtonColor: mainWindow.hasColorError ? "#1C0F01" : appColors["borderButtonColor"]
        borderButtonFocusColor: mainWindow.hasColorError ? "#FFA000" : appColors["borderButtonFocusColor"]
        notEnabledBorderButtonColor: mainWindow.hasColorError ? "#75614B" : appColors["notEnabledBorderButtonColor"]
        borderButtonHoveredColor: mainWindow.hasColorError ? "#78562C" : appColors["borderButtonHoveredColor"]
        borderButtonPressedColor: mainWindow.hasColorError ? "#78562C" : appColors["borderButtonPressedColor"]
    }

    Connections {
        target: login_register_manager

        function onLoadDb(title) {
            console.log("loadDb")
            loginRegisterScreen.visible = false
            loadDbScreen.visible = true
            mainWindow.color = mainWindow.hasColorError ? "#FAF6F2" : appColors["drawerBackgroundColor"]
            mainWindow.title = title
            mainWindow.flags = Qt.Window | Qt.WindowTitleHint | Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint;
        }
        function onUpdateLoadDbProgress(progress, mes) {
            loadDbScreen.text = mes;
            loadDbScreen.value = progress;
        }
    }
}
