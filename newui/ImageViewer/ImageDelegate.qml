import QtQuick 2.0
import QtQuick.Controls 2.2
ItemDelegate {
    id:root
    property alias selected: root.checked
    property alias image: image
//    property alias text: text1

    signal close
//    signal doubleClick
//    signal click
    background:Item{}

    contentItem:
    Rectangle {
        id: rectangle
        anchors.fill: parent
        anchors.margins: 10
        color:image.status == Image.Error ?"black":"white"
        Image {
            id: image
            asynchronous : true
//            cache: false
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            anchors.topMargin: 25
            anchors.bottomMargin: 25

//            MouseArea {
//                id: mouseArea
//                anchors.fill: parent

//            }
        }

        Image {
            id: image_close
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.top: parent.top
            anchors.topMargin: 2
            source: "qrc:/Images/close.png"
            visible: !selected

            MouseArea {
                id: mouseArea_close
                anchors.fill: parent
            }
        }

        Rectangle {
            id: rectangle1
            color: "green"
            radius: width / 2
            anchors.fill:image_close
            visible: selected

            Text {
                id: text1
                text: root.text
                font.italic: true
                anchors.centerIn: parent
                font.pixelSize: 15
                color:"white"
            }
        }
    }

//    Connections {
//        target: mouseArea
//        onClicked: click()
//        onDoubleClicked: {
//            doubleClick()
////            mouse.accepted = false
//        }
//    }

    Connections {
        target: mouseArea_close
        onClicked: close()
    }

}
