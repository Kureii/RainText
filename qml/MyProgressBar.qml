import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects

Rectangle {
    id: passwordProgressBar
    height: 0
    width: parent.width
    property real value: 0.0
    clip: true

    radius: height / 2
    color: "transparent"

    Rectangle {
        id: contentItemPasswordProgressBar
        width: passwordProgressBar.value * passwordProgressBar.width
        height: passwordProgressBar.height
        radius: height / 2
        color: Qt.hsla(passwordProgressBar.value * 0.4, .8, 0.45, passwordProgressBar.value === 0? 0 : 1)
    }
}