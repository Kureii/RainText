import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 840
    height: 500
    title: "Vítej v Qt QML"
    color: "transparent"
    minimumHeight: 500
    minimumWidth: 800
    flags: Qt.FramelessWindowHint


    LoginRegisterScreen {}
}
