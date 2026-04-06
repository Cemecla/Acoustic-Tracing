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
    }
}
