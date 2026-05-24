import QtQuick 2.15
import QtQuick.Layouts 1.15

RowLayout {
    id: root

    property string title: "Section"
    property int contentSpacing:  8
    property int contentMargins: 10

    default property alias content: contentContainer.data

    Layout.fillWidth: true
    Layout.fillHeight: true
    implicitWidth: 200
    implicitHeight: headerRow.implicitHeight + contentContainer.implicitHeight + contentMargins * 2 + headerRow.height + 8

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
