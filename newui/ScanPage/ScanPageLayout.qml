import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    id: root
    width: 750
    height: 533
    property alias item_settings: item_settings
    property alias item_scanto: item_scanto
    property alias item_separation: item_separation
    property alias item_decode: item_decode
    property alias item_rightButton: item_rightButton
    property alias item_quickscan: item_quickscan
    property alias item_search: item_search
    property alias item_view: item_view
    property alias item_leftButton: item_leftButton
    ColumnLayout {
        id: columnLayout
        anchors.rightMargin: 40
        anchors.leftMargin: 40
        anchors.fill: parent

        RowLayout {
            id: rowLayout
            Layout.fillWidth: true
            spacing: 16
            Layout.maximumHeight: root.height * 2 / 3
            Layout.minimumHeight: root.height * 2 / 3
            anchors.right: parent.right
            anchors.left: parent.left

            Item {
                id: item_leftButton
                Layout.maximumHeight: 90
                Layout.maximumWidth: 90
                Layout.minimumHeight: 90
                Layout.minimumWidth: 90
            }

            Image{
                id: item_view
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: "qrc:/Images/screenbg_normal.png"
                ColumnLayout {
                    id: columnLayout1
                    spacing: 0
                    anchors.fill: parent
                    Image {
                        id: item_search
                        Layout.fillWidth: true
                        Layout.maximumHeight: 40
                        Layout.minimumHeight: 40
                        source: "qrc:/Images/main_img_search device panel.png"
                    }
//                    Image{
//                        Layout.fillWidth: true
//                        height: 26
//                        source:"qrc:/Images/quick scan_shadow_top.png"
//                    }
                    Item{
                        id:item_quickscan
//                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        height: 282
                    }
                    Item{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

//                    Image{
//                        Layout.fillWidth: true
//                        height: 26
//                        source:"qrc:/Images/quick scan_shadow_below.png"
//                    }
                }
            }

            Item {
                id: item_rightButton
                Layout.maximumHeight: 90
                Layout.minimumHeight: 90
                Layout.maximumWidth: 90
                Layout.minimumWidth: 90
            }
        }

        RowLayout {
            id: rowLayout2
            anchors.right: parent.right
            anchors.left: parent.left
            property real itemWidth: width / 4
            height: 160
            spacing: 0
            Layout.fillWidth: true

            Item {
                id: item_decode
                Layout.fillHeight: true
                Layout.maximumWidth: parent.itemWidth
                Layout.minimumWidth: parent.itemWidth
            }

            Item {
                id: item_separation
                Layout.fillHeight: true
                Layout.maximumWidth: parent.itemWidth
                Layout.minimumWidth: parent.itemWidth
            }

            Item {
                id: item_scanto
                Layout.fillHeight: true
                Layout.maximumWidth: parent.itemWidth
                Layout.minimumWidth: parent.itemWidth
            }

            Item {
                id: item_settings
                Layout.fillHeight: true
                Layout.maximumWidth: parent.itemWidth
                Layout.minimumWidth: parent.itemWidth
            }
        }
        Item{
            Layout.fillHeight: true
        }
    }
}
