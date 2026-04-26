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
        eulerRotation.y: 280
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
        source: "#Cylinder"
        instancing: communicator.rayInstances

        castsShadows: false
        receivesShadows: false

        materials: CustomMaterial {
            shadingMode: CustomMaterial.Shaded
            
            vertexShader: "qrc:/shaders/ray.vert"
            fragmentShader: "qrc:/shaders/ray.frag"
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
    
        Keys.onPressed: (event) => {
            if (!event.isAutoRepeat)
                communicator.userInput(event.key, true)
        }
    
        Keys.onReleased: (event) => {
            if (!event.isAutoRepeat)
                communicator.userInput(event.key, false)
        }
    }
}
