import QtQuick
import QtQuick3D 6.11
import QtQuick3D.Helpers

View3D {
    required property bool debugEnable
    property bool isFirstPerson: false

    
    property int baseSize: 100

    id: view3DMain
    camera: debugEnable ? freeCamera : isFirstPerson ? avatarCamera : topCamera

    environment: ExtendedSceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        lensFlareEnabled: true
        lensFlareApplyDirtTexture: true
        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {
                sunEnergy: 60.0
                sunLongitude: 280.0
                textureQuality: ProceduralSkyTextureData.SkyTextureQualityLow
            }
        }

        InfiniteGrid {
            visible: view3DMain.debugEnable
            gridInterval: 100
        }
    }

    AxisHelper {
        visible: view3DMain.debugEnable
        enableXYGrid: true
    }

    WasdController {
        controlledObject: freeCamera
    }

    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_C) {
            isFirstPerson = !isFirstPerson
        }
    }

    PerspectiveCamera {
        id: freeCamera
        position: Qt.vector3d(0,0,0)
        clipNear: 5
        clipFar: 1000000
        frustumCullingEnabled: false
    }

    PerspectiveCamera {
        property var vec: Qt.vector3d(
            communicator.avatarPos.x,
            communicator.avatarPos.y + 600,
            communicator.avatarPos.z
        )
        id: topCamera
        position: vec
        clipNear: 5
        clipFar: 1000000
        frustumCullingEnabled: false
        lookAtNode: avatarModel
    }

    DirectionalLight {
        eulerRotation.x: -45
        eulerRotation.y: 45
    }

    Node {

        id: avatarNode
        position: communicator.avatarPos
        eulerRotation: communicator.headPos
        
        PerspectiveCamera {
            id: avatarCamera
            clipNear: 5
            position: Qt.vector3d(0, 90, 0)
            clipFar: 1000000
            frustumCullingEnabled: false
        }

        Model {
            source: "#Sphere"
            id: avatarModel
            scale: Qt.vector3d(1, 1, 1)

            materials: DefaultMaterial {
                lighting: DefaultMaterial.NoLighting
                diffuseColor: "gray"
            }
        }
    }

    Model {
        source: "#Cube"
        // pickable: true
        instancing: communicator.voxelInstances

        materials: CustomMaterial {
            shadingMode: CustomMaterial.Shaded
            
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

                // if (cameraFocus.forward)  camera.position.z -= speed
                // if (cameraFocus.backward) camera.position.z += speed
                // if (cameraFocus.leftside) camera.position.x -= speed
                // if (cameraFocus.rightside) camera.position.x += speed

                if (cameraFocus.forward) communicator.avatarPos.z -= speed
                if (cameraFocus.backward) communicator.avatarPos.z += speed
                if (cameraFocus.leftside) communicator.avatarPos.x -= speed
                if (cameraFocus.rightside) communicator.avatarPos.x += speed

                // if (cameraFocus.rotateLeft) camera.eulerRotation.z -= speed - 5
                // if (cameraFocus.rotateRight) camera.eulerRotation.z += speed - 5

                // if (cameraFocus.rotateLeft) communicator.avatarPos.z -= speed - 5
                // if (cameraFocus.rotateRight) communicator.avatarPos.z += speed - 5
            }
        }
    }
}
