import QtQuick


Item {
    id: ripple
    clip: true

    property color rippleColor: "red"
    property int rippleDuration: 450
    property real maxRadius: 60
    property bool clickThrough: false

    Rectangle {
        id: wave
        width: 0
        height: 0
        radius: width / 2
        color: ripple.rippleColor
        opacity: 0
        visible: opacity > 0

        property real cx: 0
        property real cy: 0
        x: cx - width / 2
        y: cy - height / 2

        ParallelAnimation {
            id: rippleAnim

            NumberAnimation {
                target: wave
                properties: "width,height"
                from: 0
                to: ripple.maxRadius * 2
                duration: ripple.rippleDuration
                easing.type: Easing.OutQuad
            }

            NumberAnimation {
                target: wave
                property: "opacity"
                from: 0.6
                to: 0
                duration: ripple.rippleDuration
                easing.type: Easing.OutQuad
            }
        }
    }

    MouseArea {
        id: tapArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        propagateComposedEvents: ripple.clickThrough
        onPressed: (mouse) => {
            wave.cx = mouse.x
            wave.cy = mouse.y
            rippleAnim.restart()
            mouse.accepted = !ripple.clickThrough
        }
    }
}
