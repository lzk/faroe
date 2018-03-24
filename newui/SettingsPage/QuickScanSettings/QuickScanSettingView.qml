import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../../component"
import "../component"
import "../../ScanData.js" as ScanData
Item {
    id:root
    width: 477
    height: 425
    property var setting: ScanData.defaultQuickScanSetting_toPrint()
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            id: item3
            height: 60
            Layout.fillWidth: true

            JKText {
                id: text1
                text: qsTr("Scan Setting:")
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
        text.text: qsTr("Settings...")
        onClicked: {
            openScanSettingDialog(setting.scanSetting)
        }
    }

    function init(){
        text_title.text = scanData.getQuickScanSettingTitle(setting)
        var source = ""
        switch(setting.sid){
        case "Scan To Print":   source = "ScanToPrintView.qml";  break
        case "Scan To File":   source = "ScanToFileView.qml";  break
        case "Scan To Application":   source = "ScanToApplicationView.qml";  break
        case "Scan To Email":   source = "ScanToEmailView.qml";  break
        case "Scan To FTP":   source = "ScanToFTPView.qml";  break
        case "Scan To Cloud":   source = "ScanToCloudView.qml";  break
        default:
            break
        }
        settingLoader.setSource(source ,{"setting":setting})
    }

    function ok(){
        return settingLoader.item.ok()
    }
}
