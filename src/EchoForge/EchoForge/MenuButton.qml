import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Button { 
    property alias buttonText: btnRoot.text

    id: btnRoot
    font.pointSize: 24
    Layout.preferredWidth: parent.width

    // SOUND EFFECT
    onHoveredChanged: () => {
        if (hovered) {
            hoverSound.play();
        }
    }
   
    SoundEffect {
        id: hoverSound
        source: "qrc:/sounds/buttonSound"
        volume: appSettings.sfxVolume
        loops: 1
    }

    MouseArea {
        anchors.fill: parent
        onClicked: btnRoot.clicked()

        RippleEffect {
            anchors.fill: parent
            clickThrough: true 
            rippleColor: mainWindow.secondaryColor
        }
    }

    // ANIMATED BACKGROUND
    background: Item {
        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: btnRoot.hovered ? 0.15 : 0
            Behavior on opacity {
                NumberAnimation { duration: 150 }
            }
        }

        Rectangle {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: btnRoot.hovered ? 5 : 0
            color: mainWindow.secondaryColor
            opacity: btnRoot.hovered ? 1 : 0

            Behavior on opacity {
                NumberAnimation { duration: 150 }
            }

            Behavior on width {
                NumberAnimation { duration: 150 }
            }
        }

    }
    
    // BUTTON TEXT
    contentItem: Text {
        text: btnRoot.text
        font: btnRoot.font
        opacity: enabled ? 1.0 : 0.3
        color: btnRoot.hovered ? "#FFFFFF" : "#000000"
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        anchors.leftMargin: btnRoot.hovered ? 15 : 0

        Behavior on anchors.leftMargin {
            NumberAnimation { duration: 120 }
        }
    }
}