import QtQuick
import QtQuick3D 6.5
import QtQuick3D.Helpers

View3D {
    property int baseSize: 100
    property bool debugEnable: true
    id: view3DMain
    anchors.fill: parent
    camera: topCamera

    environment: SceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {
                sunEnergy: 60.0
                textureQuality: ProceduralSkyTextureData.SkyTextureQualityLow
            }
        }
    }

    AxisHelper {
        visible: view3DMain.debugEnable
    }

    WasdController {
        controlledObject: topCamera
    }

    PerspectiveCamera {
        id: topCamera
        position: Qt.vector3d(0, 600, 0)
        eulerRotation: Qt.vector3d(-90, 0, 0)
        clipNear: 5
        clipFar: 1000000
        frustumCullingEnabled: false
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
        source: "#Cube"
        // pickable: true
        instancing: communicator.voxelInstances

        materials: CustomMaterial {
            vertexShader: "qrc:/shaders/voxel.vert"
            fragmentShader: "qrc:/shaders/voxel.frag"
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
