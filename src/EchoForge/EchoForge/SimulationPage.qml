import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D.Helpers

Page {
    required property bool debugEnable
    id: simulationPage
    anchors.fill: parent

    background : Rectangle {
        color: "#d4d4d4"
    }

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent

        SimulationScreen {
            id: viewScreen
            Layout.fillWidth: true
            Layout.fillHeight: true
            debugEnable: simulationPage.debugEnable
        }

        DebugView {
            visible: simulationPage.debugEnable
            source: viewScreen
            resourceDetailsVisible: true
        }

        // Button {
        //     height: 40
        //     Layout.fillWidth: true
        //     anchors.horizontalCenter: mainColumn.horizontalCenter
        //     text: "Start Simulation"
        //     onClicked: communicator.loadSimulation("../../../../Acoustic-Tracing/saves/DevTest.json")
        // }
    }
}
