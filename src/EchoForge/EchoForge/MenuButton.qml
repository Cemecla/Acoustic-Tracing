import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button { 
    property alias buttonText: menuButton.text

    id: menuButton
    font.pointSize: 24
    Layout.preferredWidth: parent.width
    
    background: Rectangle {
        color: menuButton.hovered ? "#000000" : "transparent"
    }
    
    contentItem: Text {
        text: menuButton.text
        font: menuButton.font
        opacity: enabled ? 1.0 : 0.3
        color: menuButton.hovered ? "#FFFFFF" : "#000000"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}
