import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    width: Window.width
    height: Window.height

    background : Rectangle {
        color: "#d4d4d4"
    }

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent

        Rectangle {
            height: 100
            Layout.fillWidth: true
            Layout.topMargin: 20
            color: "transparent"

            Text {
                text: "ECHO FORGE"
                anchors.centerIn: parent
                font.pixelSize: 96
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                width: 200
                Layout.fillHeight: true
                Layout.leftMargin: 20
                Layout.bottomMargin: 20

                Rectangle {
                    Layout.fillHeight: true
                    color: "transparent"
                }

                Button { 
                    id: bScenario
                    text: "Play Scenario"
                    font.pointSize: 24
                    Layout.preferredWidth: parent.width
                    onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/SimulationPage.qml"))

                    background: Rectangle {
                        color: bScenario.hovered ? "#000000" : "transparent"
                    }

                    contentItem: Text {
                        text: bScenario.text
                        font: bScenario.font
                        opacity: enabled ? 1.0 : 0.3
                        color: bScenario.hovered ? "#FFFFFF" : "#000000"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button { 
                    id: bDev
                    text: "Dev Mode"
                    font.pointSize: 24
                    Layout.preferredWidth: parent.width
                    onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/DevMode.qml"))
                    background: Rectangle {
                        color: bDev.hovered ? "#000000" : "transparent"
                    }

                    contentItem: Text {
                        text: bDev.text
                        font: bDev.font
                        opacity: enabled ? 1.0 : 0.3
                        color: bDev.hovered ? "#FFFFFF" : "#000000"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button { 
                    id: bOptions
                    text: "Options"
                    font.pointSize: 24
                    Layout.preferredWidth: parent.width
                    onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/OptionPage.qml"))
                    background: Rectangle {
                        color: bOptions.hovered ? "#000000" : "transparent"
                    }

                    contentItem: Text {
                        text: bOptions.text
                        font: bOptions.font
                        opacity: enabled ? 1.0 : 0.3
                        color: bOptions.hovered ? "#FFFFFF" : "#000000"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    } 
                }

                Button { 
                    id: bquit
                    text: "Quit" 
                    font.pointSize: 24
                    onClicked: Qt.quit()
                    Layout.preferredWidth: parent.width

                    background: Rectangle {
                        color: bquit.hovered ? "#000000" : "transparent"
                    }

                    contentItem: Text {
                        text: bquit.text
                        font: bquit.font
                        opacity: enabled ? 1.0 : 0.3
                        color: bquit.hovered ? "#FFFFFF" : "#000000"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
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
    }
}
