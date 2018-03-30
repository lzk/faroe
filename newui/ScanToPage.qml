import QtQuick 2.0
import QtQuick.Controls 2.2
import "ScanToPage"
import "component"
import "ImageViewer"
import "ScanData.js" as JSData
import com.liteon.JKInterface 1.0
import QtQuick.Dialogs 1.2

ScanToPageLayout {
    id: root
    width: 750
    height: 533

    JKImageButton {
        id: button_back
        parent:item_back

        width: 50
        height: 50
        x:3
        y:2
        source_disable: "qrc:/Images/back_disable.png"
        source_normal: "qrc:/Images/back_normal.png"
        source_press: "qrc:/Images/back_press.png"
    }

    JKImageButton {
        id: button_down
        parent:item_down
        width: 50
        height: 50
        source_disable: "qrc:/Images/down_disable.png"
        source_normal: "qrc:/Images/down_normal.png"
        source_press: "qrc:/Images/down_press.png"
    }
    JKImageButton {
        id: button_up
        parent:item_up
        width: 50
        height: 50
        source_disable: "qrc:/Images/up_disable.png"
        source_normal: "qrc:/Images/up_normal.png"
        source_press: "qrc:/Images/up_press.png"
    }

    CheckBox{
        id:checkbox_selectall
        parent:item_title
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 5
        anchors.bottomMargin: 5
        text:qsTr("Select All")
        checked: imageViewer.allSelected
    }

    JKFunctionButton{
        id:button_toPrint
        parent: item_toPrint
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 70
        height: 100
        jktext.text: qsTr("To Print")
        jktext.font.pixelSize: 12
        source_normal: "qrc:/Images/ScanToPrint.png"
        source_press: "qrc:/Images/ScanToPrint.png"
    }
    JKFunctionButton{
        id:button_toEmail
        parent: item_toEmail
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 70
        height: 100
        jktext.text: qsTr("To Email")
        jktext.font.pixelSize: 12
        source_normal: "qrc:/Images/ScanToEmail.png"
        source_press: "qrc:/Images/ScanToEmail.png"
    }
    JKFunctionButton{
        id:button_toFile
        parent: item_toFile
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 70
        height: 100
        jktext.text: qsTr("To File")
        jktext.font.pixelSize: 12
        source_normal: "qrc:/Images/ScanToFile.png"
        source_press: "qrc:/Images/ScanToFile.png"
    }

    JKFunctionButton{
        id:button_toFTP
        parent: item_toFTP
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 70
        height: 100
        jktext.text: qsTr("To FTP")
        jktext.font.pixelSize: 12
        source_normal: "qrc:/Images/ScanToFtp.png"
        source_press: "qrc:/Images/ScanToFtp.png"
    }
    JKFunctionButton{
        id:button_toApplication
        parent: item_toApplication
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 70
        height: 100
        jktext.text: qsTr("To Application")
        jktext.font.pixelSize: 12
        source_normal: "qrc:/Images/ScanToProgram.png"
        source_press: "qrc:/Images/ScanToProgram.png"
    }
    JKFunctionButton{
        id:button_toCloud
        parent: item_toCloud
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 70
        height: 100
        jktext.text: qsTr("To Cloud")
        jktext.font.pixelSize: 12
        source_normal: "qrc:/Images/ScanToCloud.png"
        source_press: "qrc:/Images/ScanToCloud.png"
    }

    ImageViewer{
        id:imageViewer
        parent: item_view
        anchors.fill: parent
    }

    Connections{
        target: button_up
        onClicked: imageViewer.moveup()
    }

    Connections{
        target: button_down
        onClicked: imageViewer.moveDown()
    }

    Connections{
        target: button_toPrint
        onClicked: {
            if(imageViewer.selectList.length < 1){
                warning_noSelectedFiles()
            }else{
                setScanToCmd(DeviceStruct.CMD_ScanTo_ToPrint ,imageViewer.selectList)
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Save As")
        folder: shortcuts.pictures
        nameFilters: JSData.constFileDialogSaveFileType()
        selectExisting:false
        onAccepted: {
            var setting = fileUrl.toString().replace("file:///" ,"/")
            setScanToCmd(DeviceStruct.CMD_ScanTo_ToFile ,imageViewer.selectList ,setting)
        }
    }
//    function saveFile(fileUrl){
//        jkInterface.setScanToCmd(DeviceStruct.CMD_ScanTo_ToFile ,imageViewer.selectList ,fileUrl.toString().replace("file:///" ,"/"))
//    }

    Connections{
        target: button_toFile
        onClicked:{
            if(imageViewer.selectList.length < 1){
                warning_noSelectedFiles()
            }else{
                fileDialog.open()
            }
        }
    }

    Connections{
        target: button_toEmail
        onClicked: {
            if(imageViewer.selectList.length < 1){
                warning_noSelectedFiles()
            }else{
                var setting = JSData.defaultEmailSettings()
                setting.fileType = scanData.scanToParameter.emailAttachmentFileType
                setScanToCmd(DeviceStruct.CMD_ScanTo_ToEmail ,imageViewer.selectList ,setting)
            }
        }
    }

    Connections{
        target: button_toApplication
        onClicked: {
            if(imageViewer.selectList.length < 1){
                warning_noSelectedFiles()
            }else{
                var setting =scanData.applicationSetting
                dialog = openDialog("ScanToPage/ApplicationDialog.qml" ,{} ,function(dialog){
                                dialog.setting = setting
                                dialog.accepted.connect(toApplication)
                            })
            }
        }
    }
    function toApplication(){
        var setting = scanData.applicationSetting
        setScanToCmd(DeviceStruct.CMD_ScanTo_ToApplication ,imageViewer.selectList ,setting)
    }

    Connections{
        target: button_toFTP
        onClicked: {
            if(imageViewer.selectList.length < 1){
                warning_noSelectedFiles()
            }else{
                var setting = scanData.ftpSetting
                dialog = openDialog("ScanToPage/FTPDialog.qml" ,{"setting":setting} ,function(dialog){
                                dialog.accepted.connect(toFTP)
                            })
            }
        }
    }

    function toFTP(){
        var setting = scanData.ftpSetting
        setScanToCmd(DeviceStruct.CMD_ScanTo_ToFTP ,imageViewer.selectList ,setting)
    }
    Connections{
        target: button_toCloud
        onClicked: {
            if(imageViewer.selectList.length < 1){
                warning_noSelectedFiles()
            }else{
                var setting ={}
                setting.cloudTypeText = scanData.scanToParameter.cloudTypeText
                setting.callback = toCloud
                setting.okButtonText = qsTr("Upload")
                setCmdExtra(DeviceStruct.CMD_Cloud_getFileList ,setting)
            }
        }
    }

    function toCloud(setting){
        var cloudTypes = JSData.supportCloudType()
        switch(setting.cloudTypeText){
        case cloudTypes.icloud:
            var setting_new = {}
            setting_new.cloudTypeText = setting.cloudTypeText
            setting_new.selectList = jkImageModel.getFileList(imageViewer.selectList)
            setCmdExtra(DeviceStruct.CMD_Cloud_isExist ,setting_new)
//            setScanToCmd(DeviceStruct.CMD_ScanTo_ToCloud ,imageViewer.selectList ,setting)
            break
        }
    }

    Connections{
        target: checkbox_selectall
        onCheckedChanged:imageViewer.selectAll(checkbox_selectall.checked)
    }

    Connections{
        target: button_back
        onClicked:{
            information(qsTr("Do you want leave this page ,if you exit ,all the images will be deleted?") ,back)
        }
    }

    function warning_noSelectedFiles(){
        information_1button(qsTr("please select one or more pictures to process!"))
    }

    property var dialog
    function back(){
        imageViewer.removeAllImages()
        root.StackView.view.pop()

    }
}
