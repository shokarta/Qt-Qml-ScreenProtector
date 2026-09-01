import QtQuick
import QtQuick.Window


Window {
    id: root

    visible: false

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool | Qt.WindowDoesNotAcceptFocus

    onClosing: function(close) { close.accepted = false }

    color: "transparent"

    Image {
        anchors.fill: parent
        sourceSize: Qt.size(width, height)
        fillMode: Image.PreserveAspectCrop
        source: "qrc:/" + APP_URI + "/resources/screen.jpg"
    }
}