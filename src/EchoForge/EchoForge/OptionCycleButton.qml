import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Row {
    id: cycleRoot
    required property var options
    property int currentIndex: 0
    property var callback: (option) => {}
    spacing: 6
    
    ArrowButton {
        side: ArrowButton.Side.Left
        onClicked: {
            cycleRoot.currentIndex = (cycleRoot.currentIndex - 1 + cycleRoot.options.length) % cycleRoot.options.length
            cycleRoot.callback(cycleRoot.options[cycleRoot.currentIndex])
        }
    }
    
    Text {
        id: cycleLabel
        anchors.verticalCenter: cycleRoot.verticalCenter
        text: cycleRoot.options[cycleRoot.currentIndex]
        font.pointSize: 13
        color: "#111111"
        width: 110
        horizontalAlignment: Text.AlignHCenter
    }
    
    ArrowButton {
        side: ArrowButton.Side.Right
        onClicked: {
            cycleRoot.currentIndex = (cycleRoot.currentIndex + 1) % cycleRoot.options.length 
            cycleRoot.callback(cycleRoot.options[cycleRoot.currentIndex])
        }
    }
}
