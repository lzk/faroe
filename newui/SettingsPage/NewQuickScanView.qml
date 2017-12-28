import QtQuick 2.0
import "../component"
import QtQuick.Layouts 1.3

Item {
    id:item
    width: 477
    height: 425
    property var setting:undefined
    Column {
        id: column
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 30
        anchors.rightMargin: 30

        Item {
            id: item3
            height: 60
            width: parent.width

            JKText {
                id: text1
                text: qsTr("Name:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput
                width: 250
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: item4
            height: 60
            width: parent.width

            JKText {
                id: text2
                text: qsTr("Type:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Item {
                id: item_comboxBox
                width: 250
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: item5
            height: 100
            width: parent.width

            JKText {
                id: text5
                text: qsTr("Scan Settings:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            Item {
                id: item_btnSettings
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    JKTextButton{
        parent: item_btnSettings
        anchors.fill: parent
        text.text: qsTr("Settings...")
        onClicked: {
            dialog.setting = setting
            dialog.mode = 0
            dialog.open()
        }
    }

    JKComboBox{
        id:comboBox
        parent: item_comboxBox
        anchors.fill: parent
        model:scanData.supportSid
        onActivated:{
            console.log("activate")
            setting = scanData.create(currentText)
        }
    }

    QuickScanDialog{
        id:dialog
    }

    Component.onCompleted: {
        setting = scanData.create(comboBox.currentText)
    }

    function ok(){
        if(setting == undefined)
            setting = scanData.create(comboBox.currentText)
        if(setting.name !== ""){
            scanData.add(setting)
        }
    }
}
