import QtQuick
import QtQuick.Controls

Rectangle {
    id: btn
    property string label: ""
    property color borderColor: mainWindow.secondaryColor
    signal activated()

    height: 32
    color: mouseArea.pressed ? Qt.darker(borderColor, 1.4) : mouseArea.containsMouse ? "#1f2428" : "transparent"
    border.color: borderColor
    border.width: 1
    radius: 2

    Text {
        anchors.centerIn: parent
        text: btn.label
        color: mouseArea.containsMouse ? "#ffffff" : "#cccccc"
        font.family: "Consolas"
        font.pixelSize: 12
        font.letterSpacing: 1
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: btn.activated()
    }
}