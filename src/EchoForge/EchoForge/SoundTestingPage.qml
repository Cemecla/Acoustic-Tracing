import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


MainLayout {
    id: mainLayout
    maintitle: "Sound Testing"
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

        OptionSlider {
            id: elevationSlider
            from: -90
            initialValue: 0
            to: 90
            stepSize: 0.5

            onValueChanged: {
                communicator.updateTestSlider(azimuthSlider.value, value);
            }
        }


        Text {
            text: `Azimuth: ${azimuthSlider.value}`;
        }
       
        OptionSlider {
            id: azimuthSlider
            from: -180
            initialValue: 0
            to: 180
            stepSize: 0.5

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
