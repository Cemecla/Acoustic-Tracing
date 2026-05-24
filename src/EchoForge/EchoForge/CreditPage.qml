import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MainLayout {
    id: creditPage
    maintitle: "Credit"

    property var creditSections: [
        {
            title: "Developers",
            content: ["Jacques Michel Caron", "Joel Mailly-Niquay", "Yuta Landreville"]
        },
        {
            title: "Assets",
            content: ["Kenney.nl - Voxels Texture"]
        }
    ]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        HorizontalSeparator {}

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            contentWidth: availableWidth

            ColumnLayout {
                width: parent.width
                spacing: 40

                Repeater {
                    model: creditPage.creditSections

                    ColumnLayout {
                        required property var modelData
                        Layout.fillWidth: true
                        Layout.topMargin: 20
                        spacing: 15

                        ColumnLayout {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: 6

                            Text {
                                Layout.alignment: Qt.AlignHCenter
                                text: modelData.title
                                font.pixelSize: 28
                                font.italic: true
                                color: "#000000"
                            }

                            Rectangle {
                                Layout.alignment: Qt.AlignHCenter
                                Layout.preferredWidth: 60
                                Layout.preferredHeight: 3
                                color: mainWindow.secondaryColor
                            }
                        }

                        Repeater {
                            model: modelData.content

                            Text {
                                required property string modelData
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignHCenter
                                text: modelData
                                font.pixelSize: 22
                                color: "#333333"
                            }
                        }
                    }
                }
            }
        }

        HorizontalSeparator {}

        MenuButton {
            buttonText: "← Return"
            Layout.fillWidth: true
            onClicked: stackView.pop()
        }
    }
}