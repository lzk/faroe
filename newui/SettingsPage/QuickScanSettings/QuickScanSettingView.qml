import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../../component"
import "../component"
import "../../ScanData.js" as JSData
Item {
    id:root
    width: 477
    height: 425
    property var setting: JSData.defaultQuickScanSetting_toPrint()
    property var constScanIds: JSData.constScanIds()
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            id: item3
            height: 60
            Layout.fillWidth: true

            JKText {
                id: text1
                text: qsTr("ResStr_DocScan_scan_setting")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            Item {
                id: item_btnSettings
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        DividingLine{
            height: 6
            Layout.fillWidth: true
        }

        Item {
            id: item_view
            Layout.fillHeight: true
            Layout.fillWidth: true
            Item{
                anchors.fill: parent
                anchors.bottomMargin: parent.height - 50
                JKText{
                    id:text_title
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 14
                    font.bold: true
                }
            }
            Loader{
                id:settingLoader
                anchors.fill: parent
                anchors.topMargin: 50
                anchors.leftMargin: 30
            }
        }
    }

    JKTextButton{
        parent: item_btnSettings
        anchors.fill: parent
        text: qsTr("ResStr_Setting")
        onClicked: {
            openScanSettingDialog(setting.scanSetting)
        }
    }

    function init(){
        var source = ""
        switch(setting.sid){
        case constScanIds.scanToPrint:          text_title.text = qsTr("ResStr_DocScan_print_setting") ;source = "ScanToPrintView.qml";  break
        case constScanIds.scanToFile:           text_title.text = qsTr("ResStr_DocScan_file_setting") ;source = "ScanToFileView.qml";  break
        case constScanIds.scanToApplication:    text_title.text = qsTr("ResStr_DocScan_app_setting") ;source = "ScanToApplicationView.qml";  break
        case constScanIds.scanToEmail:          text_title.text = qsTr("ResStr_DocScan_email_setting") ;source = "ScanToEmailView.qml";  break
        case constScanIds.scanToFTP:            text_title.text = qsTr("ResStr_DocScan_ftp_setting") ;source = "ScanToFTPView.qml";  break
        case constScanIds.scanToCloud:          text_title.text = qsTr("ResStr_DocScan_cloud_setting") ;source = "ScanToCloudView.qml";  break
        default:
            break
        }
        settingLoader.setSource(source ,{"setting":setting})
//        settingLoader.item.init()
    }

    function ok(){
        return settingLoader.item.ok()
    }
}
