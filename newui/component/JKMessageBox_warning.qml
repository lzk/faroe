import QtQuick 2.0

JKDialog {
    id:root
    width: 548 + 20
    height: 228 + 20
//    signal accepted
//    property alias title: jKToolbar.text
    property alias message: messagebox.message
    property alias showImage: messagebox.showImage
    property alias image: image

    toolbar{
        text.text: qsTr("ResStr_Warning")
        text.color: "black"
        color: "#FF67A1CF"
    }
    JKMessageBoxLayout{
        id:messagebox
        parent: container
        anchors.fill: parent

        Image{
            id:image
            parent:messagebox.item_image
            source:"qrc:/Images/warning_2.png"
            anchors.fill: parent
        }

        JKTextButton {
            id: button_ok
            parent: messagebox.item_button
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.top: parent.top
            width: 100
            height: 30
            text: qsTr("ResStr_OK")
            onClicked:{
//                root.accepted()
                root.close()
            }
        }
    }
    function openWithMessage(message){
        messagebox.message.text = message
        open()
    }
}
