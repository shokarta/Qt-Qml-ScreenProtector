import QtQuick
import QtQuick.Window


Window {
    id: root

    visible: overlayController.visible

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool

    color: "transparent"

    Image {
        anchors.fill: parent
        sourceSize: Qt.size(width, height)
        fillMode: Image.PreserveAspectCrop
        source: "resources/screen.jpg"
    }
}