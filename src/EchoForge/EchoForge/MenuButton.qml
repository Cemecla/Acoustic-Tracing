import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Button { 
    property alias buttonText: menuButton.text

    id: menuButton
    font.pointSize: 24
    Layout.preferredWidth: parent.width

    onHoveredChanged: () => {
        if (hovered) {
            hoverSound.play();
        }
    }
   
    SoundEffect {
        id: hoverSound
        source: "qrc:/sounds/buttonSound"
        loops: 1
    }
    
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
