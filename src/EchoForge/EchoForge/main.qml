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

    Keys.onPressed: (event) => { if ( event.key === Qt.Key_F1 ) mainWindow.showFullScreen() }  

    Shortcut {
        sequences: Qt.Key_F1
        onActivated: mainWindow.showFullScreen()
    } 

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: SplashPage {}
    }
}
