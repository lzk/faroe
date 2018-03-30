import QtQuick 2.0
import "../component"

JKAbstractButton {
    id:root
    width: ListView.view.width
    height:76
//    activeFocusOnTab: ListView.isCurrentItem
    dashRectange.z: 2
    property alias source: image.source
    property alias text: text.text
    property alias size: textSize.text

    Rectangle{
        anchors.fill: parent
        anchors.margins: 1
        z:1
        opacity: root.ListView.isCurrentItem ?0.7 :mouseArea.containsMouse ?0.5 :1
        border.color: mouseArea.containsMouse || root.ListView.isCurrentItem ?"#2996e0"
                                             :"Transparent"
        border.width: 1
        color: mouseArea.containsMouse || root.ListView.isCurrentItem ?"lightblue"
                                       : "white"
        Image{
            id:image
            x:10
            width: 50
            height:50
            anchors.verticalCenter: parent.verticalCenter
        }

        JKText{
            id:text
            x:80
//            width: parent.width - 200
            anchors.verticalCenter: parent.verticalCenter
        }
        JKText{
            id:textSize
            anchors.left: parent.right
            anchors.leftMargin: -100
            anchors.verticalCenter: parent.verticalCenter
        }
    }

//    onFocusChanged:
//        activeFocusOnTab = focus
    onClicked: {
        root.ListView.view.currentIndex = index
    }
    onDoubleClicked: {
        root.ListView.view.currentIndex = index
    }
}

