import QtQuick 2.0

Item {
    id:root
    signal close
    signal minimize
    property real movedX
    property real movedY
    property bool enableMinimize: false
    property alias text: text.text
    Text{
        id:text
        anchors.centerIn: parent
        font.pixelSize: 20
        color: "white"
    }

    MouseArea{
        property real dx
        property real dy
        anchors.fill: parent
        onPositionChanged:{
            movedX = mouse.x - dx
            movedY = mouse.y - dy
        }
        onPressed:{
            dx = mouse.x
            dy = mouse.y
        }
    }
    Row{
        id:row
        anchors.fill: parent
        layoutDirection: Qt.RightToLeft
        JKToolBotton{label: "x";onClicked: close()}
        JKToolBotton{
            label: "-";visible: enableMinimize ;onClicked: minimize()}
    }
}
