import QtQuick
import QtQuick3D

View3D {
    anchors.fill: parent
    camera: camera

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.Color
        clearColor: "lightblue"
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 0, 350)
        eulerRotation.x: 0
        eulerRotation.y: 0
    }

    DirectionalLight {
        eulerRotation.x: -45
        eulerRotation.y: 45
    }

    Model {
        materials: DefaultMaterial { diffuseColor: "orange" }
        source: "#Cube"
        scale: Qt.vector3d(1, 1, 1)
        position: Qt.vector3d(0, 0, 0)

        NumberAnimation on eulerRotation.y {
            from: 0
            to: 360
            duration: 4000
            loops: Animation.Infinite
        }
    }

    FocusScope {
        id: cameraFocus
        anchors.fill: parent
        focus: true

        MouseArea {
            anchors.fill: parent
            onClicked: cameraFocus.forceActiveFocus()
        }

        property bool forward: false
        property bool backward: false
        property bool leftside: false
        property bool rightside: false

        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_W) forward = true
            if (event.key === Qt.Key_S) backward = true
            if (event.key === Qt.Key_A) leftside = true
            if (event.key === Qt.Key_D) rightside = true
        }

        Keys.onReleased: (event) => {
            if (event.key === Qt.Key_W) forward = false
            if (event.key === Qt.Key_S) backward = false
            if (event.key === Qt.Key_A) leftside = false
            if (event.key === Qt.Key_D) rightside = false
        }

        Timer {
            interval: 16   // Temporary for testing will be in backend
            running: true
            repeat: true
            onTriggered: {
                var speed = 10

                if (cameraFocus.forward)  camera.position.z -= speed
                if (cameraFocus.backward) camera.position.z += speed
                if (cameraFocus.leftside) camera.position.x -= speed
                if (cameraFocus.rightside) camera.position.x += speed
            }
        }
    }
}
