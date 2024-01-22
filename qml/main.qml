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
    title: "Vítej v Qt QML"
    color: "transparent"
    minimumHeight: 500
    minimumWidth: 800
    //flags: Qt.Window | Qt.FramelessWindowHint

    Component.onCompleted: {
        if (settings_manager.hasError) {
            errorDialog.text = settings_manager.errorMessage
            errorDialog.title = settings_manager.errorHeadline
            errorDialog.open()
        }
    }

    Connections {
        target: settings_manager
        function errorChanged(settings_manager) {
            if (settings_manager.hasError) {
                errorDialog.text = settings_manager.errorMessage
                errorDialog.title = settings_manager.errorHeadline
                errorDialog.open()
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

    PasswordManagerScreen {
        id: passwordManagerScreen
        anchors.fill: parent
        primaryColor: settings_manager.hasError? "#FF9800" : appColors["primaryColor"]
        primaryPressedColor: settings_manager.hasError? "#8A5300" : appColors["primaryPressedColor"]
        primaryHoveredColor: settings_manager.hasError? "#b56c00" : appColors["primaryHoveredColor"]
        notEnablePrimaryColor: settings_manager.hasError? "#F0B762" : appColors["notEnablePrimaryColor"]
        textColor: settings_manager.hasError? "#0C011C" : appColors["textColor"]
        notEnableTextColor: settings_manager.hasError? "#453957" : appColors["notEnableTextColor"]
        focusColor: settings_manager.hasError? "#FFA000" : appColors["focusColor"]
        textOnPrimaryColor: settings_manager.hasError? "#1C1001" : appColors["textOnPrimaryColor"]
        focusOnPrimaryColor: settings_manager.hasError? "#E3C59A" : appColors["focusOnPrimaryColor"]
        backgroundColor: settings_manager.hasError? "#ECDFD4" : appColors["backgroundColor"]
        drawerBackgroundColor: settings_manager.hasError? "#FAF6F2" : appColors["drawerBackgroundColor"]
        borderButtonColor: settings_manager.hasError? "#1C0F01" : appColors["borderButtonColor"]
        borderButtonFocusColor: settings_manager.hasError? "#FFA000" : appColors["borderButtonFocusColor"]
        nonEnabledBorderButtonColor: settings_manager.hasError? "#75614B" : appColors["nonEnabledBorderButtonColor"]
        borderButtonHoveredColor: settings_manager.hasError? "#78562C" : appColors["borderButtonHoveredColor"]
    }
    //LoginRegisterScreen {}
}
