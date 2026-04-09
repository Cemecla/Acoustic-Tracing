import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

MainLayout {
    maintitle: "Map Selection"
    anchors.fill: parent

    ScrollView {
        ListView {
            model: 20
            delegate: ItemDelegate {
                text: "Item " + index
                onClicked: stackView.push(Qt.resolvedUrl("qrc:/qt/qml/echoforge/LoadingPage.qml"))
                required property int index
            }
        }
    }
}
