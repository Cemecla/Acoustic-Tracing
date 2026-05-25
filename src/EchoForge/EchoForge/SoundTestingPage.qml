import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

MainLayout {
    id: mainLayout
    maintitle: "Sound Lab"

    // STATES
    property var registrySounds: []       // [{id, path}]
    property var activeEmitters: []       // [{emitterId, soundFileId, state}]
    property int selectedEmitterId: -1
    property int selectedSoundFileId: -1
    property bool isAzimuthRotating: false
    property bool isElevationRotating: false
    property string fontFamily: "Consolas"
    
    // FUNCTION
    function updateEmitterState(emitterId, newState) {
        let updated = []

        for (let i = 0; i < mainLayout.activeEmitters.length; i++) {
            let e = mainLayout.activeEmitters[i]
            if (e.emitterId == emitterId)
                updated.push({ emitterId: e.emitterId, soundFileId: e.soundFileId, state: newState, azimuth: e.azimuth })
            else
                updated.push(e)
        }

        mainLayout.activeEmitters = updated
    }

    function updateEmitterPosition(emitterId, azimuth) {
        let updated = []

        for (let i = 0; i < mainLayout.activeEmitters.length; i++) {
            let e = mainLayout.activeEmitters[i]
            if (e.emitterId == emitterId)
                updated.push({ emitterId: e.emitterId, soundFileId: e.soundFileId, state: e.state, azimuth: azimuth })
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
        if (mainLayout.selectedEmitterId == emitterId)
            mainLayout.selectedEmitterId = -1
    }

    // FILE DIALOG
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
        id: openAmbientDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Sound files (*.mp3 *.wav)"]
        onAccepted: {
            let path = decodeURIComponent(selectedFile.toString().replace(/^(file:\/{3})/, ""))
            communicator.addAmbient(path)
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

    //  ANIMATIONS
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

    // MAIN LAYOUT
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
            id: soundPanel
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
                        id: mainTab
                        text: "Main"
                        font.pointSize: 13
                        implicitWidth: contentItem.implicitWidth + 24
                        implicitHeight: 40

                        background: Rectangle {
                            color: mainTab.checked ? "#111111" : "transparent"
                            border.color: mainTab.checked ? "transparent" : "#888888"
                            border.width: mainTab.checked ? 0 : 1
                        }

                        contentItem: Text {
                            text: mainTab.text
                            font: mainTab.font
                            color: mainTab.checked ? "#ffffff" : "#111111"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    TabButton {
                        id: pinnaTab
                        text: "Pinna"
                        font.pointSize: 13
                        implicitWidth: contentItem.implicitWidth + 24
                        implicitHeight: 40

                        background: Rectangle {
                            color: pinnaTab.checked ? "#111111" : "transparent"
                            border.color: pinnaTab.checked ? "transparent" : "#888888"
                            border.width: pinnaTab.checked ? 0 : 1
                        }

                        contentItem: Text {
                            text: pinnaTab.text
                            font: pinnaTab.font
                            color: pinnaTab.checked ? "#ffffff" : "#111111"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                HorizontalSeparator { color: "#888888" }

                // CONTENTS
                StackLayout {
                    id: stackLayout
                    Layout.fillWidth:  true
                    Layout.fillHeight: true
                    currentIndex: tabBar.currentIndex

                    // MAIN TAB
                    Item {
                        id: mainPage

                        ScrollView {
                            id: mainScroll
                            anchors.fill: parent
                            clip: true
                            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                            ScrollBar.vertical.policy: ScrollBar.AsNeeded
                            contentWidth:  mainRow.implicitWidth
                            contentHeight: mainRow.implicitHeight

                            RowLayout {
                                id: mainRow
                                width: Math.max(implicitWidth,  mainScroll.availableWidth)
                                height: Math.max(implicitHeight, mainScroll.availableHeight)
                                spacing: 0

                                // REGISTRY
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
                                        color: "#555555"
                                    }

                                    HorizontalSeparator { color: mainWindow.secondaryColor }

                                    // SOUND LIST
                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumHeight: 200
                                        color: "#1a1a1a"
                                        radius: 2

                                        ListView {
                                            id: registryList
                                            anchors.fill: parent
                                            anchors.margins: 4
                                            clip: true
                                            model: mainLayout.registrySounds
                                            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                                            delegate: Rectangle {
                                                width: registryList.width
                                                height: 40
                                                color: mainLayout.selectedSoundFileId == modelData.id
                                                       ? mainWindow.secondaryColor : (index % 2 == 0 ? "#222" : "#1e1e1e")
                                                radius: 2

                                                RowLayout {
                                                    anchors.fill: parent
                                                    anchors.leftMargin: 8
                                                    anchors.rightMargin: 4
                                                    spacing: 6

                                                    Text {
                                                        text: "id:" + modelData.id
                                                        color: mainLayout.selectedSoundFileId === modelData.id ? "#000" 
                                                        : modelData.isAmbient ? "#1ba1e2" : mainWindow.secondaryColor
                                                        font.pixelSize: 11
                                                        font.family: mainLayout.fontFamily
                                                        Layout.preferredWidth: 36
                                                    }

                                                    Text {
                                                        text: (modelData.isAmbient ? "[A] " : "") + modelData.path.split("/").pop().split("\\").pop()
                                                        color: mainLayout.selectedSoundFileId == modelData.id ? "#000" : "#ccc"
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

                                    // REGISTRY BUTTONS
                                    MenuButton {
                                        buttonText: "+ Add Sound"
                                        Layout.fillWidth: true
                                        onClicked: openSoundDialog.open()
                                    }

                                    MenuButton {
                                        buttonText: "+ Add Ambient"
                                        Layout.fillWidth: true
                                        onClicked: openAmbientDialog.open()
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

                                VerticalSeparator {
                                    color: "#888888"
                                    Layout.topMargin: 8
                                    Layout.bottomMargin: 8
                                    Layout.leftMargin: 8
                                    Layout.rightMargin: 8
                                }

                                // EMITTERS
                                ColumnLayout {
                                    Layout.preferredWidth: 320
                                    Layout.fillHeight: true
                                    Layout.topMargin: 8
                                    Layout.bottomMargin: 8
                                    spacing: 4

                                    Text {
                                        text: "EMITTERS"
                                        font.pixelSize: 13
                                        font.letterSpacing: 3
                                        color: "#555555"
                                    }

                                    HorizontalSeparator { color: mainWindow.secondaryColor }

                                    // EMITTERS LIST
                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        Layout.minimumHeight: 200
                                        color: "#1a1a1a"
                                        radius: 2

                                        ListView {
                                            id: emitterList
                                            anchors.fill: parent
                                            anchors.margins: 4
                                            clip: true
                                            model: mainLayout.activeEmitters
                                            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                                            delegate: Rectangle {
                                                width: emitterList.width
                                                height: 40
                                                color: mainLayout.selectedEmitterId == modelData.emitterId
                                                       ? mainWindow.secondaryColor : (index % 2 == 0 ? "#222" : "#1e1e1e")
                                                radius: 2

                                                RowLayout {
                                                    anchors.fill: parent
                                                    anchors.leftMargin: 8
                                                    anchors.rightMargin: 4
                                                    spacing: 6

                                                    Text {
                                                        text: "e:" + modelData.emitterId
                                                        color: mainLayout.selectedEmitterId == modelData.emitterId ? "#000" : mainWindow.secondaryColor
                                                        font.pixelSize: 11
                                                        font.family: mainLayout.fontFamily
                                                        Layout.preferredWidth: 36
                                                    }

                                                    Text {
                                                        text: "file:" + modelData.soundFileId
                                                        color: mainLayout.selectedEmitterId == modelData.emitterId ? "#000" : "#aaa"
                                                        font.pixelSize: 11
                                                        font.family: mainLayout.fontFamily
                                                        Layout.preferredWidth: 48
                                                    }

                                                    Text {
                                                        text: modelData.state
                                                        color: {
                                                            if (mainLayout.selectedEmitterId == modelData.emitterId) return "#000"
                                                            if (modelData.state == "Playing") return "#4caf50"
                                                            if (modelData.state == "Paused")  return "#ff9800"
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

                                    // ADD SOUNDS
                                    MenuButton {
                                        font.family: "Segoe UI Symbol"
                                        buttonText: "▶ Add Selected Sound"
                                        Layout.fillWidth: true
                                        enabled: mainLayout.selectedSoundFileId != -1
                                        onClicked: {
                                            let emitterId = communicator.playFromFileId(mainLayout.selectedSoundFileId)
                                            mainLayout.activeEmitters = mainLayout.activeEmitters.concat([{
                                                emitterId: emitterId,
                                                soundFileId: mainLayout.selectedSoundFileId,
                                                state: "Paused",
                                                azimuth: 0,
                                            }])
                                            mainLayout.selectedEmitterId = emitterId
                                            communicator.updateTestSlider(mainLayout.selectedEmitterId, 0.0, 0.0)
                                        }
                                    }

                                    // EMITTERS BUTTONS
                                    ColumnLayout  {
                                        Layout.fillWidth: true
                                        spacing: 4

                                        MenuButton {
                                            buttonText: "Resume"
                                            Layout.fillWidth: true
                                            enabled: mainLayout.selectedEmitterId != -1
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

                                    // ALL EMITTERS CONTROLS
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

                                VerticalSeparator {
                                    color: "#888888"
                                    Layout.topMargin: 8
                                    Layout.bottomMargin: 8
                                    Layout.leftMargin: 8
                                    Layout.rightMargin: 8
                                }

                                // SPACIAL CONTROLS
                                ColumnLayout {
                                    Layout.preferredWidth: 320
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.rightMargin: 16
                                    Layout.topMargin: 8
                                    Layout.bottomMargin: 8
                                    spacing: 4

                                    Text {
                                        text: "SPATIAL"
                                        font.pixelSize: 13
                                        font.letterSpacing: 3
                                        color: "#555555"
                                    }

                                    HorizontalSeparator { color: mainWindow.secondaryColor }

                                    EmittersRadar {
                                        id: emitterRadar
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: width
                                        Layout.maximumHeight: 320
                                        Layout.topMargin: 4
                                        Layout.bottomMargin: 4
                                        emitters: mainLayout.activeEmitters
                                        selectedId: mainLayout.selectedEmitterId
                                        onEmitterClicked: (emitterId) => mainLayout.selectedEmitterId = emitterId
                                    }

                                    // AZIMUTH
                                    RowLayout {
                                        Layout.fillWidth: true
                                        Text { text: "Azimuth"; font.pixelSize: 13; color: "#111111"; Layout.preferredWidth: 80 }
                                        Text { text: azimuthSlider.value.toFixed(1) + "°"; font.pixelSize: 13; color: "#111111"; font.family: "Courier New"; Layout.preferredWidth: 60 }
                                    }

                                    OptionSlider {
                                        id: azimuthSlider
                                        Layout.fillWidth: true
                                        from: -180; 
                                        initialValue: 0; 
                                        to: 180; 
                                        stepSize: 0.5
                                        onValueChanged: {
                                            communicator.updateTestSlider(mainLayout.selectedEmitterId, value, elevationSlider.value)
                                            mainLayout.updateEmitterPosition(mainLayout.selectedEmitterId, value)
                                        }
                                    }

                                    MenuButton {
                                        font.family: "Segoe UI Symbol"
                                        buttonText: mainLayout.isAzimuthRotating ? "⏹ Stop Azimuth Rotation" : "↺ Rotate Azimuth"
                                        Layout.fillWidth: true
                                        onClicked: {
                                            mainLayout.isAzimuthRotating = !mainLayout.isAzimuthRotating
                                            mainLayout.isAzimuthRotating ? rotationAzimuth.start() : rotationAzimuth.stop()
                                        }
                                    }

                                    Item { Layout.preferredHeight: 8 }

                                    // ELEVATION
                                    RowLayout {
                                        Layout.fillWidth: true
                                        Text { text: "Elevation"; font.pixelSize: 13; color: "#111111"; Layout.preferredWidth: 80 }
                                        Text { text: elevationSlider.value.toFixed(1) + "°"; font.pixelSize: 13; color: "#111111"; font.family: "Segoe UI Symbol"; Layout.preferredWidth: 60 }
                                    }

                                    OptionSlider {
                                        id: elevationSlider
                                        Layout.fillWidth: true
                                        from: -90; 
                                        initialValue: 0; 
                                        to: 90; 
                                        stepSize: 0.5
                                        onValueChanged: {
                                            communicator.updateTestSlider(mainLayout.selectedEmitterId, azimuthSlider.value, value)
                                            mainLayout.updateEmitterPosition(mainLayout.selectedEmitterId, azimuthSlider.value)
                                        }
                                    }

                                    MenuButton {
                                        font.family: "Segoe UI Symbol"
                                        buttonText: mainLayout.isElevationRotating ? "⏹ Stop Elevation Rotation" : "↑↓ Rotate Elevation"
                                        Layout.fillWidth: true
                                        onClicked: {
                                            mainLayout.isElevationRotating = !mainLayout.isElevationRotating
                                            mainLayout.isElevationRotating ? rotationElevation.start() : rotationElevation.stop()
                                        }
                                    }

                                    Spacer {}
                                }
                            }
                        }
                    }

                    // PINNA TAB
                    Item {
                        id: pinnaPage

                        ScrollView {
                            id: pinnaScroll
                            anchors.fill: parent
                            clip: true
                            ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                            ScrollBar.vertical.policy: ScrollBar.AsNeeded
                            contentWidth: pinnaColumnOuter.implicitWidth
                            contentHeight: pinnaColumnOuter.implicitHeight

                            ColumnLayout {
                                id: pinnaColumnOuter
                                width: Math.max(implicitWidth, pinnaScroll.availableWidth)
                                spacing: 4

                                LayoutBox {
                                    title: "Pinna"
                                    Layout.fillWidth: true
                                    Layout.leftMargin: 16
                                    Layout.rightMargin: 16
                                    Layout.topMargin: 12

                                    ColumnLayout {
                                        id: pinnaLayout
                                        Layout.fillWidth: true

                                        function applyPreset(entry) {
                                            if (!entry)
                                                return

                                            if (entry.ordered_gains && entry.ordered_gains.length > 0)
                                                freqSliders.setValues(entry.ordered_gains)
                                        }

                                        PresetSelector {
                                            id: pinnaSelector
                                            onPresetChanged: (preset) => {
                                                pinnaLayout.applyPreset(preset)
                                            }

                                            Component.onCompleted: {
                                                if (currentPreset) pinnaLayout.applyPreset(currentPreset)
                                            }
                                        }

                                        FrequencySlider {
                                            id: freqSliders
                                        }
                                    }
                                }

                                Spacer {
                                    Layout.minimumHeight: 8 
                                }
                            }
                        }
                    }
                }

                // SEPARATOR
                HorizontalSeparator { color: "#888888" }

                // RETURN
                ColumnLayout {
                    id: actionButtons
                    Layout.fillWidth: true
                    spacing: 2
                    Layout.alignment: Qt.AlignCenter

                    MenuButton {
                        id: homeButton
                        buttonText: "← Home"
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        font.pointSize: 18
                        onClicked: {
                            communicator.pauseAll()
                            communicator.resetEmitters()
                            communicator.unloadRegistry()
                            stackView.pop()
                        }
                    }
                }
            }
        }
    }
}
