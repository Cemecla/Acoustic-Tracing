import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    anchors.fill: parent

    background : Rectangle {
        color: "#d4d4d4"
    }

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent

        SimulationScreen {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            height: 40
            Layout.fillWidth: true
            anchors.horizontalCenter: mainColumn.horizontalCenter
            text: "To Menu"
            onClicked: communicator.loadSimulation("C:/travail/Acoustic-Tracing/saves/mapDev.json")
        }
    }
}
