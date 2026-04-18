import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


MainLayout {
    id: mainLayout
    maintitle: "Sound Testing"
    anchors.fill: parent
    property string filepath: ""
    property bool isAzimuthRotating: false
    property bool isElevationRotating: false

    ColumnLayout {
        width: 500
        Layout.fillHeight: true
        Layout.leftMargin: 20
        Layout.bottomMargin: 20

        Rectangle {
            Layout.fillHeight: true
            color: "transparent"
        }

        FileDialog {
            id: fileDialog
            fileMode : FileDialog.OpenFile
            nameFilters: ["Sound files (*.mp3 *.wav)"]
            onAccepted: {
                let path = selectedFile.toString();
                
                let cleanPath = path.replace(/^(file:\/{3})/, "");

                console.log("User selected path:", decodeURIComponent(cleanPath));
                mainLayout.filepath = cleanPath
            }
        }

        Text {
            text: {mainLayout.filepath == "" ? "No file selected" : mainLayout.filepath}
        }

        MenuButton {
            buttonText: "Select"
            onClicked: {
                fileDialog.open()
            }
        }

        Text {
            text: `Elevation: ${elevationSlider.value}`;
        }

        Slider {
            id: elevationSlider
            from: -90
            value: 0
            to: 90
            stepSize: 0.5

            // https://doc.qt.io/qt-6/qtquickcontrols-customize.html#customizing-slider
            handle: Rectangle {
                x: elevationSlider.leftPadding + elevationSlider.visualPosition * (elevationSlider.availableWidth - width)
                y: elevationSlider.topPadding + elevationSlider.availableHeight / 2 - height / 2
                implicitWidth: 10
                implicitHeight: 10
                radius: 10
                color: elevationSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                border.color: "#bdbebf"
            }

            onValueChanged: {
                communicator.updateTestSlider(azimuthSlider.value, value);
            }
        }


        Text {
            text: `Azimuth: ${azimuthSlider.value}`;
        }
        
        Slider {
            id: azimuthSlider
            from: -180
            value: 0
            to: 180
            stepSize: 0.5

            
            // https://doc.qt.io/qt-6/qtquickcontrols-customize.html#customizing-slider
            handle: Rectangle {
                x: azimuthSlider.leftPadding + azimuthSlider.visualPosition * (azimuthSlider.availableWidth - width)
                y: azimuthSlider.topPadding + azimuthSlider.availableHeight / 2 - height / 2
                implicitWidth: 10
                implicitHeight: 10
                radius: 10
                color: azimuthSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                border.color: "#bdbebf"
            }

            onValueChanged: {
                communicator.updateTestSlider(value, elevationSlider.value);
            }
        }

        MenuButton {
            buttonText: "Play"
            onClicked: communicator.playtest(mainLayout.filepath);
        }

        MenuButton {
            buttonText: mainLayout.isAzimuthRotating ? "Stop Azimuth Rotation" : "Rotate Azimuth"

            onClicked: {
                mainLayout.isAzimuthRotating = !mainLayout.isAzimuthRotating
                if (mainLayout.isAzimuthRotating)
                    rotationAzimuth.start()
                else
                    rotationAzimuth.stop()
            }
        }

        NumberAnimation {
            id: rotationAzimuth
            target: azimuthSlider
            property: "value"
            from: -180
            to: 180
            duration: 4000
            loops: Animation.Infinite
            running: false
        }

        MenuButton {
            buttonText: mainLayout.isElevationRotating ? "Stop Elevation Rotation" : "Rotate Elevation"

            onClicked: {
                mainLayout.isElevationRotating = !mainLayout.isElevationRotating
                if (mainLayout.isElevationRotating)
                    rotationElevation.start()
                else
                    rotationElevation.stop()
            }
        }

        NumberAnimation {
            id: rotationElevation
            target: elevationSlider
            property: "value"
            from: -90
            to: 90
            duration: 4000
            loops: Animation.Infinite
            running: false
        }

        MenuButton {
            buttonText: "Stop"
            onClicked: communicator.stoptest(mainLayout.filepath);
        }

        MenuButton {
            buttonText: "Home"
            onClicked: {
                communicator.stoptest(mainLayout.filepath);
                stackView.pop();
            }
        }
    }
}
