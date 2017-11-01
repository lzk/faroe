import QtQuick 2.0
import QtQuick.Controls 2.2

Dialog {
    id: root
    parent:  ApplicationWindow.overlay
    modal:true
    focus:true
    closePolicy:Popup.NoAutoClose
    width:Math.round(ApplicationWindow.window.width * 1  / 3)
    height: Math.round(ApplicationWindow.window.height * 1 / 3 )
    property alias container: container
    property alias toolbar:toolbar
    background: Item{

    }

    Item{
        anchors.fill: parent

        Rectangle {
            id: rectangle1
            height: 30
            color: "#252323"
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            JKToolbar{
                id:toolbar
                anchors.fill: parent
                onClose: root.close()
                onMovedXChanged: root.x += movedX
                onMovedYChanged: root.y += movedY
            }
        }
        Item{
            id:container
            anchors.top: rectangle1.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
    }
    function moveToCenter(){
//            x=parent.mapFromItem(null ,Math.round((ApplicationWindow.window.width - width) / 2),0).x
//            y=parent.mapFromItem(null ,0,Math.round((ApplicationWindow.window.height - height) / 2)).y
        x=Math.round((ApplicationWindow.window.width - width) / 2)
        y=Math.round((ApplicationWindow.window.height - height) / 2)

    }
}
