import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MainLayout {
    maintitle: "Sound Testing"
    ColumnLayout {
        width: 200
        Layout.fillHeight: true
        Layout.leftMargin: 20
        Layout.bottomMargin: 20

        Rectangle {
            Layout.fillHeight: true
            color: "transparent"
        }

        MenuButton {
            buttonText: "Home"
            onClicked: stackView.pop();
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
