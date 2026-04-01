import QtQuick
import QtQuick.Controls

Page {
    width: Window.width
    height: Window.height

    background : Rectangle {
        color: "#d4d4d4"
    }

    Column {
        id: mainColumn
        anchors.centerIn: parent

        Text {
            anchors.horizontalCenter: mainColumn.horizontalCenter
            text: "Simulation!"
        }

        Button {
            anchors.horizontalCenter: mainColumn.horizontalCenter
            text: "To Menu"
            onClicked: communicator.testing("Test")
        }
    }
}
