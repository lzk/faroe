import QtQuick 2.0
import "path"
Item {
    id:root
    signal close
    signal minimize
    property real movedX
    property real movedY
    property bool enableMinimize: false
    property bool enableClose: true
    property alias text: text1
    property alias color: background.color
    Rectangle{
        id:background
        color: "transparent"
        anchors.fill: parent
    }

    Text{
        id:text1
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
        JKToolButton{
            height: parent.height
            width:height
            visible: enableClose
            JKPath_close{
                anchors.fill: parent
                lineWidth: 2
            }
            onClicked: close()
        }
        JKToolButton{
            height: parent.height
            width:height
            visible: enableMinimize
            JKPath_minimize{
                anchors.fill: parent
            }
            onClicked: minimize()
        }
    }
}
