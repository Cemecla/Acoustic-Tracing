import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtMultimedia
import Qt.labs.folderlistmodel

MainLayout {
    id: root
    required property bool debugMode
    maintitle: "Map Selection"

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 15

        // HEADER
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
                width: 4
                Layout.preferredHeight: 24
                color: "#FFA500"
            }

            Text {
                text: "Select a map to load"
                font.pixelSize: 18
                font.italic: true
                color: "#555555"
                Layout.fillWidth: true
            }

            Text {
                text: folderModel.count + " map" + (folderModel.count > 1 ? "s" : "") + " available"
                font.pixelSize: 14
                color: "#888888"
            }
        }

        // SEPARATOR
        HorizontalSeparator {}

        // LIST
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: mapListView
                model: folderModel
                spacing: 4

                delegate: ItemDelegate {
                    id: mapItem
                    required property string fileName
                    required property string filePath
                    required property int index

                    width: ListView.view.width
                    height: 56
                    hoverEnabled: true

                    // BACKGROUND
                    background: Item {
                        Rectangle {
                            anchors.fill: parent
                            color: "black"
                            opacity: mapItem.hovered ? 0.15 : 0
                            Behavior on opacity {
                                NumberAnimation { duration: 150 }
                            }
                        }

                        Rectangle {
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            width: mapItem.hovered ? 5 : 0
                            color: "#FFA500"
                            opacity: mapItem.hovered ? 1 : 0

                            Behavior on opacity {
                                NumberAnimation { duration: 150 }
                            }

                            Behavior on width {
                                NumberAnimation { duration: 150 }
                            }
                        }
                    }

                    contentItem: RowLayout {
                        spacing: 12

                        // INDEX
                        Text {
                            text: String(mapItem.index + 1).padStart(2, "0")
                            font.pixelSize: 14
                            font.bold: true
                            color: mapItem.hovered ? "#FFA500" : "#AAAAAA"
                            Layout.preferredWidth: 30

                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                        }

                        // FILE NAME
                        Text {
                            text: mapItem.fileName.replace(/\.json$/i, "")
                            color: mapItem.hovered ? "#FFFFFF" : "#000000"
                            font.pixelSize: 18
                            verticalAlignment: Text.AlignVCenter
                            Layout.fillWidth: true
                        }

                        // ARROW
                        Text {
                            text: "›"
                            font.pixelSize: 24
                            font.bold: true
                            color: "#FFA500"
                            opacity: mapItem.hovered ? 1 : 0
                            verticalAlignment: Text.AlignVCenter

                            Behavior on opacity {
                                NumberAnimation { duration: 150 }
                            }
                        }
                    }

                    // SOUND EFFECT
                    SoundEffect {
                        id: hoverSound
                        source: "qrc:/sounds/buttonSound"
                        loops: 1
                    }

                    onHoveredChanged: {
                        if (hovered) hoverSound.play();
                    }

                    onClicked: stackView.push(
                        Qt.resolvedUrl("qrc:/qt/qml/echoforge/LoadingPage.qml"),
                        { mapUrl: filePath, debugMode: root.debugMode }
                    )
                }

                // TEXT IF NO MAP
                Text {
                    anchors.centerIn: parent
                    visible: folderModel.count == 0 && folderModel.status == FolderListModel.Ready
                    text: "No maps found"
                    font.pixelSize: 18
                    font.italic: true
                    color: "#999999"
                }

                // FOLDER LIST
                FolderListModel {
                    id: folderModel
                    folder: Qt.resolvedUrl("file:./res/saves/")
                    nameFilters: ["*.json"]
                    showDirs: false
                }
            }
        }

        // SEPARATOR
        HorizontalSeparator {}

       // BACK BUTTON
       MenuButton {
           buttonText: "← Return"
           Layout.fillWidth: true
           onClicked: stackView.pop()
       }
    }
}