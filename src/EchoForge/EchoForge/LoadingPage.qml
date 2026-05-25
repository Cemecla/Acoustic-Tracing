import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    property bool isLoading: true
    required property bool debugMode
    property string mapUrl

    id: loadingPage
    Layout.fillHeight: true
    Layout.fillWidth: true

    Connections {
        target: communicator

        function onSimulationLoaded() {
            simulationLoader.setSource("SimulationPage.qml", {"debugEnable": loadingPage.debugMode})
            simulationLoader.active = true
        }
    }

    Component.onCompleted: {
        communicator.loadSimulation(mapUrl)
    }


    Rectangle {
        property int points: 0

        id : loadingBox
        visible: loadingPage.isLoading
        anchors.fill: parent
        color: "white"

        Text {
            anchors.centerIn: parent
            text: "LOADING" + String(".").repeat(loadingBox.points)
            font.pixelSize: 32
        }

        SequentialAnimation {
            running: true
            loops: Animation.Infinite

            NumberAnimation { 
                target: loadingBox
                property: "points"
                to: 3;
                duration: 1000;
            }
        }
    }

    Loader {
        id: simulationLoader
        active: false
        anchors.fill: parent
        asynchronous: true

        onStatusChanged: {
            if (status == Loader.Ready) {
                if (!loadingPage.debugMode)
                    communicator.startSimulation();
                loadingPage.isLoading = false;
            }
        }
    }
}
