import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    signal stopRequested()
    signal quitRequested()

    property bool raysVisible: true
    property color panelTitleColor: "#FFA500" 
    property color panelTextColor: "#000000"
    property string fontFamily: "Consolas"

    HudPanel {
        id: infoPanel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 16
        width: 200

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 8

            Text {
                text: "INFORMATIONS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: 11
                font.letterSpacing: 2
            }

            Grid {
                columns: 2
                columnSpacing: 12
                rowSpacing: 4

                Text { text: "x"; color: root.panelTitleColor; font.family: root.fontFamily; font.pixelSize: 12 }

                Text {
                    text: communicator.avatarPos.x.toFixed(1)
                    color: root.panelTextColor; font.family: root.fontFamily; font.pixelSize: 13
                }

                Text { text: "y"; color: root.panelTitleColor; font.family: root.fontFamily; font.pixelSize: 12 }

                Text {
                    text: communicator.avatarPos.y.toFixed(1)
                    color: root.panelTextColor; font.family: root.fontFamily; font.pixelSize: 13
                }

                Text { text: "z"; color: root.panelTitleColor; font.family: root.fontFamily; font.pixelSize: 12 }

                Text {
                    text: communicator.avatarPos.z.toFixed(1)
                    color: root.panelTextColor; font.family: root.fontFamily; font.pixelSize: 13
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: "#2a2a2a" }

            RowLayout {
                spacing: 8

                Text {
                    text: "rays"
                    color: root.panelTitleColor; 
                    font.family: root.fontFamily; 
                    font.pixelSize: 12
                }

                Text {
                    text: "—"
                    color: "#5a5a5a"; 
                    font.family: root.fontFamily; 
                    font.pixelSize: 13
                }

                Text {
                    text: communicator.rayInstances.rayCount
                    color: "#f0b040"
                    font.family: root.fontFamily; 
                    font.pixelSize: 10
                    font.letterSpacing: 1
                }
            }
        }
    }

    HudPanel {
        id: controlPanel
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 16
        width: 200

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 10

            Text {
                text: "CONTROLS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: 11
                font.letterSpacing: 2
            }

            HudToggle {
                Layout.fillWidth: true
                label: "Show rays"
                checkedText: "Visible"
                uncheckedText: "Invisible"
                checked: root.raysVisible
                onToggled: root.raysVisible = !root.raysVisible
            }

            HudButton {
                Layout.fillWidth: true
                label: "Stop simulation (WIP)"
                onActivated: root.stopRequested()
            }

            HudButton {
                Layout.fillWidth: true
                label: "Quit to menu (WIP)"
                onActivated: root.quitRequested()
            }
        }
    }
}
