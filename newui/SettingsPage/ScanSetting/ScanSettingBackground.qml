import QtQuick 2.0
import "../../component"

Item {
    property alias text: text1.text
    default property alias container: container.children
    Rectangle {
        id: rectangle
        color: "transparent"
        anchors.fill: parent
        border.color: "lightgray"
        radius: 10

        Rectangle {
            id: rectangle1
            height: 28
            color: "#ffffff"
            radius: 12
            border.color: "#00000000"
            border.width: 2
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FFF1F1F1" }
                GradientStop { position: 1.0; color: "#FFA4A4A4" }
                GradientStop { position: 0.214; color: "#FFEBEBEB" }
                GradientStop { position: 0.461; color: "#FFDADADA" }
                GradientStop { position: 0.764; color: "#FFB8B8B8" }
            }

            JKText {
                id: text1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
                color: "#FF0E0E0E"
            }
        }
    }

    Item {
        id: container
        anchors.topMargin: 28
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        anchors.bottomMargin: 15
        anchors.fill: parent
    }

}
