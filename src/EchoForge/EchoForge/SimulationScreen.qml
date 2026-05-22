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
        lensFlareEnabled: false
        lensFlareApplyDirtTexture: false

        glowEnabled: true
        glowBloom: 0.5
        glowIntensity: 1.5

        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {
                sunEnergy: 0.0
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
        eulerRotation: communicator.headAngles
        
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

        Model {
            source: "#Cone"
            id: avatarArrow
            scale: Qt.vector3d(1, 1, 1)
            
            eulerRotation: Qt.vector3d(-90, 0, 0)
            materials: DefaultMaterial {
                lighting: DefaultMaterial.NoLighting
                diffuseColor: "gray"
            }
        }
    }

    // Rays
    Model {
        source: "#Cube"
        instancing: communicator.rayInstances

        castsShadows: false
        receivesShadows: false
        materials: CustomMaterial {
            shadingMode: CustomMaterial.Shaded
            property real intensity: 1.0
            vertexShader: "qrc:/shaders/ray.vert"
            fragmentShader: "qrc:/shaders/ray.frag"

            NumberAnimation on intensity {
                from: 1.0; to: 5.0; duration: 2000; loops: Animation.Infinite
            }
        }
    }

    // Voxels
    Model {
        source: "#Cube"
        instancing: communicator.voxelInstances

        materials: CustomMaterial {

            property real atlasCols: 9.0
            property real atlasRows: 10.0
            property real atlasTexWidth: 1152.0
            property real atlasTexHeight: 1280.0

            property TextureInput baseColorMap: TextureInput {
                texture: Texture {
                    source: "qrc:/textures/atlasVoxel"
                    minFilter: Texture.Nearest
                    magFilter: Texture.Nearest
                    mipFilter: Texture.None
                    tilingModeHorizontal: Texture.ClampToEdge
                    tilingModeVertical: Texture.ClampToEdge
                    generateMipmaps: false
                }
            }

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
