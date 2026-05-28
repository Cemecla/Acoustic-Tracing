import QtQuick

Rectangle {
    id: root
    property color themeColor: mainWindow.secondaryColor
    property string label: ""
    property string checkedText: ""
    property string uncheckedText: ""
    property string fontFamily: "Consolas"
    property bool checked: false
    signal toggled()

    height: 32
    color: mouseArea.containsMouse ? "#1f2428" : "transparent"
    border.color: root.checked ? root.themeColor : "#5a5a5a"
    border.width: 1
    radius: 2
    opacity: root.checked ? 1.0 : 0.6

    Row {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 8

        // Indicator dot
        Rectangle {
            width: 10; 
            height: 10; 
            radius: 5
            anchors.verticalCenter: parent.verticalCenter
            color: root.checked ? root.themeColor : "transparent"
            border.color: root.themeColor
            border.width: 1
        }

        Text {
            text: root.label
            color: "#cccccc"
            font.family: root.fontFamily
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }

        Item { width: 2; height: 1 }

        Text {
            text: root.checked ? root.checkedText : root.uncheckedText
            color: root.themeColor
            font.family: root.fontFamily
            font.pixelSize: 10
            font.letterSpacing: 1
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: root.checked ? Qt.ArrowCursor : Qt.PointingHandCursor
        onClicked: root.toggled()
    }
}