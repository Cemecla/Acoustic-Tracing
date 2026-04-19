import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D.Helpers

Page {
    required property bool debugEnable
    id: simulationPage
    anchors.fill: parent

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
    }
}
