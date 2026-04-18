import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    property bool isLoading: true
    property string mapUrl

    id: loadingPage
    Layout.fillHeight: true
    Layout.fillWidth: true

    Connections {
        target: communicator

        function onSimulationLoaded() {
            simulationLoader.active = true
        }
    }

    Component.onCompleted: {
        communicator.loadSimulation("../../../../Acoustic-Tracing/saves/generatedMap2.json")
    }

    
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
        id: simulationLoader
        active: false
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
