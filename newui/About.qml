import QtQuick 2.0
import "component"

JKDialog {
    width: 400 + 20
    height: 200 + 20

    property int pixelSize: 16
    property color textColor: "#222222"

    toolbar{
        text.text: qsTr("ResStr_About")
        text.color: "White"
        color: "#FF67A1CF"
    }

    Rectangle{
        parent: container
        anchors.fill: parent

        JKText{
            font.pixelSize: pixelSize
            text: qsTr("ResStr_DocScan") + " 0.1.4"
            color: textColor
            anchors.centerIn: parent
        }

//        Column{
//            anchors.fill: parent
//            anchors.margins: 30
//            spacing: 5



////            Item {
////               width: parent.width
////               height: 10
////            }

////            JKText{
////                font.pixelSize: pixelSize
////                text: qsTr("ResStr_CopyRight")
////                color: textColor
////            }
////            JKText{
////                font.pixelSize: pixelSize
////                text: qsTr("ResStr_Company")
////                color: textColor
////            }
////            JKText{
////                font.pixelSize: pixelSize
////                text: qsTr("ResStr_AllRight")
////                color: textColor
////            }
//        }
    }
}
