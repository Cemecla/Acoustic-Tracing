import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MainLayout {
    maintitle: "ECHO FORGE"

    // SETUP PINNA
    Component.onCompleted: {
        communicator.setSelectedPinnaId(appSettings.pinaId)
    }

    ColumnLayout {
        width: 200
        Layout.fillHeight: true
        Layout.leftMargin: 20
        Layout.bottomMargin: 20

        Rectangle {
            Layout.fillHeight: true
            color: "transparent"
        }

        MenuButton {
            buttonText: "Simulation" 
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/SimulationSelectionPage.qml"), { debugMode: false })
        }
        
        MenuButton { 
            buttonText: "Dev Mode"
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/SimulationSelectionPage.qml"), { debugMode: true })
        }

        MenuButton {
            buttonText: "Sound Lab" 
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/SoundTestingPage.qml"))
        }
        
        MenuButton { 
            buttonText: "Options"
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/OptionPage.qml"))
        }

        MenuButton { 
            buttonText: "Credit"
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/CreditPage.qml"))
        }
        
        MenuButton { 
            buttonText: "Quit"
            onClicked: Qt.quit()
        }
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "transparent"
    }

    ColumnLayout {
        width: 100
        Layout.fillWidth: false
        Layout.fillHeight: true

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
        }
        
        Image {
            source: "qrc:/images/github"
            Layout.preferredWidth: 100
            Layout.preferredHeight: 100
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    Qt.openUrlExternally("https://github.com/Cemecla/Acoustic-Tracing")
                }
            }
        }
    }
}
