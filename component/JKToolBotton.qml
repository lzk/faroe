import QtQuick 2.0

Item {
    id: container

    property alias label: labelText.text
    signal clicked

    width: 30
    height: 30

    Rectangle{
        anchors.fill: parent
        border.color: "yellow"
        border.width: mouseArea.containsMouse ?1 :0
        color: mouseArea.pressed ?"yellow":"transparent"
    }

    Text {
        id: labelText;
        font.pixelSize: 20;
        font.bold: true
        color: "#0e5685"
        anchors.centerIn: parent
    }

    MouseArea {
        id:mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: container.clicked()
    }
}
