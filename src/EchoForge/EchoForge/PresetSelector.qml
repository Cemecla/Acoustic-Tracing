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

    function reloadCatalogue() {
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
        reloadCatalogue()
        if (_entries.length > 0) {
            presetCombo.currentIndex = 0
            let first = _entries[0]
            if (first) communicator.setSelectedPinnaId(first.id)
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

        Text {
            text: "Preset"
            color: "#555555"
            font { pixelSize: 11; letterSpacing: 1.5 }
            Layout.fillWidth: true
        }

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
                    border.color: presetCombo.activeFocus ? "#FFA500" : "#BBBBBB"
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
                        implicitHeight: contentHeight
                        model: presetCombo.delegateModel
                        clip: true
                        ScrollIndicator.vertical: ScrollIndicator {}
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
                        color: highlighted ? "#FFA500" : "transparent"
                    }
                }
            }

            Button {
                id: addBtn
                implicitWidth: root._addingNew ? 72 : 32
                implicitHeight: 32
                text: root._addingNew ? "Confirm" : "+"

                Behavior on implicitWidth { NumberAnimation { duration: 120 } }

                contentItem: Text {
                    text: addBtn.text
                    color: addBtn.hovered ? "#FFFFFF" : "#000000"
                    font.pixelSize: root._addingNew ? 12 : 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: addBtn.pressed  ? "#FFA500" : addBtn.hovered  ? "#000000" : "#FFFFFF"
                    border.color: "#FFA500"
                    border.width: 1
                    radius:       4

                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                onClicked: {
                    if (!root._addingNew) {
                        root._addingNew = true
                        newNameField.text = ""
                        newNameField.forceActiveFocus()
                    } else {
                        let trimmed = newNameField.text.trim()
                        if (trimmed.length > 0) {
                            let entry = {
                                id: root._nextTempId--,
                                name: trimmed,
                                ordered_gains: [0,0,0,0,0,0,0]
                            }

                            let copy = root._entries.slice()
                            copy.push(entry)
                            root._entries = copy
                            presetCombo.currentIndex = root._entries.length - 1
                            communicator.setSelectedPinnaId(entry.id)
                            root.presetAdded(entry)
                            root.presetChanged(entry)
                            console.log("Added preset:", trimmed, "id:", entry.id)
                        }
                        root._addingNew = false
                    }
                }
            }
        }

        /// TODO SAVE
        TextField {
            id: newNameField
            Layout.fillWidth: true
            visible: false // root._addingNew
            placeholderText: "New preset name..."
            color: "#000000"
            font.pixelSize: 13

            placeholderTextColor: "#999999"

            background: Rectangle {
                color: "#FFFFFF"
                border.color: newNameField.activeFocus ? "#FFA500" : "#BBBBBB"
                border.width: 1
                radius: 4
            }

            Keys.onReturnPressed: addBtn.clicked()
            Keys.onEscapePressed: root._addingNew = false

            opacity: root._addingNew ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: 120 } }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#BBBBBB"
        }

        Button {
            id: saveBtn
            Layout.fillWidth: true
            implicitHeight: 34
            text: "Save"

            contentItem: Text {
                text: saveBtn.text
                color: saveBtn.hovered ? "#FFFFFF" : "#000000"
                font { pixelSize: 13; letterSpacing: 0.8 }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: saveBtn.pressed ? "#FFA500" : saveBtn.hovered ? "#000000" : "#FFFFFF"
                border.color: "#FFA500"
                border.width: 1
                radius: 4

                Behavior on color { ColorAnimation { duration: 120 } }
            }

            onClicked: {
                let entry = root.currentPreset
                if (!entry) {
                    console.warn("Save requested with no selection.")
                    return
                }

                // communicator.setSelectedPinnaId(entry.id)
                // root.saveRequested(entry)
                console.log("Save requested for preset id:", entry.id, "name:", entry.name)
            }
        }

        Item { implicitHeight: 2 }
    }
}
