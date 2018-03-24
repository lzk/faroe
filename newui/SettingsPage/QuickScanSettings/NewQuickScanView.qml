import QtQuick 2.0
import "../../component"
import QtQuick.Layouts 1.3
import "../../JSApi.js" as JSApi
Item {
    id:item
    width: 477
    height: 425
    property var settings : [undefined ,undefined ,undefined ,undefined ,undefined ,undefined]
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
                maximumLength: 20
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
            openModifyQuickScanSettingDialog(settings[comboBox.currentIndex])
        }
    }

    JKComboBox{
        id:comboBox
        parent: item_comboxBox
        anchors.fill: parent
        model:scanData.constQuickScanSids
        onActivated:{
            if(settings[index] === undefined){
                settings[index] = scanData.createQuickScanSetting(currentText)
            }
        }
    }

    function init(){
        if(mode === 1){
            settings[comboBox.currentIndex] = scanData.createQuickScanSetting(comboBox.currentText)
        }else{
            textInput.text = setting.name
            comboBox.currentIndex = scanData.constQuickScanSids.indexOf(setting.sid)
            settings[comboBox.currentIndex] = JSApi.deepCopy(setting)
        }
    }

    function ok(){
        var text = textInput.text
        if(text === ""){
//            warning({"message.text":qsTr("The Name cannot be empty!")
//                    ,"showImage": true
//                    ,"width": 520
//                    ,"height": 200})
            warningWithImage(qsTr("The Name cannot be empty!"))
            return false
        }else if(text.match(/^[\s]+$/)){
            warningWithImage(qsTr("The Name cannot be all space!"))
            return false
        }else{
            var texts = /\s*(.+?)\s*$/.exec(text)
            console.log(texts)
            text = texts[1]
        }

        if(settings[comboBox.currentIndex] === undefined){
            settings[comboBox.currentIndex] = scanData.createQuickScanSetting(comboBox.currentText)
        }
        settings[comboBox.currentIndex].name = text
        var verify = true
        if(mode !== 1 && settings[comboBox.currentIndex].name === setting.name){
            //edit do not change name
            verify = false
        }
        if(verify){
            if(!scanData.verifyQuickScanSettingName(settings[comboBox.currentIndex].name)){
                warning({"message.text":qsTr("Quick Scan item name already exists.change to another name!")
                        ,"showImage": true
                        ,"width": 520
                        ,"height": 200})
                return false
            }
        }
        setting = settings[comboBox.currentIndex]
        return true
    }
    property var dialog
    function openModifyQuickScanSettingDialog(setting){
        dialog = openDialog("SettingsPage/QuickScanSettings/QuickScanDialog.qml" ,{} ,function(dialog){
                dialog.initWithPara(setting ,0)
            })
    }
}
