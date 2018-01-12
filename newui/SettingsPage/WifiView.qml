import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import "component"
import "../component"

Item {
    id: item1
    width: 495
    height: 460

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        Item {
            id: item3
            height: 50
            width: parent.width


            JKText {
                id: text1
                text: qsTr("WLAN")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            Item {
                id: item_checkbox
                width: 100
                height: 50
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        DividingLine{
            height: 6
            width: parent.width
        }

        Item {
            id: item2
            height: 45
            width: parent.width

            JKTextButton{
                width: 120
                height: 35
                text.text: qsTr("Manual Wi-Fi...")
                anchors.right: parent.right
                anchors.rightMargin: 5
            }
        }

        Item {
            id: item5
            height: 40
            width: parent.width
            JKText {
                id: text4
                text: qsTr("WLAN Network")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
            }

            JKImageButton{
                width: 17
                height: 17
                source_disable: "qrc:/Images/Refresh_Disable.png"
                source_normal: "qrc:/Images/Status_RefreshEnable.tif"
                source_press: "qrc:/Images/Status_RefreshEnable.tif"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
            }
            Image{
                width: parent.width
                height: 2
                source: "qrc:/Images/GreenLine.png"
                anchors.bottom: parent.bottom
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

    }

    Component.onCompleted: {
    }

    CheckBox{
        parent: item_checkbox
        anchors.fill: parent

    }

}
