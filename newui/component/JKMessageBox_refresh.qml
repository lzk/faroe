import QtQuick 2.0

JKDialog {
    id:root
    width: 490 + 20
    height: 188 + 20
    JKMessageBoxLayout{
        id:messagebox
        parent: container
        anchors.fill: parent
        item_button.visible: false
        message.text: qsTr("Searching,please wait...")

        showImage: true
        JKBusyIndicator{
            parent:messagebox.item_image
            anchors.fill: parent
            anchors.margins: 15
        }

        JKToolbar {
            id: jKToolbar
            parent: messagebox.item_title
            anchors.fill: parent
            color: "#FF67A1CF"
            enableClose: false
            onMovedXChanged: root.x += movedX
            onMovedYChanged: root.y += movedY
        }
    }

}
