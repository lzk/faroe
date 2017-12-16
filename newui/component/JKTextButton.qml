import QtQuick 2.0

JKButton{
    id:root
    property alias text: text1
    Rectangle {
        parent: container
        anchors.fill: parent
        radius: 5
        opacity: 0.8
        border.color: "lightgray"
        color: root.state == "pressed" ?"lightgray" :"transparent"
        Text{
            id: text1
            anchors.centerIn: parent
            font.pixelSize: 16
            color: root.state == "disable" ?"lightgray" :"black"
        }
    }
}
