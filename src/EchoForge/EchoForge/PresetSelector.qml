import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    readonly property var currentPreset:
        (presetCombo.currentIndex >= 0 && _entries.length > 0)
            ? _entries[presetCombo.currentIndex]
            : null

    readonly property int currentPresetId: currentPreset ? currentPreset.id : -1 // Temporaary
    readonly property string currentPresetName: currentPreset ? currentPreset.name : ""

    signal presetChanged(var preset)
    signal saveRequested(var preset)
    signal presetAdded(var preset)

    Layout.fillWidth: true
    implicitWidth: 320
    implicitHeight: rootColumn.implicitHeight + 24

    color: "#d4d4d4"
    border.color: "#BBBBBB"
    border.width: 1
    radius: 6

    property var _entries: []

    property int _nextTempId: -1
    property bool _addingNew: false

    function reloadSettings() {
        let cat = communicator.loadPinnaSetting()
        let arr = []
        for (let i = 0; i < cat.length; ++i) {
            let src = cat[i]
            let gains = []
            if (src.ordered_gains) {
                for (let g = 0; g < src.ordered_gains.length; ++g) {
                    gains.push(Number(src.ordered_gains[g]))
                }
            }

            arr.push({
                id: src.id,
                name: src.name,
                category: src.category,
                cipic_subject: src.cipic_subject,
                ear_size_cm: src.ear_size_cm,
                description: src.description,
                ordered_gains: gains
            })
        }
        _entries = arr
    }

    Component.onCompleted: {
        reloadSettings()
        if (_entries.length > 0) {
            let index = appSettings.pinaId
            presetCombo.currentIndex = index
            let first = _entries[index]
            if (first) 
                communicator.setSelectedPinnaId(first.id)
            root.presetChanged(first)
        }
    }

    ColumnLayout {
        id: rootColumn
        anchors {
            left: parent.left;  
            right: parent.right
            top: parent.top
            margins: 12
        }
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 6

            ComboBox {
                id: presetCombo
                Layout.fillWidth: true
                model: root._entries
                textRole: "name"
                valueRole: "id"

                onActivated: {
                    let entry = root._entries[currentIndex]
                    if (entry) {
                        appSettings.pinaId = currentIndex
                        communicator.setSelectedPinnaId(entry.id)
                        root.presetChanged(entry)
                    }
                }

                contentItem: Text {
                    leftPadding: 10
                    text: presetCombo.displayText
                    color: "#000000"
                    font.pixelSize: 13
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: presetCombo.pressed ? "#EEEEEE" : "#FFFFFF"
                    border.color: presetCombo.activeFocus ? mainWindow.secondaryColor : "#BBBBBB"
                    border.width: 1
                    radius: 4
                }

                popup: Popup {
                    y: presetCombo.height + 2
                    width: presetCombo.width
                    padding: 1

                    background: Rectangle {
                        color: "#FFFFFF"
                        border.color: "#BBBBBB"
                        border.width: 1
                        radius: 4
                    }

                    contentItem: ListView {
                        implicitHeight: 300
                        model: presetCombo.delegateModel
                        clip: true
                            ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AlwaysOn
                            active: true
                        }
                    }
                }

                delegate: ItemDelegate {
                    width: presetCombo.width
                    height: 32

                    contentItem: Text {
                        leftPadding: 10
                        text: modelData ? modelData.name : ""
                        color: highlighted ? "#FFFFFF" : "#000000"
                        font.pixelSize: 13
                        verticalAlignment: Text.AlignVCenter
                    }

                    highlighted: presetCombo.highlightedIndex == index

                    background: Rectangle {
                        color: highlighted ? mainWindow.secondaryColor : "transparent"
                    }
                }
            }
        }
    }
}
