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
    opacity: enabled ?1 :0.3

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
                text: qsTr("ResStr_IP_Type")
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
                    text:qsTr("ResStr_DHCP")
                    checked: true
                    opacity: enabled || !root.enabled ? 1.0 : 0.3
                }
                JKRadioButton{
                    id:radiobutton_static
                    width:143
                    height: parent.height
                    text:qsTr("ResStr_Static")
                    checked: !radiobutton_dhcp.checked
                    opacity: enabled || !root.enabled ? 1.0 : 0.3
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
                    text: qsTr("ResStr_IP_Address")
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
                    tooltip.text: text.match(regExp) && jkInterface.isIpv4(text)  ?"" :qsTr("ResStr_The_entered_IP_address__is_wrong__please_confirm_and_enter_again_")
                    onFocusChanged: {
                        if(!focus){
                            if(tooltip.text !== ""){
                                text = "0.0.0.0"
                            }
                        }
                    }
                    KeyNavigation.tab: textInput_submask
                    focus:radiobutton_static.checked
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
                    text: qsTr("ResStr_Submask")
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
                    tooltip.text: text.match(regExp) && jkInterface.isSubmask(text) ?"" :qsTr("ResStr_The_subnet_mask_input_error__please_input_again_after_confirmation")
                    onFocusChanged: {
                        if(!focus){
                            if(tooltip.text !== ""){
                                text = "255.255.255.0"
                            }
                        }
                    }
                }
                KeyNavigation.tab: textInput_gateway
            }

            Row {
                id: item6
                height: 60
                width: parent.width
//                spacing: 20
                JKText {
                    id: text5
                    width: 100
                    text: qsTr("ResStr_Gateway")
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
                    tooltip.text: text.match(regExp) && jkInterface.isIpv4(text) ?"" :qsTr("ResStr_The_entered_Gateway_is_wrong__please_confirm_and_enter_again_")
                    onFocusChanged: {
                        if(!focus){
                            if(tooltip.text !== ""){
                                text = "0.0.0.0"
                            }
                        }
                    }
                    KeyNavigation.tab: textInput_ipAddress
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
                text: qsTr("ResStr_Apply")
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
                warningWithImage(qsTr("ResStr_The_entered_IP_address__is_wrong__please_confirm_and_enter_again_"))
                textInput_ipAddress.forceActiveFocus()
//                textInput_ipAddress.focus = true
                return
            }
            if(!textInput_submask.text.match(regExp)){
                warningWithImage(qsTr("ResStr_The_subnet_mask_input_error__please_input_again_after_confirmation"))
                textInput_submask.forceActiveFocus()
//                textInput_submask.focus = true
                return
            }
            if(!textInput_gateway.text.match(regExp)){
                warningWithImage(qsTr("ResStr_The_entered_Gateway_is_wrong__please_confirm_and_enter_again_"))
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
        onCmdResultToUi:{
            if(phase !== JKEnums.CommandPhase_complelte)
                return
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
