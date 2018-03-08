import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import "component"
import "../component"
import com.liteon.JKInterface 1.0

Item {
    id: root
    width: 495
    height: 460
    enabled: scanData.deviceStatus

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

                JKRadioButton{
                    id:radiobutton_dhcp
                    width:142
                    height: parent.height
                    text:qsTr("DHCP")
                    checked: true
                }
                JKRadioButton{
                    id:radiobutton_static
                    width:143
                    height: parent.height
                    text:qsTr("Static")
                    checked: !radiobutton_dhcp.checked
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

        Column{
            width: parent.width
            enabled: radiobutton_static.checked
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
                    input.validator: RegExpValidator{
                        regExp: /[0-2]?\d{0,2}\.[0-2]?\d{0,2}\.[0-2]?\d{0,2}\.[0-2]?\d{0,2}/
                    }
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
                    input.validator: RegExpValidator{
                        regExp: /[0-2]?\d{0,2}\.[0-2]?\d{0,2}\.[0-2]?\d{0,2}\.[0-2]?\d{0,2}/
                    }
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
                    input.validator: RegExpValidator{
                        regExp: /[0-2]?\d{0,2}\.[0-2]?\d{0,2}\.[0-2]?\d{0,2}\.[0-2]?\d{0,2}/
                    }
                }
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


    Connections{
        target: button_apply
        onClicked: {
            setting.gatewayAddress = input_gateway.text
            setting.address = input_ipAddress.text
            setting.subnetMask = input_submask.text
            setting.addressMode = radiobutton_dhcp.checked ?3 :4
            setSetterCmd(DeviceStruct.CMD_setIpv4 ,setting)
        }
    }

    property var setting:{
        "enable":true
        ,"address":"127.0.0.1"
        ,"addressMode":3//3 dhcp 4 manual
        ,"gatewayAddress":"127.0.0.1"
        ,"mode":0
        ,"subnetMask":"127.0.0.1"
    }
    Component.onCompleted: {
        setSetterCmd(DeviceStruct.CMD_getIpv4 ,setting)
    }
    Connections{
        target: jkInterface
        onCmdResult:{
            switch(cmd){
            case DeviceStruct.CMD_getIpv4:
                if(!result){
                    setting = JSON.parse(data)
                    console.log(data)
                    input_gateway.text = setting.gatewayAddress
                    input_ipAddress.text = setting.address
                    input_submask.text = setting.subnetMask
//                    if(setting.addressMode === 4)
//                        radiobutton_static.checked = true
//                    else
////                        if(setting.addressMode === 3)
//                        radiobutton_dhcp.checked = true
                    radiobutton_dhcp.checked = setting.addressMode !== 4
                }else{
                    input_gateway.text = "0.0.0.0"
                    input_ipAddress.text = "0.0.0.0"
                    input_submask.text = "0.0.0.0"
                    radiobutton_dhcp.checked = true
                }

                break;
            case DeviceStruct.CMD_setIpv4:
                if(!result){
                    information_1button(qsTr("Configuration completed. Restart the device to apply changes."))
                }
                break
            }
        }
    }

}
