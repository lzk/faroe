import QtQuick 2.0

JKDialog {
    id:root
    width: 548 + 20
    height: 228 + 20
    signal accepted(string para)
    property alias message: messagebox.message
    property alias showImage: messagebox.showImage
    property string para

    toolbar{
        text.text: qsTr("Infomation")
        text.color: "black"
        color: "#FF67A1CF"
    }
    JKMessageBoxLayout{
        id:messagebox
        parent: container
        anchors.fill: parent

        Image{
            parent:messagebox.item_image
            source:"qrc:/Images/warning.png"
            anchors.fill: parent
            anchors.margins: 5
        }

        message.text: qsTr("Do you want to delete the selected image?")

        Row {
            parent: messagebox.item_button
            width: 250
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.top: parent.top

            JKTextButton {
                id: button_yes
                width: 120
                height: 30
                text.text: "Yes"
                anchors.verticalCenter: parent.verticalCenter
                onClicked:{
                    root.close()
                    root.accepted(para)
                }
            }

            JKTextButton {
                id: button_no
                width: 120
                height: 30
                text.text: "No"
                anchors.verticalCenter: parent.verticalCenter
                onClicked: root.close()
            }
        }
    }

//    function openWithMessage(message){
//        messagebox.message.text = message
//        open()
//    }
}
