import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    property bool isLoading: true
    id: loadingPage
    Layout.fillHeight: true
    Layout.fillWidth: true
    
    Rectangle {
        visible: loadingPage.isLoading
        anchors.fill: parent
        color: "white"
        Text {
            anchors.centerIn: parent
            text: "LOADING..."
            font.pixelSize: 32
        }
    }

    Loader {
        active: true
        anchors.fill: parent
        asynchronous: true
        source: "SimulationPage.qml"

        onStatusChanged: {
            if (status == Loader.Ready) {
                loadingPage.isLoading = false
            }
        }
    }
}
