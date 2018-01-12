import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import "component"
import "../component"
import "../ScanData.js" as JSData
import "../JSApi.js" as JSApi

Item {
    id: item1
    width: 495
    height: 460

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.bottomMargin: 10
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
            height: 130
            width: parent.width

            GroupBox {
                id: groupBox
                width: 400
                height: 112
                font.bold: true
                font.pixelSize: 12
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                title: qsTr("QRCode/BarCode Decode")

                Column {
                    width: 360
                    height: 90
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Item {
                        id: item4
                        width: parent.width
                        height: 45

                        JKText {
                            id: text2
                            text: qsTr("Code Type:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        JKComboBox {
                            id: comboBox_codeType
                            width: 200
                            height: 30
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            model: JSData.constQrcodeCodeType()
                        }
                    }
                    Item {
                        id: item5
                        width: parent.width
                        height: 45

                        JKText {
                            id: text3
                            text: qsTr("Output Result:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        JKTextInput {
                            id: input_outputResult
                            width: 200
                            height: 30
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }

        Item {
            id: item6
            height: 180
            width: parent.width

            GroupBox {
                id: groupBox1
                width: 400
                height: 177
                font.bold: true
                font.pixelSize: 12
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                title: qsTr("Document Separation")

                Column {
                    width: 360
                    height: 155
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Item {
                        id: item7
                        width: parent.width
                        height: 50

                        JKText {
                            id: text4
                            text: qsTr("Save File Type:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        JKComboBox {
                            id: comboBox_saveFileType
                            width: 200
                            height: 30
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            model: JSData.constQrcodeFileType()
                        }
                    }
                    Item {
                        id: item8
                        width: parent.width
                        height: 50

                        JKText {
                            id: text5
                            text: qsTr("File Path:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        JKTextInput {
                            id: input_filePath
                            width: 200
                            height: 30
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Item{
                        width: parent.width
                        height: 55
                        JKTextButton {
                            id: button_browser
                            text.text: qsTr("Browser...")
                            width: 100
                            height: 35
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Item{
            Layout.fillWidth: true
            height: 60
            JKTextButton {
                id: button_apply
                text.text: qsTr("Apply")
                width: 150
                height: 35
                anchors.centerIn: parent
            }
        }

    }

    property var qrcodeSetting
    Component.onCompleted: {
        qrcodeSetting = JSApi.deepCopy(scanData.qrcodeSetting)
        comboBox_codeType.currentIndex = qrcodeSetting.codeType
        input_outputResult.text = qrcodeSetting.outputResult
        comboBox_saveFileType.currentIndex = qrcodeSetting.fileType
        input_filePath.text = qrcodeSetting.filePath
    }

    JKTextButton{
        parent: item_btnSettings
        anchors.fill: parent
        text.text: qsTr("Settings...")
        onClicked: {
            openScanSettingDialog(qrcodeSetting.scanSetting)
        }
    }
    Connections{
        target: button_apply
        onClicked: {
            qrcodeSetting.codeType = comboBox_codeType.currentIndex
            qrcodeSetting.outputResult = input_outputResult.text
            qrcodeSetting.fileType = comboBox_saveFileType.currentIndex
            qrcodeSetting.filePath = input_filePath.text
            scanData.qrcodeSetting = qrcodeSetting
            warning({"message.text":qsTr("Configuration Completed!")
                        ,"message.horizontalAlignment":Text.AlignLeft
                        ,"toolbar.text.text":qsTr("Information")
                    ,"width": 520
                    ,"height": 200
                    })
        }
    }

    property var dialog
    function openScanSettingDialog(setting){
        dialog = openDialog("SettingsPage/ScanSetting/ScanSettingDialog.qml" ,{} ,function(dialog){
            dialog.initWithSetting(setting)
        })
    }
}
