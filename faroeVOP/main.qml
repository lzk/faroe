import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import "../component"

ApplicationWindow {
    id:window
    visible: true
    width: 850
    height: 640
    title: qsTr("Faroe VOP")
    flags: Qt.Window | Qt.FramelessWindowHint

    background: Rectangle{
        border.color: "#0e5685"
        border.width: 10
        anchors.fill: parent


        Image {
            source: "Images/background3.png"
            anchors.fill: parent
        }
        JKToolbar{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            height: 40
            enableMinimize: true
//            text:qsTr("Faroe VOP")
            onClose: {
                console.log("quit")
                Qt.quit()
            }
            onMinimize: window.visibility = ApplicationWindow.Minimized
            onMovedXChanged: window.x += movedX
            onMovedYChanged: window.y += movedY
        }
//        Row{
//            anchors.top: parent.top
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.margins: 10
//            height: 50
//            layoutDirection: Qt.RightToLeft
//            JKToolBotton{label: "x";onClicked: Qt.quit()}
//            JKToolBotton{
//                label: "-";onClicked: window.visibility = ApplicationWindow.Minimized}
//            Rectangle{
//                color:"transparent"
//                width: parent.width - 50
//                height: parent.height
//                MouseArea{
//                    property real dx
//                    property real dy
//                    anchors.fill: parent
//                    onPositionChanged:{
//                        window.x += mouse.x - dx
//                        window.y += mouse.y - dy
//                    }
//                    onPressed:{
//                        dx = mouse.x
//                        dy = mouse.y
//                    }
//                }
//            }
//        }
    }

    StackView{
        id:stackView
        anchors.fill: parent
        anchors.topMargin: 100
        anchors.bottomMargin: 30
        initialItem: MainWindow{

        }
    }

}
