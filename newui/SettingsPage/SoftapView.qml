import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import "component" as Local
import "../component"
import com.liteon.JKInterface 1.0

Item {
    id: root
    width: 495
    height: 460
    enabled: scanData.deviceStatus && setting.wifiEnable && setting.powerSupply !== JKEnums.PowerMode_usbBusPower
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
                text: qsTr("Soft AP:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            Local.JKCheckBox {
                id: checkbox
                width: 45
                height: 22
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5
                opacity: enabled || !root.enabled ?1 :0.3
            }
//            Row{
//                anchors.right: parent.right
//                anchors.rightMargin: 10
//                anchors.verticalCenter: parent.verticalCenter
//                spacing: 5
//                Text{
//                    text: qsTr("Close")
//                    anchors.verticalCenter: parent.verticalCenter
//                }
//                Local.JKCheckBox {
//                    id: checkbox
//                    width: 45
//                    height: 22
//                    anchors.verticalCenter: parent.verticalCenter
//                }
//                Text{
//                    text: qsTr("Open")
//                    anchors.verticalCenter: parent.verticalCenter
//                }
//            }
        }

        Local.DividingLine{
            height: 6
            width: parent.width
        }

        Item{
            width: parent.width
            height: 40
        }

        Column{
            height: 120
            width: parent.width
            Row {
                id: item2
                height: 60
                width: parent.width
                enabled: checkbox.checked
                opacity: enabled || !root.enabled ?1 :0.3
                JKText {
                    id: text3
                    width: 100
                    text: qsTr("SSID")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_ssid
                    width: 275
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /^[\x20-\x7e]{1,32}$/
                    }
                }
            }

            Row {
                id: item5
                height: 60
                width: parent.width
                enabled: checkbox.checked
                opacity: enabled || !root.enabled ?1 :0.3
                visible: false
                JKText {
                    id: text4
                    width: 100
                    text: qsTr("Password")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_password
                    width: 275
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /^(?:.{8,63}|[0-9a-fA-F]{64})$/
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
            }
        }

    }

    Connections{
        target: button_apply
        onClicked: {
            if(!textInput_ssid.text.match(/^[\x20-\x7e]{1,32}$/)){
                warningWithImage(qsTr("The SSID must be 1 to 32 characters long. Please check and enter again."))
                textInput_ssid.forceActiveFocus()
//                textInput_ssid.focus = true
                return
            }
//            if(!textInput_password.text.match(/^(?:.{8,63}|[0-9a-fA-F]{64})$/)){
//                warningWithImage(qsTr("The Password must be 8 to 63 ASCII characters or 64 hex characters,please check and enter again."))
//                textInput_password.forceActiveFocus()
////                textInput_password.focus = true
//                return
//            }
            setting.enable = checkbox.checked
            setting.ssid = textInput_ssid.text
            setting.password = textInput_password.text
            setSetterCmd(DeviceStruct.CMD_setSoftap ,setting)
        }
    }

    property var setting:{
        "enable":true
        ,"wifiEnable":true
        ,"ssid":""
        ,"password":""
        ,"powerSupply":JKEnums.PowerMode_unknown
    }
    Component.onCompleted: {
        setSetterCmd(DeviceStruct.CMD_getSoftap ,setting)
    }
    Connections{
        target: jkInterface
        onCmdResult:{
            switch(cmd){
            case DeviceStruct.CMD_getSoftap:
                if(!result){
                    setting = JSON.parse(data)
                    console.log(data)
                    if(setting.wifiEnable){
                        checkbox.checked = setting.enable
                        textInput_ssid.text = setting.ssid
                        textInput_ssid.cursorPosition = 0
                        textInput_password.text = ""//setting.password
                    }else{
                        checkbox.checked = false
                        textInput_ssid.text = ""
                        textInput_password.text = ""
                    }
                }else{
                    console.log("fail")
                    setting.wifiEnable = false
                    checkbox.checked = false
                    textInput_ssid.text = ""
                    textInput_password.text = ""
                    setting = setting
                }
                break;
            case DeviceStruct.CMD_setSoftap:
                break
            }
        }
    }
}
