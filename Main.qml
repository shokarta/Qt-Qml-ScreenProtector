import QtQuick
import QtQuick.Window


Window {
    id: root

    visible: false
    //visible: overlayController.visible
    //visibility: overlayController.visible ? Window.FullScreen : Window.Hidden           // helps to show windows as TOPMOST

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool

    color: "transparent"

    Image {
        anchors.fill: parent
        sourceSize: Qt.size(width, height)
        fillMode: Image.PreserveAspectCrop
        source: "resources/screen.jpg"
    }
}