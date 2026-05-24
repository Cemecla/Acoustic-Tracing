import QtQuick
import QtQuick.Layouts

Item {
    id: radar
    implicitWidth: 280
    implicitHeight: 280

    property var emitters: []
    property int selectedId: -1
    signal emitterClicked(int emitterId)

    property color bgColor: "#1a1a1a"
    property color gridColor: "#333333"
    property color axisColor: "#555555"
    property color accentColor: mainWindow.secondaryColor
    property color avatarColor: "#d4d4d4"
    property color emitterColor: "#7FFF00"
    property string fontFamily: "Consolas"

    readonly property real cx: width / 2
    readonly property real cy: height / 2
    readonly property real ringRadius: Math.min(width, height) / 2 - 24
    readonly property real emitterRingRadius: ringRadius * 0.78

    // BACKGROUND
    Rectangle {
        anchors.fill: parent
        color: radar.bgColor
        radius: 4
        border.color: "#444444"
        border.width: 1
    }


    // RADAR CIRCLE
    Repeater {
        model: 3
        Rectangle {
            property real r: radar.ringRadius * ((index + 1) / 3)
            x: radar.cx - r
            y: radar.cy - r
            width: r * 2
            height: r * 2
            radius: r
            color: "transparent"
            border.color: radar.gridColor
            border.width: 1
        }
    }

    // CROSS
    Rectangle {
        x: radar.cx - radar.ringRadius
        y: radar.cy - 0.5
        width: radar.ringRadius * 2
        height: 1
        color: radar.axisColor
    }

    Rectangle {
        x: radar.cx - 0.5
        y: radar.cy - radar.ringRadius
        width: 1
        height: radar.ringRadius * 2
        color: radar.axisColor
    }

    // ROSE
    Text {
        text: "N"
        color: radar.accentColor
        font.family: radar.fontFamily
        font.pixelSize: 10
        font.bold: true
        x: radar.cx - width / 2
        y: radar.cy - radar.ringRadius - height - 2
    }

    Text {
        text: "S"
        color: radar.accentColor
        font.family: radar.fontFamily
        font.pixelSize: 10
        font.bold: true
        x: radar.cx - width / 2
        y: radar.cy + radar.ringRadius + 2
    }

    Text {
        text: "O"
        color: radar.accentColor
        font.family: radar.fontFamily
        font.pixelSize: 10
        font.bold: true
        x: radar.cx - radar.ringRadius - width - 4
        y: radar.cy - height / 2
    }

    Text {
        text: "E"
        color: radar.accentColor
        font.family: radar.fontFamily
        font.pixelSize: 10
        font.bold: true
        x: radar.cx + radar.ringRadius + 4
        y: radar.cy - height / 2
    }

    // Avatar
    Item {
        id: avatar
        x: radar.cx
        y: radar.cy
        z: 1

        Rectangle {
            width: 14; 
            height: 14; 
            radius: 7
            x: -7; 
            y: -7
            color: radar.avatarColor
            border.color: "#000000"
            border.width: 1
        }


        Rectangle {
            width: 2; 
            height: 10
            x: -1; 
            y: -16
            color: radar.avatarColor
        }
    }

    // EMITTERS
    Repeater {
        id: emitterRepeater
        model: radar.emitters

        delegate: Item {
            id: marker
            z: 2

            readonly property real azDeg: (modelData.azimuth !== undefined) ? modelData.azimuth : 0
            readonly property bool isSelected: modelData.emitterId === radar.selectedId
            readonly property string state: modelData.state || ""
            readonly property real azRad: marker.azDeg * Math.PI / 180

            x: radar.cx + radar.emitterRingRadius * Math.sin(azRad)
            y: radar.cy - radar.emitterRingRadius * Math.cos(azRad)

            Behavior on x { NumberAnimation { duration: 80; easing.type: Easing.OutQuad } }
            Behavior on y { NumberAnimation { duration: 80; easing.type: Easing.OutQuad } }

            // MARKER
            Rectangle {
                id: dot
                width: isSelected ? 16 : 12
                height: width
                radius: width / 2
                x: -width / 2
                y: -height / 2
                color: {
                    if (marker.state == "Playing") return radar.emitterColor
                    return "#ff0000"
                }

                border.color: marker.isSelected ? "#ffffff" : "#000000"
                border.width: marker.isSelected ? 2 : 1

                Behavior on width { NumberAnimation { duration: 90 } }
                Behavior on height { NumberAnimation { duration: 90 } }
                Behavior on color { ColorAnimation { duration: 90 } }
            }

            // LABEL
            Text {
                text: "e" + modelData.emitterId
                color: marker.isSelected ? "#ffffff" : "#cccccc"
                font.family: radar.fontFamily
                font.pixelSize: 10
                font.bold: marker.isSelected
                x: 8 * Math.sin(marker.azRad) - width / 2
                y: -8 * Math.cos(marker.azRad) - height / 2 + (marker.azDeg > -90 && marker.azDeg < 90 ? -14 : 14)
            }

            MouseArea {
                anchors.centerIn: parent
                width: 28; height: 28
                onClicked: radar.emitterClicked(modelData.emitterId)
            }
        }
    }
}

