import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// TODO : CLEAN
Item {
    id: root

    property var bands: [4000, 6000, 8000, 10000, 12000, 14000, 16000]

    property real minValue: -10
    property real maxValue: 12
    property real stepSize: 0.5

    property var values: [0, 0, 0, 0, 0, 0, 0]

    signal valueChanged(int hz, real value)

    function setValues(newValues) {
        if (!newValues) return
        let next = []
        for (let i = 0; i < bands.length; ++i) {
            let raw = i < newValues.length ? newValues[i] : 0
            next.push(Number(raw))
        }
        root.values = next
    }

    Layout.fillWidth: true
    implicitWidth: bands.length * 60
    implicitHeight: 240

    RowLayout {
        anchors.fill: parent
        spacing: 4

        Repeater {
            id: sliderRepeater
            model: root.bands

            delegate: ColumnLayout {
                id: sliderDelegate
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10

                readonly property real sliderValue:
                    index < root.values.length ? Number(root.values[index]) : 0

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text:  {
                        let v = theSlider.value
                        return (v >= 0 ? "+" : "") + v.toFixed(1)
                    }
                    color: {
                        let v = theSlider.value
                        if (v > 0)  return "#3CB371"
                        if (v < 0)  return "#F06C6C"
                        return "#808080"
                    }
                    font.pixelSize: 10
                }

                Slider {
                    id: theSlider
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignHCenter

                    value: sliderDelegate.sliderValue

                    onMoved: {
                        if (value === root.values[index]) return
                        let copy = root.values.slice()
                        copy[index] = value
                        root.values = copy
                        root.valueChanged(root.bands[index], value)
                    }

                    orientation: Qt.Vertical
                    from: root.minValue
                    to: root.maxValue
                    stepSize: root.stepSize

                    background: Rectangle {
                        x: theSlider.leftPadding + theSlider.availableWidth / 2 - width / 2
                        y: theSlider.topPadding
                        width: 4
                        height: theSlider.availableHeight
                        color: "#2A2A45"
                        radius: 2

                        Rectangle {
                            property real zeroY: {
                                let range = root.maxValue - root.minValue
                                let ratio = (root.maxValue - 0) / range
                                return parent.height * ratio
                            }
                            property real handleY: {
                                let range = root.maxValue - root.minValue
                                let ratio = (root.maxValue - theSlider.value) / range
                                return parent.height * ratio
                            }

                            y: Math.min(zeroY, handleY)
                            width: parent.width
                            height: Math.abs(zeroY - handleY)
                            color: theSlider.value >= 0 ? "#3CB371" : "#F06C6C"
                            radius: 2
                        }

                        Rectangle {
                            property real zeroRatio: (root.maxValue - 0) / (root.maxValue - root.minValue)
                            y: parent.height * zeroRatio - 1
                            width: 10
                            height: 1
                            x: -3
                            color: "#FFFFFF"
                        }
                    }

                    handle: Rectangle {
                        x: theSlider.leftPadding + theSlider.availableWidth / 2 - width / 2
                        y: theSlider.topPadding + theSlider.visualPosition * theSlider.availableHeight - height / 2
                        width: 10
                        height: 10
                        radius: 9
                        color: "#000000"
                        border.color: "#FFFFFF"
                        border.width: 1
                    }
                }

                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: {
                        let hz = modelData
                        return hz >= 1000 ? (hz / 1000) + "k" : hz
                    }
                    color: "#FFFFFF"
                    font.pixelSize: 10
                }
            }
        }
    }
}
