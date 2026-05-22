import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    property int preferedImageSize: 400
    
    // BACKGROUND
    background : Rectangle {
        color: "#d4d4d4"
    }

    // CONTENTS
    ColumnLayout {
        anchors.fill : parent
        spacing: 20

        Spacer {}

        RowLayout {
            spacing: 50
            Layout.fillWidth: true
            Layout.fillHeight: true

            Spacer {
                Layout.fillHeight: false
            }

            Image {
                id: logo1
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter 
                source: "qrc:/images/logoSound"
                Layout.preferredHeight: root.preferedImageSize
                Layout.preferredWidth: root.preferedImageSize
                fillMode: Image.PreserveAspectFit
                opacity: 0.0
            }

            Image {
                id: logo2
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter 
                source: "qrc:/images/logoRaytracing"
                Layout.preferredHeight: root.preferedImageSize
                Layout.preferredWidth: root.preferedImageSize
                fillMode: Image.PreserveAspectFit
                opacity: 0.0
            }

            Image {
                id: logo3
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter 
                source: "qrc:/images/logo"
                Layout.preferredHeight: root.preferedImageSize
                Layout.preferredWidth: root.preferedImageSize
                fillMode: Image.PreserveAspectFit
                opacity: 0.0
            }

            Spacer {
                Layout.fillHeight: false
            }
        }

        Text {
            id: maintitle
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "ECHO FORGE"
            font.pointSize: 48
            opacity: 0.0
        }

        Spacer {}
    }
 
    // ANIMATIONS
    SequentialAnimation {
        running: true
        loops: 1

        NumberAnimation { 
            target: maintitle
            property: "opacity"
            to: 1.0;
            duration: 800;
        }

        NumberAnimation { 
            target: logo1
            property: "opacity"
            to: 1.0;
            duration: 800;
        }

        NumberAnimation { 
            target: logo2
            property: "opacity"
            to: 1.0;
            duration: 800;
        }

        NumberAnimation { 
            target: logo3
            property: "opacity"
            to: 1.0;
            duration: 800;
        }

        onFinished: {
            pageTimer.start()
        }
    }
    
    // TIMER
    Timer { 
        id: pageTimer
        interval: 1000; 
        running: false;
        onTriggered: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/MenuPage.qml"))
    }
}
