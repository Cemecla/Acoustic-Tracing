import QtQuick
import QtQuick.Window
import QtQuick.Controls


Window {
    id: mainWindow
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: "Echo Forge"

    color: "#d4d4d4"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: SplashPage {}
    }
}
