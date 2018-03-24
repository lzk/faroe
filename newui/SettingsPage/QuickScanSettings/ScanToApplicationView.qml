import QtQuick 2.0
import QtQuick.Controls 2.2
import "../../component"
import QtQuick.Dialogs 1.2
import "../../ScanData.js" as ScanData
Item {
    width: 477
    height: 309
    property var setting:ScanData.defaultApplicationSettings()

    Column{
        anchors.fill: parent
        Item {
            id: item1
            width: parent.width
            height: 60

            JKText {
                id: text1
                text: qsTr("Programs:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKComboBox {
                id: comboBox
                width: 250
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                model: ["Preview" ,"Other Application"]
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60


            JKTextInput {
                id: textInput2
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                visible: comboBox.currentIndex === 1
                input.readOnly:true
            }
        }
        Item {
            id: item3
            width: parent.width
            height: 60

            JKTextButton {
                id: button_browse
                text.text: qsTr("Browser...")
                width: 100
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: fileDialog.open()
                visible: comboBox.currentIndex === 1
            }
        }
    }

    Connections{
        target: comboBox
        onActivated:{
//            if(index !== 0){
//                openSettingDialog(setting)
//            }
        }
    }

    Component.onCompleted:{
        init()
    }

    function init(){
//        var index = -1
//        if(comboBox.count > 0)
//            index = 0
//        for (var i=0; i<comboBox.count; i++){
//            if(comboBox.textAt(i) === setting.fileName){
//                index = i
//            }
//        }
//        comboBox.currentIndex = index
        comboBox.currentIndex = setting.fileType >= 1 ?1 :0
        textInput2.text = setting.fullFileName
    }
    function ok(){
        if(comboBox.currentIndex !== 0){

            if(textInput2.text.lastIndexOf(".app") === textInput2.text.length - 4){
                setting.fullFileName = textInput2.text
            }else{
                warningWithImage(qsTr("Not application!"))
                return false
            }
        }else{
            setting.fullFileName = "/Applications/Preview.app"
        }
        setting.fileType = comboBox.currentIndex
        return true
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Applications")
        folder: "file:///Applications"
        nameFilters: ["APP (*.app)"]
//        selectFolder: true
        onAccepted: {
            textInput2.text = fileUrl.toString().replace("file:///" ,"/")
        }
        onRejected: {
        }
    }
}
