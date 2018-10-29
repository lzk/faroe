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


            Item {
                id: item_btnCalibration
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10
            }
        }

        Local.DividingLine{
            height: 6
            width: parent.width
        }

        Item{
            height: 120
            width: parent.width
            Item{
                anchors.fill: parent
                anchors.rightMargin: parent.width / 2

                GroupBox {
                    id: groupBox
                    anchors.fill: parent

                    font.bold: true
                    font.pixelSize: 12
                    spacing: 0
                    title: qsTr("ResStr_Sleep_Time")

                    Column {
                        anchors.fill: parent

                        Item {
                            id: item4
                            width: parent.width
                            height: 50
                            Local.SpinView{
                                id:spin_saveTime
                                anchors.fill: parent
                                anchors.leftMargin: 20
                                anchors.rightMargin: 20
                                slider.visible: false
                                input.validator: RegExpValidator { regExp: /([0-9])|([1-5]\d)|(60)/ }
                                from: 1
                                to:60
                                value: 0
                                maximumLength: 2
                                tooltip.text: text >= 1 ?"": qsTr("ResStr_The_valid_range_is_1_60__please_confirm_and_enter_again_")
                            }
                        }
                        JKText {
                            id: text3
                            text: qsTr("ResStr_1_60_Minute")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 14
                        }
                    }
                }
            }

            Item{
                anchors.fill: parent
                anchors.leftMargin: parent.width / 2

                GroupBox {
                    id: groupBox1
                    anchors.fill: parent

                    font.bold: true
                    font.pixelSize: 12
                    spacing: 0
                    title: qsTr("ResStr_AutoOff_Time")

                    Column {
                        anchors.fill: parent
                        Item {
                            id: item6
                            width: parent.width
                            height: 50
                            Local.SpinView{
                                id:spin_offTime
                                anchors.fill: parent
                                anchors.leftMargin: 20
                                anchors.rightMargin: 20
                                slider.visible: false
                                to :4
                                value :0
                                maximumLength: 1
                                tooltip.text:  text !== "" ?"": qsTr("ResStr_The_valid_range_is_0_4__please_confirm_and_enter_again_")
                            }
                        }
                        JKText {
                            id: text4
                            text: qsTr("ResStr_0_4_Hour")
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 14
                        }
                    }
                }
            }
        }

        Item{
            height: 140
            Layout.fillWidth: true
            GroupBox {
                id: groupBox2
                anchors.fill: parent

                font.bold: true
                font.pixelSize: 12
                spacing: 0
                title: qsTr("ResStr_Life_Count")

                Column {
                    anchors.fill: parent
                    RowLayout{
                        width: parent.width
                        height: parent.height / 3
                        Item{
                            width: 140
                            height: parent.height
                            JKText{
                                font.pixelSize: 12
                                text:qsTr("ResStr_Roller_Count")
                                anchors.verticalCenter: parent.verticalCenter
                                x:5
                            }
                        }
                        Item{
                            width: 80
                            height: parent.height
                            JKText{
                                id:text_rollerCount
                                font.pixelSize: 12
                                anchors.centerIn: parent
                                text:setting.rollerCount
                            }
                        }
                        Item{
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            JKTextButton{
                                id:button_clearRollerCount
                                text: qsTr("ResStr_Clear")
                                width: 100
                                height: 24
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right

                            }
                        }
                    }
                    RowLayout{
                        width: parent.width
                        height: parent.height / 3
                        Item{
                            width: 140
                            height: parent.height
                            JKText{
                                font.pixelSize: 12
                                text:qsTr("ResStr_ACM_Count")
                                anchors.verticalCenter: parent.verticalCenter
                                x:5
                            }
                        }
                        Item{
                            width: 80
                            height: parent.height
                            JKText{
                                id:text_ACMCount
                                font.pixelSize: 12
                                anchors.centerIn: parent
                                text:setting.acmCount
                            }
                        }
                        Item{
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            JKTextButton{
                                id:button_clearACMCount
                                text: qsTr("ResStr_Clear")
                                width: 100
                                height: 24
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right

                            }
                        }
                    }
                    RowLayout{
                        width: parent.width
                        height: parent.height / 3
                        Item{
                            width: 140
                            height: parent.height
                            JKText{
                                font.pixelSize: 12
                                text:qsTr("ResStr_Scan_Count")
                                anchors.verticalCenter: parent.verticalCenter
                                x:5
                            }
                        }
                        Item{
                            width: 80
                            height: parent.height
                            JKText{
                                id:text_scanCount
                                font.pixelSize: 12
                                anchors.centerIn: parent
                                text:setting.scanCount
                            }
                        }
                        Item{
                            Layout.fillHeight: true
                            Layout.fillWidth: true
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
                enabled:(spin_saveTime.text !== ""
                          && spin_offTime.text !== ""
                          &&(((Math.floor(Number.fromLocaleString(Qt.locale(), spin_saveTime.text)) !== setting.saveTime)
                          && (Math.floor(Number.fromLocaleString(Qt.locale(), spin_saveTime.text)) !== 0))
                         ||(Math.floor(Number.fromLocaleString(Qt.locale(), spin_offTime.text)) !== setting.offTime)))
                text: qsTr("ResStr_Apply")
                width: 150
                height: 35
                anchors.centerIn: parent
            }
        }

    }

    JKTextButton{
        parent: item_btnCalibration
        anchors.fill: parent
        text: qsTr("ResStr_Calibration")
        onClicked: {
            setSetterCmd(DeviceStruct.CMD_doCalibration ,setting)
        }
        enabled: scanData.currentDevice.match(/^usb+/i) && setting.powerSupply === JKEnums.PowerMode_ACPower
    }
    Connections{
        target: button_apply
        onClicked: {
            setting.saveTime = Math.floor(spin_saveTime.value)
            setting.offTime = Math.floor(spin_offTime.value)
            if(setting.saveTime < 1)
                setting.saveTime = 1;
            if(setting.saveTime > 60)
                setting.saveTime = 60;
            if(setting.offTime < 0)
                setting.offTime = 0;
            if(setting.offTime > 4)
                setting.offTime = 4;
            spin_saveTime.value = setting.saveTime
            spin_offTime.value = setting.offTime
            setting.offTime *= 60
            setSetterCmd(DeviceStruct.CMD_setPowerSaveTime ,setting)
        }
    }

    property var setting:{
        "saveTime":0,
        "offTime":0,
        "rollerCount":0,
        "acmCount": 0,
        "scanCount": 0,
        "powerSupply": JKEnums.PowerMode_ACPower
    }
    Component.onCompleted: {
        setSetterCmd(DeviceStruct.CMD_getDeviceSetting ,setting)
    }
    Connections{
        target: jkInterface
        onCmdResultToUi:{
            if(phase !== JKEnums.CommandPhase_complelte)
                return
            switch(cmd){
            case DeviceStruct.CMD_getDeviceSetting:
                if(!result){
                    setting = JSON.parse(data)
                    console.log(data)
                    setting.offTime /= 60

                    if(setting.saveTime < 1)
                        setting.saveTime = 1;
                    if(setting.saveTime > 60)
                        setting.saveTime = 60;
                    if(setting.offTime < 0)
                        setting.offTime = 0;
                    if(setting.offTime > 4)
                        setting.offTime = 4;

                    spin_saveTime.value = setting.saveTime
                    spin_offTime.value = setting.offTime

                }
                break;
            case DeviceStruct.CMD_clearACMCount:
                if(!result){
                    setting.acmCount = 0
                    setting = setting
                }
                break
            case DeviceStruct.CMD_clearRollerCount:
                if(!result){
                    setting.rollerCount = 0
                    setting = setting
                }
                break;
            }
        }
    }
    Connections{
        target: button_clearACMCount
        onClicked:{
            information(qsTr("ResStr_Do_you_want_to_clear") ,clearACMCount)
        }
    }
    function clearACMCount(){
        setSetterCmd(DeviceStruct.CMD_clearACMCount ,setting)
    }

    function clearRollerCount(){
        setSetterCmd(DeviceStruct.CMD_clearRollerCount ,setting)
    }
    Connections{
        target: button_clearRollerCount
        onClicked:{
            information(qsTr("ResStr_Do_you_want_to_clear") ,clearRollerCount)
        }
    }
}
