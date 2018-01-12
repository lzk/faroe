import QtQuick 2.0

Item {
    id: root
    width: 648
    height: 700-30
    property alias item_next: item_next
    property alias item_pre: item_pre
    property alias item_return: item_return
    property alias item_fb4: item_fb4
    property alias item_fb3: item_fb3
    property alias item_fb2: item_fb2
    property alias item_fb1: item_fb1
    property alias item_viewname: item_viewname
    property alias item_view: item_view

    Image {
       anchors.fill: parent
       anchors.topMargin: -30
        source: "qrc:/Images/popup_gnd_Image Preview.png"
    }

    Column {
        id: column
        anchors.fill: parent

        Item {
            id: item2
            height: 30
            anchors.right: parent.right
            anchors.left: parent.left
        }

        Item {
            id: item_view
            width: 540
            height: 450
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            id: item1
            width: 30
            height: 30
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter

            Item {
                id: item_pre
                width: 10
                height: 15
                anchors.verticalCenter: parent.verticalCenter
            }

            Item {
                id: item_next
                width: 10
                height: 15
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: item_viewname
            width: 200
            height: 30
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item {
            id: item3
            height: 80
            anchors.right: parent.right
            anchors.left: parent.left

            Row {
                id: row
                width: 310
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.top: parent.top

                Item {
                    id: item_fb1
                    width: 70
                    height: 75
                }

                Item {
                    id: item_fb2
                    width: 70
                    height: 75
                }

                Item {
                    id: item_fb3
                    width: 70
                    height: 75
                }

                Item {
                    id: item_fb4
                    width: 70
                    height: 75
                }
            }
        }

        Item {
            id: item_return
            width: 150
            height: 33
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

}
