import QtQuick 2.0

JKDialog {
    id:root
    width: 490 + 20
    height: 168 + 20
    property alias message: messagebox.message
    property bool showCancel: true
    signal cancelClick
    toolbar{
        color: "#FF67A1CF"
        enableClose: false
    }
    JKMessageBoxLayout{
        id:messagebox
        parent: container
        anchors.fill: parent
        item_button.visible: showCancel
        message.text: qsTr("Searching,please wait...")
        item_image.width: 90
        item_image.height: 90

        showImage: true
        JKBusyIndicator{
            parent:messagebox.item_image
            anchors.fill: parent
            anchors.margins: 15
        }

        JKTextButton{
            parent:messagebox.item_button
            width: 100
            height: 25
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.bottom: parent.bottom
            text: qsTr("Cancel")
            onClicked: cancelClick()
        }
    }

}
