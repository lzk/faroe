import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 750
    height: 533
    property alias item_view: item_view
    property alias item_back: item_back
    property alias item_tab: item_tab
    Item {
        id: item_back
        width: 55
        height: 55
    }

    RowLayout {
        id: rowLayout
        spacing: 0
        anchors.rightMargin: 40
        anchors.leftMargin: 40
        anchors.bottomMargin: 18
        anchors.top: item_back.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Rectangle {
            id: item_tab
            width: 175
            color: "LightGray"
            anchors.bottomMargin: 5
            anchors.top: parent.top
            anchors.bottom: parent.bottom

        }

        Item{
            id: item_view
            Layout.fillHeight: true
            Layout.fillWidth: true
            Image {
                anchors.fill: parent
                source: "qrc:/Images/SettingViewBackground.png"
            }
        }

    }

}
