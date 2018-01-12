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
                text: qsTr("IP Type:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            Row {
                id: item_checkbox
                width: 285
                height: 25
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                RadioButton{
                    id:radiobutton_dhcp
                    width:142
                    height: parent.height
                    text:qsTr("DHCP")
                }
                RadioButton{
                    id:radiobutton_static
                    width:143
                    height: parent.height
                    text:qsTr("Static")
                }
            }
        }

        DividingLine{
            height: 6
            width: parent.width
        }
        Item{
            height: 30
            Layout.fillWidth: true
        }

        Item {
            id: item2
            height: 60
            width: parent.width
            JKText {
                id: text3
                text: qsTr("IP Address")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: input_ipAddress
                width: 250
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
                text: qsTr("Submask")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: input_submask
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: item6
            height: 60
            width: parent.width
            JKText {
                id: text5
                text: qsTr("Gateway")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: input_gateway
                width: 250
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

    Connections{
        target: button_apply
        onClicked: {
        }
    }

}
