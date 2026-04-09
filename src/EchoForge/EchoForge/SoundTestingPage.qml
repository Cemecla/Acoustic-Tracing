import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MainLayout {
    maintitle: "Sound Testing"
    anchors.fill: parent

    ColumnLayout {
        width: 200
        Layout.fillHeight: true
        Layout.leftMargin: 20
        Layout.bottomMargin: 20

        Rectangle {
            Layout.fillHeight: true
            color: "transparent"
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
            buttonText: "Home"
            onClicked: {
                communicator.stoptest("../../../../Acoustic-Tracing/assets/mooo.wav");
                stackView.pop();
            }
        }

        MenuButton {
            buttonText: "Play"
            onClicked: communicator.playtest("../../../../Acoustic-Tracing/assets/mooo.wav");
        }

        MenuButton {
            buttonText: "Stop"
            onClicked: communicator.stoptest("../../../../Acoustic-Tracing/assets/mooo.wav");
        }
    }
}
