import QtQuick 2.0
import QtQuick.Dialogs 1.2
import "../../component"
import "../../ScanData.js" as JSData
Item {
    width: 477
    height: 309

    Column{
        anchors.fill: parent
        Item {
            id: item1
            width: parent.width
            height: 60

            JKText {
                id: text1
                text: qsTr("Save File Type:")
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
                model: ["PDF" ,"TIFF" ,"JPG" ,"BMP"]
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60

            JKText {
                id: text2
                text: qsTr("File Name:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput2
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 32
            }
        }
        Item {
            id: item3
            width: parent.width
            height: 60

            JKText {
                id: text3
                text: qsTr("File Path:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput3
                enabled: false
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Item {
            id: item4
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
            }
        }
    }
    FileDialog {
        id: fileDialog
        title: qsTr("Save As")
//        folder: shortcuts.pictures
        folder: "file://" + JSData.defaultFilePath()
        nameFilters: JSData.constFileDialogSaveFileType()
        selectFolder: true
        onAccepted: textInput3.text = fileUrl.toString().replace("file:///" ,"/")
    }


    property var setting
    Component.onCompleted:{
        init()
    }

    function init(){
        comboBox.currentIndex = setting.fileType
        textInput2.text = setting.fileName
        textInput3.text = setting.filePath
//        .replace("~" ,jkInterface.homeDictory())
    }
    function ok(){
        var fileType = comboBox.currentIndex
        var fileName = textInput2.text
        var filePath = textInput3.text
        if(filePath === ""){
            if(fileName === ""){
                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("File Path and File Name")))
                textInput2.input.focus = true
            }else{
                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("File Path")))
                textInput3.input.focus = true
            }
            return false
        }else if(fileName === ""){
            warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("File Name")))
            textInput2.input.focus = true
            return false
        }else if(!filePath.match(/^[^\\\?\*:<>|\"\(\)\[\]]*$/)){
//            if(!fileName.match(/^[0-9a-zA-Z\-_.]*$/)){
            if(!fileName.match(/^[^\/\\\?\*:<>|\"\(\)\[\]]*$/)){
                warningWithImage(qsTr("Invalid %1!").arg(qsTr("File Name and File Path")))
                textInput2.input.focus = true
            }else{
                warningWithImage(qsTr("Invalid %1!").arg(qsTr("File Path")))
                textInput3.input.focus = true
            }
            return false
        }else if(!fileName.match(/^[^\/\\\?\*:<>|\"\(\)\[\]]*$/)){
//        }else if(!fileName.match(/^[0-9a-zA-Z\-_.]*$/)){
            warningWithImage(qsTr("Invalid %1!").arg(qsTr("File Name")))
            textInput2.input.focus = true
            return false
        }
        setting.fileType = fileType
        setting.fileName = fileName
        setting.filePath = filePath
        return true
    }
}
