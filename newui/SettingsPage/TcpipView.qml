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
    enabled: scanData.deviceStatus && setting.powerSupply !== JKEnums.PowerMode_usbBusPower

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
                    text:qsTr("DHCP/Auto IP")
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
            Row {
                id: item2
                height: 60
                width: parent.width
//                spacing: 20
                JKText {
                    id: text3
                    width: 100
                    text: qsTr("IP Address")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_ipAddress
                    width: 250
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /[\d.]*/
                    }
                    tooltip.text: text.match(regExp) && jkInterface.isIpv4(text)  ?"" :qsTr("Invalid IP address.Please check and enter again.")
                    onFocusChanged: {
                        if(!focus){
                            if(tooltip.text !== ""){
                                text = "0.0.0.0"
                            }
                        }
                    }
                }
            }

            Row {
                id: item5
                height: 60
                width: parent.width
//                spacing: 20
                JKText {
                    id: text4
                    width: 100
                    text: qsTr("Submask")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_submask
                    width: 250
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /[\d.]*/
                    }
                    tooltip.text: text.match(regExp) && jkInterface.isSubmask(text) ?"" :qsTr("Invalid subnet mask.Please check and enter again.")
                    onFocusChanged: {
                        if(!focus){
                            if(tooltip.text !== ""){
                                text = "255.255.255.0"
                            }
                        }
                    }
                }
            }

            Row {
                id: item6
                height: 60
                width: parent.width
//                spacing: 20
                JKText {
                    id: text5
                    width: 100
                    text: qsTr("Gateway")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_gateway
                    width: 250
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /[\d.]*/
                    }
                    tooltip.text: text.match(regExp) && jkInterface.isIpv4(text) ?"" :qsTr("Invalid Gateway address.Please check and enter again.")
                    onFocusChanged: {
                        if(!focus){
                            if(tooltip.text !== ""){
                                text = "0.0.0.0"
                            }
                        }
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
                text: qsTr("Apply")
                width: 150
                height: 35
                anchors.centerIn: parent
                enabled: textInput_ipAddress.tooltip.text === ""
                      && textInput_submask.tooltip.text === ""
                      && textInput_gateway.tooltip.text === ""
            }
        }

    }

    Connections{
        target: button_apply
        onClicked: {
            if(!textInput_ipAddress.text.match(regExp)){
                warningWithImage(qsTr("Invalid IP address.Please check and enter again."))
                textInput_ipAddress.forceActiveFocus()
//                textInput_ipAddress.focus = true
                return
            }
            if(!textInput_submask.text.match(regExp)){
                warningWithImage(qsTr("Invalid subnet mask.Please check and enter again."))
                textInput_submask.forceActiveFocus()
//                textInput_submask.focus = true
                return
            }
            if(!textInput_gateway.text.match(regExp)){
                warningWithImage(qsTr("Invalid Gateway address.Please check and enter again."))
                textInput_gateway.forceActiveFocus()
//                textInput_gateway.focus = true
                return
            }
            setting.gatewayAddress = textInput_gateway.text
            setting.address = textInput_ipAddress.text
            setting.subnetMask = textInput_submask.text
            setting.addressMode = radiobutton_dhcp.checked ?3 :4
            setSetterCmd(DeviceStruct.CMD_setIpv4 ,setting)
        }
    }

    property var regExp : /^((2[0-4]\d)|(25[0-5])|([01]\d{2})|(\d{1,2}))(\.((2[0-4]\d)|(25[0-5])|([01]\d{2})|(\d{1,2}))){0,3}$/
    property var setting:{
        "enable":true
        ,"address":"127.0.0.1"
        ,"addressMode":3//3 dhcp 4 manual
        ,"gatewayAddress":"127.0.0.1"
        ,"mode":0
        ,"subnetMask":"127.0.0.1"
        ,"powerSupply":JKEnums.PowerMode_unknown
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
                    textInput_gateway.text = setting.gatewayAddress
                    textInput_gateway.cursorPosition = 0
                    textInput_ipAddress.text = setting.address
                    textInput_ipAddress.cursorPosition = 0
                    textInput_submask.text = setting.subnetMask
                    textInput_submask.cursorPosition = 0
//                    if(setting.addressMode === 4)
//                        radiobutton_static.checked = true
//                    else
////                        if(setting.addressMode === 3)
//                        radiobutton_dhcp.checked = true
                    radiobutton_dhcp.checked = setting.addressMode !== 4
                }else{
                    textInput_gateway.text = "0.0.0.0"
                    textInput_ipAddress.text = "0.0.0.0"
                    textInput_submask.text = "0.0.0.0"
                    radiobutton_dhcp.checked = true
                }

                break;
            case DeviceStruct.CMD_setIpv4:
                break
            }
        }
    }

}
