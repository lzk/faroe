import QtQuick 2.0
import "component"
import "../component"
import "../ScanData.js" as JSData

Item {
    id: item1
    width: 495
    height: 460

    Column {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        Item {
            id: item3
            height: 50
            width: parent.width

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
            width: parent.width
        }
        
        Item {
            id: item2
            height: 20
            width: parent.width
        }
        
        Item {
            id: item4
            width: parent.width
            height: 50

            JKText {
                id: text2
                text: qsTr("Cloud Type:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            JKComboBox {
                id: comboBox_cloudType
                width: 200
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                model: JSData.constCloudType()
            }
        }
        Item {
            id: item5
            width: parent.width
            height: 50

//            JKText {
//                id: text3
//                text: qsTr("Reset access token in cacle:")
//                font.bold: true
//                anchors.left: parent.left
//                anchors.leftMargin: 30
//                anchors.verticalCenter: parent.verticalCenter
//                font.pixelSize: 12
//            }

            JKTextButton {
                id: button_reset
                text.text: qsTr("Reset")
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        DividingLine{
            height: 6
            width: parent.width
        }
        Item {
            height: 20
            width: parent.width
        }

        Item {
            id: item6
            width: parent.width
            height: 50

            JKText {
                id: text4
                text: qsTr("Email Attachment File Type:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            JKComboBox {
                id: comboBox_emailAttachmentFileType
                width: 200
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                model: JSData.constEmailAttachmentFileType()
            }
        }
    }

    property var scanParameter:scanData.scanParameter
    Component.onCompleted: {
        comboBox_cloudType.currentIndex = scanParameter.cloudType
        comboBox_emailAttachmentFileType.currentIndex = scanParameter.emailAttachmentFileType
    }

    JKTextButton{
        parent: item_btnSettings
        anchors.fill: parent
        text.text: qsTr("Settings...")
        onClicked: {
            openScanSettingDialog(scanParameter.scanSetting)
        }
    }
    Connections{
        target: comboBox_cloudType
        onActivated: scanParameter.cloudType = target.currentIndex
    }
    Connections{
        target: comboBox_emailAttachmentFileType
        onActivated: scanParameter.emailAttachmentFileType = target.currentIndex
    }
    Connections{
        target:button_reset
        onClicked: console.log("reset")
    }

    property var dialog
    function openScanSettingDialog(setting){
        dialog = openDialog("SettingsPage/ScanSetting/ScanSettingDialog.qml" ,{} ,function(dialog){
            dialog.initWithSetting(setting)
            })
    }
}
