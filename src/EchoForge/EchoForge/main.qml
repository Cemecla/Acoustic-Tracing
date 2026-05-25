import QtQuick
import QtQuick.Window
import QtCore
import QtQuick.Controls


Window {
    id: mainWindow
    property color secondaryColor: appSettings.secondaryColor
    visible: true
    minimumWidth: 800
    minimumHeight: 600
    title: "Echo Forge"

    Settings {
        id: appSettings
        property string screenSize: "Windowed"
        property string graphicQuality: "Low"
        property double sfxVolume: 1.0
        property int pinaId: 14
        property color secondaryColor: "#FFA500"
        property color avatarColor: "grey"
    }
    
    visibility: appSettings.screenSize == "Windowed" ? Window.Windowed : Window.FullScreen

    color: "#d4d4d4"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: SplashPage {}
    }
}
