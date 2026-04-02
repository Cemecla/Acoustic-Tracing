import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Page {
    anchors.fill: parent

    default property alias content: innerContainer.data
    property alias maintitle: titleText.text

    background : Rectangle {
        color: "#d4d4d4"
    }

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent

        Rectangle {
            height: 100
            Layout.fillWidth: true
            Layout.topMargin: 20
            color: "transparent"

            Text {
                id: titleText
                anchors.centerIn: parent
                font.pixelSize: 96
            }
        }

        RowLayout {
            id: innerContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
