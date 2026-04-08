import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MainLayout {
    maintitle: "ECHO FORGE"

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
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/SimulationPage.qml"))
        }
        
        MenuButton { 
            buttonText: "Dev Mode"
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/DevModePage.qml"))
        }

        MenuButton {
            buttonText: "Testing" 
            onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/SoundTestingPage.qml"))
        }
        
        MenuButton { 
            buttonText: "Option"
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
