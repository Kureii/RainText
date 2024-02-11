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
    //flags: Qt.Window | Qt.FramelessWindowHint

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
        visible: false
        loginRegisterBackgroundColor: mainWindow.hasColorError ? "#021F3C" : settings_manager.appColors["loginRegisterBackgroundColor"]
        registerLoginSwapButtonBorderColor: mainWindow.hasColorError ? "#ECDFD4" : settings_manager.appColors["registerLoginSwapButtonBorderColor"]
        registerLoginSwapButtonFillColor: mainWindow.hasColorError ? "#00ECDFD4" : settings_manager.appColors["registerLoginSwapButtonFillColor"]
        registerLoginSwapButtonFillHoveredColor: mainWindow.hasColorError ? "#11D1C7BE" : settings_manager.appColors["registerLoginSwapButtonFillHoveredColor"]
        registerLoginSwapButtonFillPressedColor: mainWindow.hasColorError ? "#33ECDED3" : settings_manager.appColors["registerLoginSwapButtonFillPressedColor"]
        backgroundColor: mainWindow.hasColorError ? "#ECDFD4" : settings_manager.appColors["backgroundColor"]
        textColor: mainWindow.hasColorError ? "#0C011C" : settings_manager.appColors["textColor"]
        focusColor: mainWindow.hasColorError ? "#FFA000" : settings_manager.appColors["focusColor"]
        primaryColor: mainWindow.hasColorError ? "#FF9800" : settings_manager.appColors["primaryColor"]
        primaryPressedColor: mainWindow.hasColorError ? "#8A5300" : settings_manager.appColors["primaryHoveredColor"]
        primaryHoveredColor: mainWindow.hasColorError ? "#b56c00" : settings_manager.appColors["primaryHoveredColor"]
        notEnablePrimaryColor: mainWindow.hasColorError ? "#F0B762" : settings_manager.appColors["notEnablePrimaryColor"]
        notEnableTextColor: mainWindow.hasColorError ? "#453957" : settings_manager.appColors["notEnableTextColor"]
        textOnPrimaryColor: mainWindow.hasColorError ? "#1C1001" : settings_manager.appColors["textOnPrimaryColor"]
        focusOnPrimaryColor: mainWindow.hasColorError ? "#E3C59A" : settings_manager.appColors["focusOnPrimaryColor"]

    }

    LoadDbScreen {
        id: loadDbScreen
        visible: false
        anchors.fill: parent
        textColor: mainWindow.hasColorError ? "#0C011C" : settings_manager.appColors["textColor"]
        progressBarBackground: mainWindow.hasColorError ? "#c3b9af" : settings_manager.appColors["loadProgressBarBackground"]
        progressBarForeground: mainWindow.hasColorError ? "#FF9800" : settings_manager.appColors["primaryColor"]
        color: mainWindow.hasColorError ? "#FAF6F2" : settings_manager.appColors["drawerBackgroundColor"]
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
        visible: true
        anchors.fill: parent
        primaryColor: mainWindow.hasColorError ? "#FF9800" : settings_manager.appColors["primaryColor"]
        primaryPressedColor: mainWindow.hasColorError ? "#8A5300" : settings_manager.appColors["primaryPressedColor"]
        primaryHoveredColor: mainWindow.hasColorError ? "#b56c00" : settings_manager.appColors["primaryHoveredColor"]
        notEnablePrimaryColor: mainWindow.hasColorError ? "#F0B762" : settings_manager.appColors["notEnablePrimaryColor"]
        textColor: mainWindow.hasColorError ? "#0C011C" : settings_manager.appColors["textColor"]
        textOnPrimaryColor: mainWindow.hasColorError ? "#1C1001" : settings_manager.appColors["textOnPrimaryColor"]
        notEnableTextColor: mainWindow.hasColorError ? "#695981" : settings_manager.appColors["notEnableTextColor"]
        focusColor: mainWindow.hasColorError ? "#FFA000" : settings_manager.appColors["focusColor"]
        focusOnPrimaryColor: mainWindow.hasColorError ? "#E3C59A" : settings_manager.appColors["focusOnPrimaryColor"]
        backgroundColor: mainWindow.hasColorError ? "#ECDFD4" : settings_manager.appColors["backgroundColor"]
        drawerBackgroundColor: mainWindow.hasColorError ? "#FAF6F2" : settings_manager.appColors["drawerBackgroundColor"]
        borderButtonColor: mainWindow.hasColorError ? "#1C0F01" : settings_manager.appColors["borderButtonColor"]
        borderButtonFocusColor: mainWindow.hasColorError ? "#FFA000" : settings_manager.appColors["borderButtonFocusColor"]
        notEnabledBorderButtonColor: mainWindow.hasColorError ? "#75614B" : settings_manager.appColors["notEnabledBorderButtonColor"]
        borderButtonHoveredColor: mainWindow.hasColorError ? "#78562C" : settings_manager.appColors["borderButtonHoveredColor"]
        borderButtonPressedColor: mainWindow.hasColorError ? "#78562C" : settings_manager.appColors["borderButtonPressedColor"]
    }

    Connections {
        target: login_register_manager

        function onLoadDb(title) {
            console.log("loadDb")
            loginRegisterScreen.visible = false
            loadDbScreen.visible = true
            mainWindow.color = mainWindow.hasColorError ? "#FAF6F2" : settings_manager.appColors["drawerBackgroundColor"]
            mainWindow.title = title
            mainWindow.flags = Qt.Window | Qt.WindowTitleHint | Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint;
        }
        function onUpdateLoadDbProgress(progress, mes) {
            loadDbScreen.text = mes;
            loadDbScreen.value = progress;
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
}
