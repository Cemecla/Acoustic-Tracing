import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D.Helpers

Page {
    required property bool debugEnable
    id: simulationPage
    anchors.fill: parent

    Item {
        id: viewportRoot
        anchors.fill: parent
        property var emitters: communicator.emitters
        property var emittersId: []

        onEmittersChanged: {
            emittersId = Object.keys(emitters)
        }

        SimulationScreen {
            id: viewScreen
            anchors.fill: parent
            debugEnable: simulationPage.debugEnable
            isRaysVisible: true
            isEmittersVisible: true
            emitters: viewportRoot.emitters
            emittersId: viewportRoot.emittersId
        }

        HudSimulation {
            id: hud
            anchors.fill: parent
            z: 1
            emitters: viewportRoot.emitters
            emittersId: viewportRoot.emittersId
            isDebugMode: debugEnable

            onRaysVisibleChanged: {
                viewScreen.isRaysVisible = raysVisible
            }

            onEmittersVisibleChanged: {
                viewScreen.isEmittersVisible = emittersVisible
            }

            onStopRequested: {
                communicator.stopSimulation()
                viewScreen.forceActiveFocus()
            }

            onQuitRequested: {
                communicator.stopSimulation()
                stackView.pop(null)
                stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/MenuPage.qml"))
            }
        }

        DebugView {
            visible: simulationPage.debugEnable
            source: viewScreen
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            resourceDetailsVisible: true
            z: 2
        }
    }
}