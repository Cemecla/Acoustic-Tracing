import QtQuick
import QtQuick.Layouts

RowLayout {
    id: root

    property string title: "Section"
    property int contentSpacing:  8
    property int contentMargins: 10

    default property alias content: contentContainer.data

    Layout.fillWidth: true
    Layout.fillHeight: true
    implicitWidth: 200

    Item {
        id: contentContainer
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: root.contentMargins
        }
    }
}
