import QtQuick
import QtQuick3D 6.11
import QtQuick3D.Helpers

View3D {
    id: view3DMain
    required property bool debugEnable
    property bool isFirstPerson: false
    property int baseSize: 100
    property bool isRaysVisible: true
    property bool isEmittersVisible: true
    property color avatarColor: appSettings.avatarColor
    property var emitters: null
    property var emittersId: []
    camera: debugEnable ? freeCamera : isFirstPerson ? avatarCamera : topCamera

    // ENVIRONMENT
    environment: ExtendedSceneEnvironment {
        backgroundMode: SceneEnvironment.SkyBox
        lensFlareEnabled: false
        lensFlareApplyDirtTexture: false

        glowEnabled: true
        glowBloom: 0.5
        glowIntensity: 1.5

        lightProbe: Texture {
            textureData: ProceduralSkyTextureData {
                sunEnergy: 60.0
                sunLongitude: 280.0
                textureQuality: appSettings.graphicQuality == "Low" ? ProceduralSkyTextureData.SkyTextureQualityLow 
                : appSettings.graphicQuality == "Medium" ? ProceduralSkyTextureData.SkyTextureQualityMedium
                : ProceduralSkyTextureData.SkyTextureQualityHigh
            }
        }

        InfiniteGrid {
            visible: view3DMain.debugEnable
            gridInterval: 100
        }
    }

    DirectionalLight {
        eulerRotation.x: -45
        eulerRotation.y: 280
    }

    AxisHelper {
        visible: view3DMain.debugEnable
        enableXYGrid: true
    }

    // CAMERAS
    PerspectiveCamera {
        id: freeCamera
        position: Qt.vector3d(0, 90, 0)
        clipNear: 5
        clipFar: 1000000
        frustumCullingEnabled: false
    }

    PerspectiveCamera {
        readonly property bool isActive: view3DMain.camera == topCamera
        readonly property real speed: 5.0
        property real height: 550 
        property var vec: Qt.vector3d(
            communicator.avatarPos.x,
            communicator.avatarPos.y + height,
            communicator.avatarPos.z
        )
        id: topCamera
        position: vec
        clipNear: 5
        clipFar: 1000000
        frustumCullingEnabled: false
        lookAtNode: avatarModel

    }

    // AVATAR
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
                diffuseColor: view3DMain.avatarColor
            }
        }

        Model {
            source: "#Cone"
            id: avatarArrow
            scale: Qt.vector3d(0.5, 0.5, 0.5)
            position: Qt.vector3d(0, 0, -60)
            eulerRotation: Qt.vector3d(-90, 0, 0)
            materials: DefaultMaterial {
                lighting: DefaultMaterial.NoLighting
                diffuseColor: view3DMain.avatarColor
            }
        }
    }

    // EMITTERS
    Node {
        visible: view3DMain.isEmittersVisible;
        Repeater3D {
            model: view3DMain.emittersId
            delegate: Model {
                property string eid: modelData
                position: Qt.vector3d(view3DMain.emitters[eid]["position"].x, view3DMain.emitters[eid]["position"].y, view3DMain.emitters[eid]["position"].z)
                scale: Qt.vector3d(0.4, 0.4, 0.4)
                
                source: "#Sphere"
                materials: [
                    DefaultMaterial {
                        diffuseColor: view3DMain.emitters[eid]["color"]
                    }
                ]
            }
        }
    }

    // RAYS
    Model {
        id: rayModel
        visible: view3DMain.isRaysVisible;
        source: "#Cube"
        instancing: communicator.rayInstances
        opacity: 0.1

        castsShadows: false
        receivesShadows: false
        materials: CustomMaterial {
            id: customMaterialRoot
            shadingMode: CustomMaterial.Shaded
            property real intensity: 1.0
            vertexShader: "qrc:/shaders/ray.vert"
            fragmentShader: "qrc:/shaders/ray.frag"
        }
    }

    // VOXELS
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

    // CONTROLS
    WasdController {
        controlledObject: freeCamera
    }

    Keys.onPressed: (event) => {
        if (event.key == Qt.Key_C) {
            isFirstPerson = !isFirstPerson
        }

        if (event.key == Qt.Key_Z && topCamera.isActive) {
            topCamera.height = Math.max(50, topCamera.height - topCamera.speed)
        }

        if (event.key == Qt.Key_X && topCamera.isActive) {
            topCamera.height = Math.min(2000, topCamera.height + topCamera.speed)
        }
    }

    FocusScope {
        id: cameraFocus
        anchors.fill: parent
        focus: true

        onFocusChanged: !view3DMain.debugEnable ? forceActiveFocus() : null
    
        MouseArea {
            anchors.fill: parent
            onClicked: !view3DMain.debugEnable ? cameraFocus.forceActiveFocus() : null
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
