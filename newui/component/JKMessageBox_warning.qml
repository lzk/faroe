import QtQuick 2.0

JKDialog {
    id:root
    width: 548 + 20
    height: 228 + 20
//    signal accepted
//    property alias title: jKToolbar.text
    property alias message: messagebox.message
    property alias showImage: messagebox.showImage
    JKMessageBoxLayout{
        id:messagebox
        parent: container
        anchors.fill: parent

        Image{
            parent:messagebox.item_image
            source:"qrc:/Images/warning_2.png"
            anchors.fill: parent
            anchors.margins: 5
        }

        JKToolbar {
            id: jKToolbar
            parent: messagebox.item_title
            anchors.fill: parent
            text.text: qsTr("Warning")
            text.color: "black"
            color: "#FF67A1CF"
            onClose: root.close()
            onMovedXChanged: root.x += movedX
            onMovedYChanged: root.y += movedY
        }

        JKTextButton {
            id: button_ok
            parent: messagebox.item_button
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.top: parent.top
            width: 120
            height: 30
            text.text: "OK"
            onClicked:{
//                root.accepted()
                root.close()
            }
        }
    }

}
