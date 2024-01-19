import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

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
            color: "#ECDFD4"
    }
    //LoginRegisterScreen {}
}
