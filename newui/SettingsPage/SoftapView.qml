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
                text: qsTr("Soft AP:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            Row{
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                Text{
                    text: qsTr("Close")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Local.JKCheckBox {
                    id: checkbox
                    width: 45
                    height: 22
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text{
                    text: qsTr("Open")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        Local.DividingLine{
            height: 6
            width: parent.width
        }

        Item {
            id: item2
            height: 80
            width: parent.width
            enabled: checkbox.checked
            opacity: enabled ?1 :0.3
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
                validator: RegExpValidator{
                    regExp: /^[\x20-\x7e]{1,32}$/
                }
            }
        }

        Item {
            id: item5
            height: 60
            width: parent.width
            enabled: checkbox.checked
            opacity: enabled ?1 :0.3
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
                validator: RegExpValidator{
                    regExp: /^(?:.{8,63}|[0-9a-fA-F]{64})$/
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
            if(!input_ssid.text.match(/^[\x20-\x7e]{1,32}$/)){
                warningWithImage(qsTr("The network name must be 1 to 32 characters long. Please check and enter again."))
                input_ssid.input.focus = true
                return
            }
            if(!input_password.text.match(/^(?:.{8,63}|[0-9a-fA-F]{64})$/)){
                warningWithImage(qsTr("The password must be 8 to 63 ASCII characters or 64 hex characters,please check and enter again."))
                input_password.input.focus = true
                return
            }
            setting.enable = checkbox.checked
            setting.ssid = input_ssid.text
            setting.password = input_password.text
            setSetterCmd(DeviceStruct.CMD_setSoftap ,setting)
        }
    }

    property var setting:{
        "enable":true
        ,"ssid":""
        ,"password":""
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
                    checkbox.checked = setting.enable
                    input_ssid.text = setting.ssid
                    input_password.text = setting.password
                }

                break;
            case DeviceStruct.CMD_setSoftap:
                break
            }
        }
    }
}
