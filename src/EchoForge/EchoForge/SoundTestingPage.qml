import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

MainLayout {
    id: mainLayout
    maintitle: "Sound Testing"

    // state 
    property var registrySounds: []       // [{id, path}]
    property var activeEmitters: []       // [{emitterId, soundFileId, state}]
    property int selectedEmitterId: -1
    property int selectedSoundFileId: -1
    property bool isAzimuthRotating: false
    property bool isElevationRotating: false

    // functuion
    function updateEmitterState(emitterId, newState) {
        let updated = []
        for (let i = 0; i < mainLayout.activeEmitters.length; i++) {
            let e = mainLayout.activeEmitters[i]
            if (e.emitterId === emitterId)
                updated.push({ emitterId: e.emitterId, soundFileId: e.soundFileId, state: newState })
            else
                updated.push(e)
        }
        mainLayout.activeEmitters = updated
    }
    function removeEmitter(emitterId) {
        let updated = []
        for (let i = 0; i < mainLayout.activeEmitters.length; i++) {
            if (mainLayout.activeEmitters[i].emitterId !== emitterId)
                updated.push(mainLayout.activeEmitters[i])
        }
        mainLayout.activeEmitters = updated
        if (mainLayout.selectedEmitterId === emitterId)
            mainLayout.selectedEmitterId = -1
    }

    // file dialogs
    FileDialog {
        id: openSoundDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Sound files (*.mp3 *.wav)"]
        onAccepted: {
            let path = decodeURIComponent(selectedFile.toString().replace(/^(file:\/{3})/, ""))
            communicator.addSound(path)
            mainLayout.registrySounds = communicator.getSounds()
        }
    }

    FileDialog {
        id: loadRegistryDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            let path = decodeURIComponent(selectedFile.toString().replace(/^(file:\/{3})/, ""))
            communicator.loadRegistry(path)
            mainLayout.registrySounds = communicator.getSounds()
        }
    }

    FileDialog {
        id: saveRegistryDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            let path = decodeURIComponent(selectedFile.toString().replace(/^(file:\/{3})/, ""))
            communicator.saveRegistry(path)
        }
    }

    //  animations 
    NumberAnimation {
        id: rotationAzimuth
        target: azimuthSlider
        property: "value"
        from: -180; to: 180
        duration: 4000
        loops: Animation.Infinite
        running: false
    }

    NumberAnimation {
        id: rotationElevation
        target: elevationSlider
        property: "value"
        from: -90; to: 90
        duration: 4000
        loops: Animation.Infinite
        running: false
    }

    //  root layout 
    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 0

        // COLUMN 1 — Registry

        ColumnLayout {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            Layout.leftMargin: 16
            Layout.topMargin: 8
            Layout.bottomMargin: 8
            spacing: 4

            Text {
                text: "REGISTRY"
                font.pixelSize: 13
                font.letterSpacing: 3
                color: "#888"
            }

            Rectangle { height: 1; Layout.fillWidth: true; color: "#FFA500" }

            // sound list
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1a1a1a"
                radius: 2

                ListView {
                    id: registryList
                    anchors.fill: parent
                    anchors.margins: 4
                    clip: true
                    model: mainLayout.registrySounds

                    delegate: Rectangle {
                        width: registryList.width
                        height: 40
                        color: mainLayout.selectedSoundFileId === modelData.id
                               ? "#FFA500" : (index % 2 === 0 ? "#222" : "#1e1e1e")
                        radius: 2

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 4
                            spacing: 6

                            Text {
                                text: "id:" + modelData.id
                                color: mainLayout.selectedSoundFileId === modelData.id ? "#000" : "#FFA500"
                                font.pixelSize: 11
                                font.family: "Courier New"
                                Layout.preferredWidth: 36
                            }

                            Text {
                                text: modelData.path.split("/").pop().split("\\").pop()
                                color: mainLayout.selectedSoundFileId === modelData.id ? "#000" : "#ccc"
                                font.pixelSize: 11
                                elide: Text.ElideLeft
                                Layout.fillWidth: true
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: mainLayout.selectedSoundFileId = modelData.id
                        }
                    }
                }
            }

            // registry buttons
            MenuButton {
                buttonText: "＋ Add Sound"
                Layout.fillWidth: true
                onClicked: openSoundDialog.open()
            }
            MenuButton {
                buttonText: "Load Registry"
                Layout.fillWidth: true
                onClicked: loadRegistryDialog.open()
            }
            MenuButton {
                buttonText: "Save Registry"
                Layout.fillWidth: true
                onClicked: saveRegistryDialog.open()
            }
            MenuButton {
                buttonText: "Unload Registry"
                Layout.fillWidth: true
                onClicked: {
                    communicator.pauseAll()
                    communicator.resetEmitters()
                    communicator.unloadRegistry()
                    mainLayout.registrySounds = []
                    mainLayout.activeEmitters = []
                    mainLayout.selectedSoundFileId = -1
                    mainLayout.selectedEmitterId = -1
                }
            }
        }

        // separator
        Rectangle { width: 1; Layout.fillHeight: true; color: "#555"; Layout.topMargin: 8; Layout.bottomMargin: 8 }

        // COLUMN 2 — Emitters
        ColumnLayout {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            Layout.leftMargin: 16
            Layout.topMargin: 8
            Layout.bottomMargin: 8
            spacing: 4

            Text {
                text: "EMITTERS"
                font.pixelSize: 13
                font.letterSpacing: 3
                color: "#888"
            }

            Rectangle { height: 1; Layout.fillWidth: true; color: "#FFA500" }

            // emitter list
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1a1a1a"
                radius: 2

                ListView {
                    id: emitterList
                    anchors.fill: parent
                    anchors.margins: 4
                    clip: true
                    model: mainLayout.activeEmitters

                    delegate: Rectangle {
                        width: emitterList.width
                        height: 40
                        color: mainLayout.selectedEmitterId === modelData.emitterId
                               ? "#FFA500" : (index % 2 === 0 ? "#222" : "#1e1e1e")
                        radius: 2

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 4
                            spacing: 6

                            Text {
                                text: "e:" + modelData.emitterId
                                color: mainLayout.selectedEmitterId === modelData.emitterId ? "#000" : "#FFA500"
                                font.pixelSize: 11
                                font.family: "Courier New"
                                Layout.preferredWidth: 36
                            }

                            Text {
                                text: "file:" + modelData.soundFileId
                                color: mainLayout.selectedEmitterId === modelData.emitterId ? "#000" : "#aaa"
                                font.pixelSize: 11
                                font.family: "Courier New"
                                Layout.preferredWidth: 48
                            }

                            Text {
                                text: modelData.state
                                color: {
                                    if (mainLayout.selectedEmitterId === modelData.emitterId) return "#000"
                                    if (modelData.state === "Playing") return "#4caf50"
                                    if (modelData.state === "Paused")  return "#ff9800"
                                    return "#f44336"
                                }
                                font.pixelSize: 11
                                Layout.fillWidth: true
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: mainLayout.selectedEmitterId = modelData.emitterId
                        }
                    }
                }
            }

            // spawn emitter from selected sound
            MenuButton {
                buttonText: "▶ Play Selected Sound"
                Layout.fillWidth: true
                enabled: mainLayout.selectedSoundFileId !== -1
                onClicked: {
                    let emitterId = communicator.playFromFileId(mainLayout.selectedSoundFileId)
                    mainLayout.activeEmitters = mainLayout.activeEmitters.concat([{
                        emitterId: emitterId,
                        soundFileId: mainLayout.selectedSoundFileId,
                        state: "Paused"
                    }])
                    mainLayout.selectedEmitterId = emitterId
                }
            }

            // per-emitter controls
            ColumnLayout  {
                Layout.fillWidth: true
                spacing: 4

                MenuButton {
                    buttonText: "Resume"
                    Layout.fillWidth: true
                    enabled: mainLayout.selectedEmitterId !== -1
                    onClicked: {
                        communicator.resumeEmitter(mainLayout.selectedEmitterId),
                        updateEmitterState(mainLayout.selectedEmitterId, "Playing")
                    }
                }
                MenuButton {
                    buttonText: "Pause"
                    Layout.fillWidth: true
                    enabled: mainLayout.selectedEmitterId !== -1
                    onClicked: { 
                        communicator.pauseEmitter(mainLayout.selectedEmitterId)
                        updateEmitterState(mainLayout.selectedEmitterId, "Paused")
                    }
                }
                MenuButton {
                    buttonText: "Stop"
                    Layout.fillWidth: true
                    enabled: mainLayout.selectedEmitterId !== -1
                    onClicked: { 
                        communicator.stopEmitter(mainLayout.selectedEmitterId)
                        removeEmitter(mainLayout.selectedEmitterId)
                    }    
                }
            }

            // global controls
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                MenuButton {
                    buttonText: "Reset Emitters"
                    Layout.fillWidth: true
                    onClicked: {
                        communicator.pauseAll()
                        communicator.resetEmitters()
                        mainLayout.activeEmitters = []
                        mainLayout.selectedEmitterId = -1
                    }
                }
                MenuButton {
                    buttonText: "Resume All"
                    Layout.fillWidth: true
                    onClicked: communicator.resumeAll()
                }
                MenuButton {
                    buttonText: "Pause All"
                    Layout.fillWidth: true
                    onClicked: communicator.pauseAll()
                }
            }
        }

        // separator
        Rectangle { width: 1; Layout.fillHeight: true; color: "#555"; Layout.topMargin: 8; Layout.bottomMargin: 8 }

        // ════════════════════════════════════════════════════════
        // COLUMN 3 — Spatial Controls
        // ════════════════════════════════════════════════════════
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.topMargin: 8
            Layout.bottomMargin: 8
            spacing: 4

            Text {
                text: "SPATIAL"
                font.pixelSize: 13
                font.letterSpacing: 3
                color: "#888"
            }

            Rectangle { height: 1; Layout.fillWidth: true; color: "#FFA500" }

            // azimuth
            RowLayout {
                Layout.fillWidth: true
                Text { text: "Azimuth"; font.pixelSize: 13; color: "#ccc"; Layout.preferredWidth: 80 }
                Text { text: azimuthSlider.value.toFixed(1) + "°"; font.pixelSize: 13; color: "#FFA500"; font.family: "Courier New"; Layout.preferredWidth: 60 }
            }

            OptionSlider {
                id: azimuthSlider
                Layout.fillWidth: true
                from: -180; initialValue: 0; to: 180; stepSize: 0.5
                onValueChanged: communicator.updateTestSlider(mainLayout.selectedEmitterId, value, elevationSlider.value)
            }

            MenuButton {
                buttonText: mainLayout.isAzimuthRotating ? "⏹ Stop Azimuth Rotation" : "↺ Rotate Azimuth"
                Layout.fillWidth: true
                onClicked: {
                    mainLayout.isAzimuthRotating = !mainLayout.isAzimuthRotating
                    mainLayout.isAzimuthRotating ? rotationAzimuth.start() : rotationAzimuth.stop()
                }
            }

            Item { Layout.preferredHeight: 8 }

            // elevation
            RowLayout {
                Layout.fillWidth: true
                Text { text: "Elevation"; font.pixelSize: 13; color: "#ccc"; Layout.preferredWidth: 80 }
                Text { text: elevationSlider.value.toFixed(1) + "°"; font.pixelSize: 13; color: "#FFA500"; font.family: "Courier New"; Layout.preferredWidth: 60 }
            }

            OptionSlider {
                id: elevationSlider
                Layout.fillWidth: true
                from: -90; initialValue: 0; to: 90; stepSize: 0.5
                onValueChanged: communicator.updateTestSlider(mainLayout.selectedEmitterId, azimuthSlider.value, value)
            }

            MenuButton {
                buttonText: mainLayout.isElevationRotating ? "⏹ Stop Elevation Rotation" : "↑↓ Rotate Elevation"
                Layout.fillWidth: true
                onClicked: {
                    mainLayout.isElevationRotating = !mainLayout.isElevationRotating
                    mainLayout.isElevationRotating ? rotationElevation.start() : rotationElevation.stop()
                }
            }

            Item { Layout.fillHeight: true }

            MenuButton {
                buttonText: "← Home"
                Layout.fillWidth: true
                onClicked: {
                    communicator.pauseAll()
                    communicator.resetEmitters()
                    communicator.unloadRegistry()
                    communicator.stoptest("")
                    stackView.pop()
                }
            }
        }
    }
}
