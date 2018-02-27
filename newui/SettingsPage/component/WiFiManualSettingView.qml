import QtQuick 2.0
import "../../component"
import QtQuick.Controls 2.2
Item{
    id:root
    signal returnClicked
    signal connectClicked(var setting)
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
                    input.validator: RegExpValidator{
                        regExp: /[^\s]{0,32}/
                    }
                }
            }

            Item {
                height: 30
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
                    anchors.rightMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    input.validator: RegExpValidator{
                        regExp: /.{0,64}/
                    }
                    input.echoMode:checkbox_input.checked ?TextInput.Normal :TextInput.Password
                }
            }

            CheckBox{
                id:checkbox_input
                text: qsTr("Display Password")
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
                RadioButton{
                    id:radio0
                    text: qsTr("Key0")
                    checked: true
                }
                RadioButton{
                    id:radio1
                    text: qsTr("Key1")
                }
                RadioButton{
                    id:radio2
                    text: qsTr("Key2")
                }
                RadioButton{
                    id:radio3
                    text: qsTr("Key3")
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
                        var setting = {}
                        setting.enable = true
                        setting.type = 0
                        setting.channel = 0
                        setting.ssid = input_ssid.text
                        setting.password = input_password.text
                        setting.encryption = combobox.currentIndex > 1 ?combobox.currentIndex + 1 :combobox.currentIndex
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
