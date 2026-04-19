import QtQuick
import QtQuick.Controls

Slider {
	id: sliderRoot
	property int initialValue: 50
	from: 0
    to: 100
    stepSize: 1
    implicitWidth: 200
    implicitHeight: 28

    // https://doc.qt.io/qt-6/qtquickcontrols-customize.html#customizing-slider
	handle: Rectangle {
        x: sliderRoot.leftPadding + sliderRoot.visualPosition * (sliderRoot.availableWidth - width)
        y: sliderRoot.topPadding + sliderRoot.availableHeight / 2 - height / 2
        implicitWidth: 10
        implicitHeight: 10
        radius: 10
        color: sliderRoot.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }

	Component.onCompleted: sliderRoot.value = sliderRoot.initialValue
}