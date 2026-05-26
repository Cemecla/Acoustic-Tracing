import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    signal stopRequested()
    signal quitRequested()

    property bool isDebugMode: false
    property bool raysVisible: true
    property bool emittersVisible: true
    property color panelTitleColor: mainWindow.secondaryColor 
    property color panelTextColor: "white"
    property string fontFamily: "Consolas"
    property int subTitleSize: 14
    property var emitters: null
    property var emittersId: []

    HudPanel {
        id: infoPanel

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 16
        width: 450

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

                TextInput {
                    width: 50
                    enabled: root.isDebugMode
                    text: communicator.avatarPos.x.toFixed(1)
                    color: root.panelTextColor; font.family: root.fontFamily; font.pixelSize: 13
                    validator: DoubleValidator {bottom: 1.0; top: 100.0}
                    onTextChanged: {
                        if (root.isDebugMode) {
                            console.log("")
                        }
                    }
                }

                Text { text: "y"; color: root.panelTitleColor; font.family: root.fontFamily; font.pixelSize: 12 }

                TextInput {
                    width: 50
                    enabled: root.isDebugMode
                    text: communicator.avatarPos.y.toFixed(1)
                    color: root.panelTextColor; font.family: root.fontFamily; font.pixelSize: 13
                    validator: DoubleValidator {bottom: 1.0; top: 100.0}
                    onTextChanged: {
                        if (root.isDebugMode) {
                            console.log("")
                        }
                    }
                }

                Text { text: "z"; color: root.panelTitleColor; font.family: root.fontFamily; font.pixelSize: 12 }

                TextInput {
                    width: 50
                    enabled: root.isDebugMode
                    text: communicator.avatarPos.z.toFixed(1)
                    color: root.panelTextColor; font.family: root.fontFamily; font.pixelSize: 13
                    validator: DoubleValidator {bottom: 1; top: 100}
                    onTextChanged: {
                        if (root.isDebugMode) {
                            console.log("")
                        }
                    }
                }
            }

            HorizontalSeparator { color: root.panelTitleColor; visible: !root.isDebugMode }

            Text {
                visible: !root.isDebugMode
                text: "RAYS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
                font.letterSpacing: 2
            }

            RowLayout {
                visible: !root.isDebugMode
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

            RowLayout {
                visible: !root.isDebugMode
                spacing: 8

                Text {
                    text: "Refresh Time"
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
                    id: refreshTimeText
                    text: communicator.getRayRefreshTime()
                    color: "#5a5a5a"; 
                    font.family: root.fontFamily; 
                    font.pixelSize: 13
                }

                OptionSlider {
                    id: raySlider
                    from: 0.1
                    to: 5.0
                    stepSize: 0.1
                    width: 30
                    value: communicator.getRayRefreshTime()

                    onValueChanged: {
                        if (activeFocus || pressed) {
                            communicator.setRayRefreshTime(value)
                            refreshTimeText.text = value.toFixed(1)
                        }
                    }
                }
            }
            
            HorizontalSeparator { color: root.panelTitleColor }

            Text {
                text: "EMITTERS"
                color: root.panelTitleColor
                font.family: root.fontFamily
                font.pixelSize: root.subTitleSize
                font.letterSpacing: 2
            }

            ScrollView {
                id: emittersScroll
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                clip: true
                ScrollBar.vertical.policy: ScrollBar.AsNeeded
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            
                ListView {
                    id: emittersList
                    model: root.emittersId
                    spacing: 4
                    boundsBehavior: Flickable.StopAtBounds
            
                    delegate: Rectangle {
                        width: ListView.view.width
                        height: 52
                        color: index % 2 == 0 ? "#1a1a1a" : "#222222"
                        border.color: "#333333"
                        border.width: 1
                        radius: 3
            
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 10
            
                            Rectangle {
                                Layout.preferredWidth: 14
                                Layout.preferredHeight: 14
                                Layout.alignment: Qt.AlignVCenter
                                color: root.emitters[modelData]["color"]
                                border.color: "#000000"
                                border.width: 1
                                radius: 50
                            }
            
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
            
                                Text {
                                     text: {
                                        const full = root.emitters[modelData]["filename"] || ""
                                        const i = Math.max(full.lastIndexOf("/"), full.lastIndexOf("\\"))
                                        return i >= 0 ? full.substring(i + 1) : full
                                     }
                                    color: "#ffffff"
                                    font.family: root.fontFamily
                                    font.pixelSize: 12
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
            
                                Text {
                                    text: `x: ${root.emitters[modelData]["position"].x.toFixed(1)}   ` +
                                          `y: ${root.emitters[modelData]["position"].y.toFixed(1)}   ` +
                                          `z: ${root.emitters[modelData]["position"].z.toFixed(1)}`
                                    color: "#aaaaaa"
                                    font.family: root.fontFamily
                                    font.pixelSize: 10
                                }
                            }
                        }
                    }
                }
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
                visible: !root.isDebugMode
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
                visible: root.isDebugMode
                Layout.fillWidth: true
                label: "Save (WIP)"
                onActivated: root.quitRequested()
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
                visible: !root.isDebugMode
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
                visible: !root.isDebugMode
                spacing: 8

                Text {
                    text: "Top Camera Up/Down"
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
                    text: "Z/X"
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
                    text: !root.isDebugMode ? "Q/E" : "Mouse + Click"
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
