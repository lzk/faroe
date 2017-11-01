import QtQuick 2.0

Item {
    id: container

    property real value: 0.0

//    Behavior on opacity { NumberAnimation { duration: 600 } }

    Rectangle { anchors.fill: parent; color: "darkgray"; opacity: 0.5 ;radius: height/2}

    Rectangle {
        id: fill;  height: container.height
        width: container.width * container.value
        radius: height / 2
        gradient: Gradient {
            GradientStop {position: 0.0 ;color: "darkgray"}
            GradientStop {position: 0.1 ;color: "white"}
            GradientStop {position: 1.0 ;color: "green"}
        }
    }

    Text {
        text: "%1%".arg(value * 100)
        anchors.centerIn: parent
    }
}
