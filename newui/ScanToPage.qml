import QtQuick 2.0
import QtQuick.Controls 2.2
import "ScanToPage"
import "component"
import "ImageViewer"
import "ScanData.js" as JSData

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
        target: button_toPrint
        onClicked: imageViewer.toPrint()
    }
    Connections{
        target: button_toFile
        onClicked: dialog = openDialog("component/JKFileDialog.qml" ,{} ,function(dialog){
            dialog.accepted.connect(saveFile)
        })
    }
    Connections{
        target: button_toEmail
        onClicked: imageViewer.toEmail(scanData.scanParameter.emailAttachmentFileType)
    }
    Connections{
        target: button_toApplication
//        onClicked: dialog = openDialog("component/JKFileDialog.qml" ,{} ,function(dialog){
//            dialog.accepted.connect(saveFile)
//        })
    }
    Connections{
        target: button_toFTP
        onClicked: dialog = openDialog("ScanToPage/FTPDialog.qml" ,{"setting":ftpSetting} ,function(dialog){
            dialog.accepted.connect(toFTP)
        })
    }
    property var ftpSetting: JSData.defaultFTPSettings()
    function toFTP(){
        imageViewer.toFTP(ftpSetting)
    }
    Connections{
        target: button_toCloud
        onClicked: imageViewer.toCloud(scanData.scanParameter)
    }
    function saveFile(){
        imageViewer.toFile(dialog.fileUrl)
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

    property var dialog
    function back(){
//        imageViewer.removeAllImages()
        root.StackView.view.pop()
    }
}
