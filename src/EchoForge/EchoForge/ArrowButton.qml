import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    enum Side {
        Right,
        Left
    }

    property int side: ArrowButton.Side.Left

    id: nextBtn
    font.family: "Segoe UI Symbol"
    text: side == ArrowButton.Side.Left ? "◀" : "▶"
    font.pointSize: 11
    implicitWidth: 28
    implicitHeight: 28
    background: Rectangle {
        color: nextBtn.hovered ? mainWindow.secondaryColor : "transparent"
        radius: 2
    }

    contentItem: Text {
        text: nextBtn.text
        font: nextBtn.font
        color: "#111111"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
