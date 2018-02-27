import QtQuick 2.0

JKDialog {
    id:root
    width: 490 + 20
    height: 188 + 20
    property alias message: messagebox.message
    toolbar{
        color: "#FF67A1CF"
        enableClose: false
    }
    JKMessageBoxLayout{
        id:messagebox
        parent: container
        anchors.fill: parent
        item_button.visible: false
        message.text: qsTr("Searching,please wait...")
        item_image.width: 100
        item_image.height: 100

        showImage: true
        JKBusyIndicator{
            parent:messagebox.item_image
            anchors.fill: parent
            anchors.margins: 15
        }
    }

}
