import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    property bool isLoading: true
    Layout.fillHeight: true
    Layout.fillWidth: true
    
    Rectangle {
        visible: isLoading
        anchors.fill: parent
        color: "white"
        Text {
            anchors.centerIn: parent
            text: "LOADING..."
            font.pixelSize: 32
        }
    }

    Loader {
        anchors.fill: parent
        asynchronous: true
        source: "SimulationPage.qml"

        onStatusChanged: {
            if (status == Loader.Ready) {
                isLoading = false
            }
        }
    }
}
