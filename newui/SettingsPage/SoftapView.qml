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
                text: qsTr("Soft AP:")
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
            height: 80
            width: parent.width
            JKText {
                id: text3
                text: qsTr("SSID")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: input_ssid
                width: 275
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: item5
            height: 60
            width: parent.width
            JKText {
                id: text4
                text: qsTr("Password")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: input_password
                width: 275
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Item{
            Layout.fillWidth: true
            height: 60
            JKTextButton {
                id: button_apply
                text.text: qsTr("Apply")
                width: 150
                height: 35
                anchors.centerIn: parent
            }
        }

    }

    Component.onCompleted: {
    }

    CheckBox{
        parent: item_checkbox
        anchors.fill: parent
        text:qsTr("Open")

    }
    Connections{
        target: button_apply
        onClicked: {
        }
    }

}
