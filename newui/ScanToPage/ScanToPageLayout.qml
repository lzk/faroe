import QtQuick 2.0

Item {
    id: item1
    width: 750
    height: 533
    property alias item_down: item_down
    property alias item_up: item_up
    property alias item_toCloud: item_toCloud
    property alias item_toApplication: item_toApplication
    property alias item_toFTP: item_toFTP
    property alias item_toFile: item_toFile
    property alias item_toEmail: item_toEmail
    property alias item_toPrint: item_toPrint
    property alias item_view: item_view
    property alias item_title: item_title
    property alias item_back: item_back
    Item {
        id: item_back
        width: 80
        height: 50
    }

    Column {
        id: column
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 80
        anchors.left: item_back.right

        Item {
            id: item_title
            height: 50
            anchors.right: parent.right
            anchors.left: parent.left
        }

        Item {
            id: item_view
            height: 350
            anchors.right: parent.right
            anchors.left: parent.left
        }

        Row {
            id: row
            height: 100
            spacing: 6
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.right: parent.right
            anchors.left: parent.left

            Item {
                id: item_toPrint
                width: 90
                anchors.bottom: parent.bottom
                anchors.top: parent.top
            }

            Item {
                id: item_toEmail
                width: 90
                anchors.bottom: parent.bottom
                anchors.top: parent.top
            }

            Item {
                id: item_toFile
                width: 90
                anchors.bottom: parent.bottom
                anchors.top: parent.top
            }

            Item {
                id: item_toFTP
                width: 90
                anchors.bottom: parent.bottom
                anchors.top: parent.top
            }

            Item {
                id: item_toApplication
                width: 90
                anchors.bottom: parent.bottom
                anchors.top: parent.top
            }

            Item {
                id: item_toCloud
                width: 90
                anchors.bottom: parent.bottom
                anchors.top: parent.top
            }
        }
    }

    Column {
        id: column1
        y: 175
        width: 60
        height: 120
        spacing: 20
        anchors.left: column.right

        Item {
            id: item_up
            width: 50
            height: 50
        }

        Item {
            id: item_down
            width: 50
            height: 50
        }
    }

}
