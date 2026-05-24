import QtQuick
import QtQuick.Layouts

Rectangle {
    id: panel
    default property alias body: contentLayout.data

    color: "#cc121417"
    border.color: "#2a2a2a"
    border.width: 1
    radius: 2

    height: contentLayout.implicitHeight + 25

    Rectangle {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 2
        color: mainWindow.secondaryColor
        z: 2
    }

    ColumnLayout {
        id: contentLayout
        
        anchors.left: parent.left
        anchors.right: parent.right

        spacing: 0 
    }
}