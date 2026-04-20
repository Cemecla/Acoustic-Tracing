import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MainLayout {
    maintitle: "Credit"
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 50

        Text {
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Qt.AlignHCenter
            Layout.fillWidth: true
            text: "Developpers"
            font.pixelSize: 28
            font.italic: true
        }

        Repeater {
            model: ["Joel Mailly-Niquay", "Yuta Landreville", "Jacques Michel Caron"]

            Text {
                required property string modelData
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Qt.AlignHCenter
                Layout.fillWidth: true
                text: modelData
                font.pixelSize: 22
            }
        }

        Rectangle {
            Layout.fillHeight: true
            color: "transparent"
        }
    }
}
