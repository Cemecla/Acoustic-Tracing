import QtQuick

Rectangle {
    id: panel
    default property alias body: container.data

    color: "#cc121417"
    border.color: "#2a2a2a"
    border.width: 1
    radius: 2

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 2
        color: "#FFA500"
    }

    Item {
        id: container
        anchors.fill: parent
    }

    implicitHeight: container.childrenRect.height + 28
}