import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

MainLayout {
    maintitle: "Options"

    ColumnLayout {
        id: mainColumn
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.leftMargin: 10
        Layout.rightMargin: 10
        Layout.bottomMargin: 10
        Layout.topMargin: 20
        spacing: 0

        Rectangle {
            id: optionPanel
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#c8c8c8"
            border.color: "#888888"
            border.width: 1

            ColumnLayout {
                id: panelColumn
                anchors.fill: parent
                spacing: 0

                // TAB BAR
                TabBar {
                    id: tabBar
                    Layout.fillWidth: true
                    background: Rectangle { color: "#c8c8c8" }

                    TabButton {
                        id: audioTab
                        text: "Audio"
                        font.pointSize: 13
                        implicitWidth: contentItem.implicitWidth + 24
                        implicitHeight: 40

                        background: Rectangle {
                            color: audioTab.checked ? "#111111" : "transparent"
                            border.color: audioTab.checked ? "transparent" : "#888888"
                            border.width: audioTab.checked ? 0 : 1
                        }

                        contentItem: Text {
                            text: audioTab.text
                            font: audioTab.font
                            color: audioTab.checked ? "#ffffff" : "#111111"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    TabButton {
                        id: graphicTab
                        text: "Graphic"
                        font.pointSize: 13
                        implicitWidth: contentItem.implicitWidth + 24
                        implicitHeight: 40

                        background: Rectangle {
                            color: graphicTab.checked ? "#111111" : "transparent"
                            border.color: graphicTab.checked ? "transparent" : "#888888"
                            border.width: graphicTab.checked ? 0 : 1
                        }

                        contentItem: Text {
                            text: graphicTab.text
                            font: graphicTab.font
                            color: graphicTab.checked ? "#ffffff" : "#111111"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    TabButton {
                        id: configTab
                        text: "Configuration"
                        font.pointSize: 13
                        implicitWidth: contentItem.implicitWidth + 24
                        implicitHeight: 40

                        background: Rectangle {
                            color: configTab.checked ? "#111111" : "transparent"
                            border.color: configTab.checked ? "transparent" : "#888888"
                            border.width: configTab.checked ? 0 : 1
                        }

                        contentItem: Text {
                            text: configTab.text
                            font: configTab.font
                            color: configTab.checked ? "#ffffff" : "#111111"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                Rectangle {
                    id: tabSeparator
                    Layout.fillWidth: true
                    height: 1
                    color: "#888888"
                }

                // CONTENT
                StackLayout {
                    id: stackLayout
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: tabBar.currentIndex

                    // AUDIO SUBPAGE
                    Item {
                        id: audioPage
                        ColumnLayout {
                            id: audioColumn
                            anchors { left: audioPage.left; right: audioPage.right; top: audioPage.top }
                            spacing: 0

                            OptionRow {
                                id: sfxVolumeRow
                                rowLabel: "SFX Volume"
                                OptionSlider { 
                                    id: sfxSlider
                                    initialValue: appSettings.sfxVolume * 100 
                                    onValueChanged: {
                                        appSettings.sfxVolume = value / 100
                                    }
                                }
                            }

                            OptionRow {
                                id: pinnaRow
                                rowLabel: "Pinna Setting"

                                PresetSelector {}
                            }
                        }
                    }

                    // GRAPHIC SUBPAGE
                    Item {
                        id: graphicPage
                        ColumnLayout {
                            id: graphicColumn
                            anchors { left: graphicPage.left; right: graphicPage.right; top: graphicPage.top }
                            spacing: 0

                            OptionRow {
                                id: resolutionRow
                                rowLabel: "Resolution"
                                OptionCycleButton { 
                                    id: resolutionCycle
                                    currentIndex: appSettings.graphicQuality == "Low" ? 0 : appSettings.graphicQuality == "Medium" ? 1 : 2
                                    options: ["Low", "Medium", "High"] 
                                    callback: (option) => { appSettings.graphicQuality = option }
                                }
                            }

                            OptionRow {
                                id: frustrumRow
                                rowLabel: "Frustrum Culling (WIP)"
                                OptionCycleButton { id: frustrumCycle; options: ["On", "Off"] }
                            }

                            OptionRow {
                                id: fullscreenRow
                                rowLabel: "Fullscreen"
                                OptionCycleButton { 
                                    id: fullscreenCycle
                                    currentIndex: appSettings.screenSize == "Windowed" ? 0 : 1
                                    options: ["Windowed", "Fullscreen"]        
                                    callback: (option) => {
                                        switch(option) {
                                            case "Fullscreen":
                                                mainWindow.visibility = Window.FullScreen
                                                appSettings.screenSize = "Fullscreen"
                                                break
                                            case "Windowed":
                                                mainWindow.visibility = Window.Windowed
                                                appSettings.screenSize = "Windowed"
                                                break
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // CONFIG SUBPAGE
                    Item {
                        id: configPage
                        ColumnLayout {
                            id: configColumn
                            anchors { left: configPage.left; right: configPage.right; top: configPage.top }
                            spacing: 0

                            OptionRow {
                                id: autoSaveRow
                                rowLabel: "Auto-Save"
                                OptionCycleButton { id: autoSaveCycle; options: ["Off", "On"] }
                            }

                            OptionRow {
                                id: languageRow
                                rowLabel: "Language"
                                OptionCycleButton {
                                    id: languageCycle
                                    options: ["English", "Français", "Español", "日本語"]
                                }
                            }
                        }
                    }
                }

                // RETURN BUTTONS
                Rectangle {
                    id: bottomSeparator
                    Layout.fillWidth: true
                    height: 1
                    color: "#888888"
                }

                ColumnLayout {
                    id: actionButtons
                    Layout.fillWidth: true
                    spacing: 2
                    Layout.alignment: Qt.AlignCenter

                    MenuButton {
                        id: acceptButton
                        buttonText: "Accept"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        font.pointSize: 18
                        onClicked: stackView.pop()
                    }

                    MenuButton {
                        id: returnButton
                        buttonText: "Return"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        font.pointSize: 18
                        onClicked: stackView.pop()
                    }
                }
            }
        }
    }

    component OptionRow: Rectangle {
        id: optionRowRoot
        property string rowLabel: ""
        default property alias rowControl: controlSlot.children
    
        Layout.fillWidth: true
        Layout.preferredHeight: 44 + controlSlot.implicitHeight
        color: "transparent"
    
        Rectangle {
            id: rowSeparator
            anchors {
                bottom: optionRowRoot.bottom
                left: optionRowRoot.left
                right: optionRowRoot.right
            }
            height: 1
            color: "#aaaaaa"
        }
    
        Text {
            id: rowLabelText
            anchors {
                left: optionRowRoot.left
                leftMargin: 12
                verticalCenter: optionRowRoot.verticalCenter
            }
            text: optionRowRoot.rowLabel
            font.pointSize: 13
            color: "#111111"
        }
    
        Item {
            id: controlSlot
            anchors {
                right: optionRowRoot.right
                rightMargin: 12
                verticalCenter: optionRowRoot.verticalCenter
            }
            implicitWidth: children.length > 0 ? children[0].implicitWidth  : 0
            implicitHeight: children.length > 0 ? children[0].implicitHeight : 0
            width: implicitWidth
            height: implicitHeight
        }
    }   
}
