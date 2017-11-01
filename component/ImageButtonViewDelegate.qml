import QtQuick 2.4

Item {
    id: container
    width: 400
    height: 400
    property alias image: image
    property alias text: text1
    signal clicked
    Column {
        id: column
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Image {
            id:image
            width: 80
            height: 80
//            source : mouseArea.containsMouse&&!mouseArea.pressed
//                    ?"Images/lightbutton_over.png"
//                    :mouseArea.pressed?"Images/lightbutton_press.png":"Images/lightbutton.png"
        }

        Text {
            id: text1
            font.pointSize: 12
            anchors.horizontalCenter: column.horizontalCenter
        }

    }
    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: column
    }
    Connections {
        target: mouseArea
        onClicked: container.clicked()
    }
}
