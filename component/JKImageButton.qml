import QtQuick 2.0

Image {
    id: container
    property url normalSource
    property url hoverSource
    property url pressedSource
    signal clicked
    source :mouseArea.containsMouse&&!mouseArea.pressed ?hoverSource:mouseArea.pressed?pressedSource:normalSource

    MouseArea {
        id:mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: container.clicked()
    }
}
