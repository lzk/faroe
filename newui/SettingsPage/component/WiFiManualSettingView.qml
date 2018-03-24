import QtQuick 2.0
import "../../component"
import QtQuick.Controls 2.2
Item{
    id:root
    signal returnClicked
    signal connectClicked(var setting)
    property alias input_ssid: input_ssid
    property alias input_password: input_password
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
                spacing: 5
                Image{
                    source: "qrc:/Images/Wifi.png"
                }
                Column{
                    JKText{
                        text:qsTr("Find and join the Wi-Fi")
                    }
                    JKText{
                        text:qsTr("Enter SSID and security type.")
                        font.pixelSize: 12
                    }
                }
            }

            Item {
                height: 30
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
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp: /[^\s]{0,32}/
                    }
                }
            }

            Item {
                height: 30
                width: parent.width
                enabled: combobox.currentIndex !== 0
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
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    validator: RegExpValidator{
                        regExp:  combobox.currentIndex === 1 ?/^(?:.{5}|.{13}|[0-9a-fA-F]{10}|[0-9a-fA-F]{26})$/
                                                          :/^(?:.{8,63}|[0-9a-fA-F]{64})$/
                    }
                    echoMode:checkbox_input.checked ?TextInput.Normal :TextInput.Password
                }
            }

            CheckBox{
                id:checkbox_input
                text: qsTr("Display Password")
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

            Item {
                width: parent.width
                height: 30
                JKText {
                    text: qsTr("Encryption")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKComboBox{
                    id:combobox
                    width: 275
                    height: 30
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    model: [qsTr("No Security")
                        ,qsTr("WEP")
//                        ,qsTr("WPA PSK TKIP")
                        ,qsTr("WPA2-PSK-AES")
                        ,qsTr("Mixed Mode PSK")]
                }
            }

            Row{
                visible:combobox.currentIndex === 1
                anchors.horizontalCenter: parent.horizontalCenter
                height: 30
                spacing: 2
                JKText {
                    text: qsTr("WEP KeyID")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }
                JKRadioButton{
                    id:radio0
                    text: qsTr("Key1")
                    checked: true
                }
                JKRadioButton{
                    id:radio1
                    text: qsTr("Key2")
                }
                JKRadioButton{
                    id:radio2
                    text: qsTr("Key3")
                }
                JKRadioButton{
                    id:radio3
                    text: qsTr("Key4")
                }
            }

            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                height: 40
                spacing: 10
                JKTextButton{
                    width: root.width / 2 - 20
                    height: parent.height
                    text.text: qsTr("Return")
                    onClicked: returnClicked()
                }

                JKTextButton{
                    width: root.width / 2 - 20
                    height: parent.height
                    text.text: qsTr("Connect")
                    onClicked: {
                        if(!input_ssid.text.match(/^[\\x0020-\\x007e]{1,32}$/)){
                            warningWithImage(qsTr("The network name must be 1 to 32 characters long. Please check and enter again."))
                            input_ssid.input.focus = true
                            return
                        }
                        var encryption = combobox.currentIndex > 1 ?combobox.currentIndex + 1 :combobox.currentIndex
                        switch(encryption){
                        case 1:
                            if(!input_password.text.match(/^(?:.{5}|.{13}|[0-9a-fA-F]{10}|[0-9a-fA-F]{26})$/)){
                                warningWithImage(qsTr("The password must be 5 or 13 ASCII characters or 10 or 26 hex characters,please check and enter again."))
                                input_password.input.focus = true
                                return
                            }
                            break;
                        case 3:
                        case 4:
                            if(!input_password.text.match(/^(?:.{8,63}|[0-9a-fA-F]{64})$/)){
                                warningWithImage(qsTr("The password must be 8 to 63 ASCII characters or 64 hex characters,please check and enter again."))
                                input_password.input.focus = true
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
                        setting.ssid = input_ssid.text
                        setting.password = input_password.text
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
