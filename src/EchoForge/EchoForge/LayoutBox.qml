import QtQuick 2.15
import QtQuick.Layouts 1.15

ColumnLayout {
    id: root

    property string title: "Section"
    property int contentSpacing:  8
    property int contentMargins: 10

    default property alias content: contentContainer.data

    Layout.fillWidth: true
    Layout.fillHeight: true
    implicitWidth: 200
    implicitHeight: headerRow.implicitHeight + contentContainer.implicitHeight + contentMargins * 2 + headerRow.height + 8

    Rectangle {
        id: headerStrip
        anchors { top: parent.top; left: parent.left; right: parent.right }
        height: 26

        RowLayout {
            id: headerRow
            anchors {
                fill: parent
                leftMargin: 10
                rightMargin: 6
            }
            spacing: 0

            Text {
                text: root.title.toUpperCase()
                color: "#8080A8"
                font {
                    pixelSize: 10
                    letterSpacing: 1.8
                    weight: Font.Medium
                }
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    Item {
        id: contentContainer
        anchors {
            top: headerStrip.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: root.contentMargins
        }
    }
}
