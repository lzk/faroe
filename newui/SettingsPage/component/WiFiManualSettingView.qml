import QtQuick 2.0
import "../../component"
import QtQuick.Controls 2.2
FocusScope{
    id:root
    signal returnClicked
    signal connectClicked(var setting)
    property alias textInput_ssid: textInput_ssid
    property alias textInput_password: textInput_password
    property alias combox_index: combobox.currentIndex
    Image {
        source: "qrc:/Images/setting_wifilist panel.png"
        anchors.fill:item_manual
        anchors.margins: -10
    }
    Item{
        id:item_manual
        width: parent.width - 20
        height: column.height
        anchors.centerIn: parent
        Column{
            id:column
            width: parent.width
            spacing: 10

            Row{
                height: 60
                spacing: 20
                Image{
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Wifi.png"
                }
                Column{
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10
                    JKText{
                        text:qsTr("ResStr_Find_and_join_the_Wi_Fi")
                    }
                    JKText{
                        text:qsTr("ResStr_Enter_SSID_and_security_type_")
                        font.pixelSize: 12
                    }
                }
            }

            Row {
                height: 30
                width: parent.width
                JKText {
                    id: text3
                    width: 100
                    text: qsTr("ResStr_SSID_2")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_ssid
                    width: 275
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /[^\s]{0,32}/
                    }
                    KeyNavigation.tab: textInput_password
                    focus:root.focus
                }
            }

            Row {
                height: 30
                width: parent.width
                enabled: combobox.currentIndex !== 0
                opacity: enabled ?1 :0.3

                JKText {
                    id: text4
                    width: 100
                    text: qsTr("ResStr_DocScan_password1")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_password
                    width: 275
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp:  combobox.currentIndex === 1 ?/^(?:.{5}|.{13}|[0-9a-fA-F]{10}|[0-9a-fA-F]{26})$/
                                                          :/^(?:.{8,63}|[0-9a-fA-F]{64})$/
                    }
                    echoMode:checkbox_input.checked ?TextInput.Normal :TextInput.Password
                    KeyNavigation.tab: textInput_ssid
                }
            }

            CheckBox{
                id:checkbox_input
                x: textInput_password.x
                text: qsTr("ResStr_Display_Password")
                enabled: combobox.currentIndex !== 0
                indicator: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 26
                    x: checkbox_input.leftPadding
                    anchors.verticalCenter: parent.verticalCenter
                    radius: 13
                    border.color: "lightgray"

                    Rectangle {
                        width: 14
                        height: 14
                        radius: 7
                        color: "#21be2b"
                        visible: checkbox_input.checked
                        anchors.centerIn: parent
                    }
                }
            }

            Row {
                width: parent.width
                height: 30
                JKText {
                    width: 100
                    text: qsTr("ResStr_Encryption")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKComboBox{
                    id:combobox
                    width: 275
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    model: [qsTr("ResStr_No_Security")
                        ,qsTr("WEP")
//                        ,qsTr("WPA PSK TKIP")
                        ,qsTr("WPA2-PSK-AES")
                        ,qsTr("Mixed Mode PSK")]
                }
            }

            Item{
                height: 30
                width: parent.width
                Row{
                    visible:combobox.currentIndex === 1
//                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 25
                    spacing: 2
                    JKText {
                        text: qsTr("ResStr_WEP_KeyID")
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }
                    JKRadioButton{
                        id:radio0
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Key1")
                        checked: true
                    }
                    JKRadioButton{
                        id:radio1
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Key2")
                    }
                    JKRadioButton{
                        id:radio2
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Key3")
                    }
                    JKRadioButton{
                        id:radio3
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Key4")
                    }
                }
            }


            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                height: 40
                spacing: 10
                JKTextButton{
                    width: root.width / 2 - 20
                    height: parent.height
                    text: qsTr("ResStr_Return")
                    onClicked: returnClicked()
                }

                JKTextButton{
                    width: root.width / 2 - 20
                    height: parent.height
                    text: qsTr("ResStr_Connect")
                    onClicked: {
                        if(!textInput_ssid.text.match(/^[\x20-\x7e]{1,32}$/)){
                            warningWithImage(qsTr("ResStr_Msg_9"))
//                            textInput_ssid.focus = true
                            textInput_ssid.forceActiveFocus()
                            return
                        }
                        var encryption = combobox.currentIndex > 1 ?combobox.currentIndex + 1 :combobox.currentIndex
                        switch(encryption){
                        case 1:
                            if(!textInput_password.text.match(/^(?:.{5}|.{13}|[0-9a-fA-F]{10}|[0-9a-fA-F]{26})$/)){
                                warningWithImage(qsTr("ResStr_Msg_2"))
//                                textInput_password.focus = true
                                textInput_password.forceActiveFocus()
                                return
                            }
                            break;
                        case 3:
                        case 4:
                            if(!textInput_password.text.match(/^(?:.{8,63}|[0-9a-fA-F]{64})$/)){
                                warningWithImage(qsTr("ResStr_Msg_3"))
//                                textInput_password.focus = true
                                textInput_password.forceActiveFocus()
                                return
                            }
                            break;
                        default:
                            break
                        }
                        var setting = {}
                        setting.enable = true
                        setting.type = 0
                        setting.channel = 0
                        setting.ssid = textInput_ssid.text
                        setting.password = textInput_password.text
                        setting.encryption = encryption
                        if(radio0.checked){
                            setting.wepKeyId = 0
                        }else if(radio1.checked){
                            setting.wepKeyId = 1
                        }else if(radio2.checked){
                            setting.wepKeyId = 2
                        }else if(radio3.checked){
                            setting.wepKeyId = 3
                        }
                        connectClicked(setting)
                    }
                }
            }

        }

    }
}
