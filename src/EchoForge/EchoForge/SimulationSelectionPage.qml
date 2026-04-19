import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.folderlistmodel

MainLayout {
    required property bool debugMode
    id: selectionPage
    maintitle: "Map Selection"
    anchors.fill: parent

    ScrollView {
        ListView {
            model: folderModel
            delegate: ItemDelegate {
                required property string fileName
                required property string filePath

                contentItem: Text {
                    text: fileName
                    color: "black"
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/LoadingPage.qml"), { mapUrl: filePath, debugMode: selectionPage.debugMode })
            }

            FolderListModel {
                id: folderModel
                folder: Qt.resolvedUrl("file:../../../../Acoustic-Tracing/saves/")
                nameFilters: ["*.json"]
                showDirs: false
            }
        }
    }
}
