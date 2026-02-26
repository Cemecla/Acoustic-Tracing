import QtQuick 2.15
import QtQuick.Window
import QtQuick3D

Window {
    visible: true
    width: 800
    height: 600
    title: "Voxel Grid 250 Cubes"

    View3D {
        anchors.fill: parent
        camera: camera

        environment: SceneEnvironment {
            backgroundMode: SceneEnvironment.Color
            clearColor: "lightblue"
        }

        // Camera
        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 50, 1000)
            eulerRotation.x: 20
            eulerRotation.y: -20
        }

        // Light
        DirectionalLight {
            eulerRotation.x: -45
            eulerRotation.y: 45
        }

        // Create 250 cubes using a ListModel
        ListModel {
            id: voxelModel
            Component.onCompleted: {
                for (var i = 0; i < 1000; i++)
                    append({"idx": i})
            }
        }

        Repeater3D {
            model: voxelModel

            Model {
                property int cubeIndex: index
        source: "#Cube"
        scale: Qt.vector3d(1,1,1)
        materials: DefaultMaterial { diffuseColor: "orange" }

        // spacing between cubes
        property real spacing: 101

        position: Qt.vector3d(
            (cubeIndex % 5) * spacing - 2.2,                  // X centered
            Math.floor(cubeIndex / 25) * spacing - 5.5,       // Y centered
            Math.floor((cubeIndex % 25) / 5) * spacing - 2.2 // Z centered
        )

            }
        }
    }
}
