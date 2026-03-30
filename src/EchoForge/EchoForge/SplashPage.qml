import QtQuick
import QtQuick.Controls

Page {
    width: Window.width
    height: Window.height

    background : Rectangle {
        color: "#d4d4d4"
    }

    Text {
        anchors.centerIn: parent
        text: "ECHO FORGE"
    }
    
    Timer { 
        interval: 10000; running: true;
        onTriggered: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/MenuPage.qml"))
    }
}
