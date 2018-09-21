import QtQuick 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id:window
    property alias container: container
    property alias background: background
    property alias toolbar: jKToolbar
//    property var dialog
    flags: Qt.FramelessWindowHint //| Qt.Dialog
    color: "transparent"
    modality: Qt.ApplicationModal // Qt.ApplicationModal Qt.WindowModal
    property bool defaultClose: true

    Item{
        anchors.fill: parent
        anchors.margins: 10
        visible: window.visible

        RectangularGlow{
            anchors.fill: parent
            glowRadius: 5
            spread: 0.2
            color: "#FF858484"
            cornerRadius: glowRadius
        }

        Item{
            id:background
            anchors.fill: parent
        }


        JKToolbar {
            id: jKToolbar
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.top
            anchors.bottomMargin: -30
            z:10
            onClose: {
                if(defaultClose)
                    window.close();
//                dialog=undefined
            }
            onMovedXChanged: window.x += movedX
            onMovedYChanged: window.y += movedY
        }
        Item{
            id:container
            anchors.fill: parent
            anchors.topMargin: 30
        }
    }

    function open(){
        show()
    }
}



//import QtQuick 2.0
//import QtQuick.Window 2.2
//import QtGraphicalEffects 1.0
//import QtQuick.Controls 2.2
//Popup {
//    id:window
//    property alias container: container
////    property alias background: background
//    property alias toolbar: jKToolbar
//    property bool defaultClose: true
//    modal: true

//    background: Item{
//        anchors.fill: parent
//        anchors.margins: 10
//        RectangularGlow{
//            anchors.fill: parent
//            glowRadius: 10
//            spread: 0.2
//            color: "#FF858484"
//            cornerRadius: glowRadius
//        }
//    }

//    Item{
//        anchors.fill: parent
//        anchors.margins: 10
//        visible: window.visible

////        RectangularGlow{
////            anchors.fill: parent
////            glowRadius: 10
////            spread: 0.2
////            color: "#FF858484"
////            cornerRadius: glowRadius
////        }

////        Item{
////            id:background
////            anchors.fill: parent
////        }


//        JKToolbar {
//            id: jKToolbar
//            anchors.top:parent.top
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.bottom: parent.top
//            anchors.bottomMargin: -30
//            z:10
//            onClose: {
//                if(defaultClose)
//                    window.close();
////                dialog=undefined
//            }
//            onMovedXChanged: window.x += movedX
//            onMovedYChanged: window.y += movedY
//        }
//        Item{
//            id:container
//            anchors.fill: parent
//            anchors.topMargin: 30
//        }
//    }
//}
