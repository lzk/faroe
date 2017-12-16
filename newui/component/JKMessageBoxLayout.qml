import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    id: root
    property alias message: text_message
    property alias item_title: item_title
    property alias item_button: item_button
    property alias item_image: item_image
    property bool showImage: false
    width: 548
    height: 228

    Rectangle{
        anchors.fill: parent
        color: "#FFEFF8FF"
    }


    ColumnLayout{
        anchors.fill: parent
        Item{
            id:item_title
//            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            height: 30
        }
        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
            RowLayout {
                id: rowlayout
                anchors.fill: parent
                anchors.rightMargin: 30
                anchors.leftMargin: 30


                Item {
                    id: item_image
                    width: 100
                    height: 100
                    anchors.verticalCenter: parent.verticalCenter
                    visible: showImage

                }

                JKText {
                    id: text_message
                    color: "#555555"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: showImage ?Text.AlignLeft :Text.AlignHCenter
                    font.pixelSize: 14

                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
