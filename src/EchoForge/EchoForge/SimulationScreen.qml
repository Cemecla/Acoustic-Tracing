import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

View3D {
    property var baseSize: 100
    id: view3DMain
    anchors.fill: parent
    camera: camera

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.Color
        clearColor : "lightblue"
        // lightProbe: Texture {
        //     source: "qrc:/skybox/blueSky.exr"
        // }
    }

    AxisHelper {
    }

    WasdController {
        controlledObject: camera
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 600, 0)
        eulerRotation: Qt.vector3d(-90, 0, 0)
    }

    DirectionalLight {
        eulerRotation.x: -45
        eulerRotation.y: 45
    }

    Model {
        id: avatarModel
        materials: DefaultMaterial { diffuseColor: "blue" }
        source: "#Sphere"
        scale: Qt.vector3d(0.6, 0.6, 0.6)
        position: Qt.vector3d(0, 100, 0)
    }

    Model {
        materials: DefaultMaterial { diffuseColor: "orange" }
        source: "#Cube"
        scale: Qt.vector3d(1, 1, 1)
        position: Qt.vector3d(view3DMain.baseSize / 2, view3DMain.baseSize / 2, view3DMain.baseSize / 2)

        // NumberAnimation on eulerRotation.y {
        //     from: 0
        //     to: 360
        //     duration: 4000
        //     loops: Animation.Infinite
        // }
    }

    Model {
            materials: DefaultMaterial { diffuseColor: "green" }
            source: "#Cube"
            scale: Qt.vector3d(1, 1, 1)
            position: Qt.vector3d(view3DMain.baseSize + view3DMain.baseSize / 2, view3DMain.baseSize / 2, view3DMain.baseSize / 2)
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

        property bool rotateRight: false
        property bool rotateLeft: false


        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_E) rotateRight = true
            if (event.key === Qt.Key_Q) rotateLeft = true
            if (event.key === Qt.Key_W) forward = true
            if (event.key === Qt.Key_S) backward = true
            if (event.key === Qt.Key_A) leftside = true
            if (event.key === Qt.Key_D) rightside = true
        }

        Keys.onReleased: (event) => {
            if (event.key === Qt.Key_E) rotateRight = false
            if (event.key === Qt.Key_Q) rotateLeft = false
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

                if (cameraFocus.forward)  avatarModel.position.z -= speed
                if (cameraFocus.backward) avatarModel.position.z += speed
                if (cameraFocus.leftside) avatarModel.position.x -= speed
                if (cameraFocus.rightside) avatarModel.position.x += speed

                if (cameraFocus.rotateLeft) camera.eulerRotation.z -= speed - 5
                if (cameraFocus.rotateRight) camera.eulerRotation.z += speed - 5

                if (cameraFocus.rotateLeft) avatarModel.eulerRotation.z -= speed - 5
                if (cameraFocus.rotateRight) avatarModel.eulerRotation.z += speed - 5
            }
        }
    }
}
