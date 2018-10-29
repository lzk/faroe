import QtQuick 2.0
import "../../component"
import QtQuick.Layouts 1.3
JKDialog {
    id:root
    width: 408 + 20
    height: 328 + 20
//    signal accepted
    property alias message: text_message

    toolbar{
        text.text: qsTr("ResStr_Warning")
        text.color: "black"
        color: "#FF67A1CF"
    }

    Item{
        parent: container
        anchors.fill: parent
        Rectangle{
            anchors.fill: parent
            anchors.topMargin: -30
            color: "#FFEFF8FF"
        }

        ColumnLayout{
            anchors.fill: parent
            Item{
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    id: rowlayout
                    anchors.fill: parent
                    anchors.rightMargin: 30
                    anchors.leftMargin: 30
                    spacing: 15


                    Image{
                        id:image
                        width: 226
                        height: 147
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:/Images/switch.png"
                    }

                    JKText {
                        id: text_message
                        color: "#555555"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter //Text.AlignLeft
                        font.pixelSize: 14

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: qsTr("ResStr_DocScan_Mediatype_Info")
                    }

                }
            }

            Item {
                id: item_button
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
    //            anchors.bottom: parent.bottom
            }
        }
    }


    JKTextButton {
        id: button_ok
        parent: item_button
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.top: parent.top
        width: 100
        height: 30
        text: "OK"
        onClicked:{
//                root.accepted()
            root.close()
        }
    }
}
