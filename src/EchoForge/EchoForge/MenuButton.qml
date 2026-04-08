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

    background: Item {
        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: menuButton.hovered ? 0.15 : 0
            Behavior on opacity {
                NumberAnimation { duration: 150 }
            }
        }

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: menuButton.hovered ? 5 : 0
            color: "#FFA500"
            opacity: menuButton.hovered ? 1 : 0

            Behavior on opacity {
                NumberAnimation { duration: 150 }
            }

            Behavior on width {
                NumberAnimation { duration: 150 }
            }
        }

    }
    
    contentItem: Text {
        text: menuButton.text
        font: menuButton.font
        opacity: enabled ? 1.0 : 0.3
        color: menuButton.hovered ? "#FFFFFF" : "#000000"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        anchors.leftMargin: menuButton.hovered ? 15 : 0

        Behavior on anchors.leftMargin {
            NumberAnimation { duration: 120 }
        }
    }
}