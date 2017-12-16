import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import "component"
import "ScanPage"
import "component/path"

Item {
    id:root
    width: 850
    height: 638

    signal minimized
    signal closed
    signal move(real dx ,real dy)

    Image {
        id: image_background
        anchors.fill: parent
        source: "qrc:/Images/background3.png"
    }

    RowLayout {
        id: rowLayout
        height: 45
        spacing: 5
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        Image {
            id: image
            Layout.maximumHeight: 32
            Layout.minimumHeight: 32
            Layout.maximumWidth: 32
            Layout.minimumWidth: 32
            source: "qrc:/Images/VOPicon.png"
        }

        Text {
            id: text1
            text: qsTr("VOP")
            Layout.fillWidth: true
            font.pixelSize: 16
        }

        Row{
            JKToolButton{
                id: button_minimize
                JKPath_minimize{
                    anchors.fill: parent
                }
            }
            JKToolButton{
                id: button_close
                JKPath_close{
                    anchors.fill: parent
                }
            }
        }
    }

    StackView{
        id:stackview
        anchors.rightMargin: 50
        anchors.leftMargin: 50
        anchors.bottomMargin: 30
        anchors.top: rowLayout.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 30
        initialItem: ScanPage{

        }
    }

    MouseArea{
        id:mouseArea
        z:-1
        anchors.fill: rowLayout
    }

    Connections{
        target: button_minimize
        onClicked: minimized()
    }
    Connections{
        target: button_close
        onClicked: closed()
    }
    Connections{
        target: mouseArea
        property real dx
        property real dy
        onPositionChanged: {
            move(mouse.x - dx ,mouse.y -dy)
        }
        onPressed: {
            dx = mouse.x
            dy = mouse.y
        }
    }
}
