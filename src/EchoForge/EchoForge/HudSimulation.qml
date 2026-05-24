import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    signal stopRequested()
    signal quitRequested()

    property bool raysVisible: true
    property bool emittersVisible: true
    property color panelTitleColor: mainWindow.secondaryColor 
    property color panelTextColor: "white"
    property string fontFamily: "Consolas"
    property int subTitleSize: 14

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
                text: "AVATAR"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
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

            HorizontalSeparator { color: root.panelTitleColor }

            Text {
                text: "RAYS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
                font.letterSpacing: 2
            }

            RowLayout {
                spacing: 8

                Text {
                    text: "count"
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
                    color: root.panelTextColor
                    font.family: root.fontFamily; 
                    font.pixelSize: 10
                    font.letterSpacing: 1
                }
            }

            HorizontalSeparator { color: root.panelTitleColor }

            Text {
                text: "EMITTERS (WIP)"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
                font.letterSpacing: 2
            }

        }
    }

    HudPanel {
        id: controlPanel
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 16
        width: 220

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 10

            Text {
                text: "CONTROLS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
                font.letterSpacing: 2
            }

            HudToggle {
                Layout.fillWidth: true
                label: "Display rays"
                checkedText: "True"
                uncheckedText: "False"
                checked: root.raysVisible
                onToggled: root.raysVisible = !root.raysVisible
            }

            HudToggle {
                Layout.fillWidth: true
                label: "Display sources"
                checkedText: "True"
                uncheckedText: "False"
                checked: root.emittersVisible
                onToggled: root.emittersVisible = !root.emittersVisible
            }

            HudButton {
                Layout.fillWidth: true
                label: "Quit to menu (WIP)"
                onActivated: root.quitRequested()
            }

            HorizontalSeparator { color: root.panelTitleColor }

            Text {
                text: "COMMANDS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
                font.letterSpacing: 2
            }

            RowLayout {
                spacing: 8

                Text {
                    text: "Camera Switch"
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
                    text: "C"
                    color:  root.panelTextColor
                    font.family: root.fontFamily; 
                    font.pixelSize: 10
                    font.letterSpacing: 1
                }
            }

            RowLayout {
                spacing: 8

                Text {
                    text: "Rotation"
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
                    text: "Q/E"
                    color: root.panelTextColor
                    font.family: root.fontFamily; 
                    font.pixelSize: 10
                    font.letterSpacing: 1
                }
            }

            RowLayout {
                spacing: 8

                Text {
                    text: "Mouvement"
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
                    text: "W/A/S/D"
                    color: root.panelTextColor
                    font.family: root.fontFamily; 
                    font.pixelSize: 10
                    font.letterSpacing: 1
                }
            }
        }
    }
}
