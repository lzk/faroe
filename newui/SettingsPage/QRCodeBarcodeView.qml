import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import "component"
import "../component"
import QtQuick.Dialogs 1.2
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
            id: item2
            height: 180
            width: parent.width

            GroupBox {
                id: groupBox
                width: 400
                height: 162
                font.bold: true
                font.pixelSize: 12
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                title: qsTr("QRCode/BarCode Decode")

                Column {
                    width: 360
                    height: 150
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Item {
                        id: item3
                        height: 50
                        width: parent.width

                        JKText {
                            id: text1
                            text: qsTr("Scan Settings:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        Item {
                            id: item_btnSettings_decode
                            width: 100
                            height: 35
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

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
                            id: input_fileName
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
            height: 230
            width: parent.width

            GroupBox {
                id: groupBox1
                width: 400
                height: 227
                font.bold: true
                font.pixelSize: 12
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                title: qsTr("Document Separation")

                Column {
                    width: 360
                    height: 205
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Item {
                        id: item7
                        height: 50
                        width: parent.width

                        JKText {
                            id: text4
                            text: qsTr("Scan Settings:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        Item {
                            id: item_btnSettings_separation
                            width: 100
                            height: 35
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    Item {
                        id: item8
                        width: parent.width
                        height: 50

                        JKText {
                            id: text5
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
                        id: item9
                        width: parent.width
                        height: 50

                        JKText {
                            id: text6
                            text: qsTr("File Path:")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 12
                        }

                        JKTextInput {
                            id: input_filePath
                            enabled: false
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
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Save As")
        folder: "file://" + JSData.defaultFilePath()
        nameFilters: JSData.constFileDialogSaveFileType()
        selectFolder: true
        onAccepted: input_filePath.text = decodeURIComponent(fileUrl).replace("file:///" ,"/")
    }
    Connections{
        target: button_browser
        onClicked:fileDialog.open()
    }

    JKTextButton{
        parent: item_btnSettings_decode
        anchors.fill: parent
        text.text: qsTr("Settings...")
        onClicked: {
            openScanSettingDialog(decodeSetting.scanSetting ,1)
        }
    }
    JKTextButton{
        parent: item_btnSettings_separation
        anchors.fill: parent
        text.text: qsTr("Settings...")
        onClicked: {
            openScanSettingDialog(separationSetting.scanSetting ,2)
        }
    }

    property var decodeSetting :scanData.decodeSetting
    property var separationSetting :scanData.separationSetting
    Component.onCompleted: {
//        console.log("qrcode on Component")
        comboBox_codeType.currentIndex = decodeSetting.codeType
        input_fileName.text = decodeSetting.fileName
        comboBox_saveFileType.currentIndex = separationSetting.fileType
        input_filePath.text = separationSetting.filePath
//        .replace("~" ,jkInterface.homeDictory())
    }
    Component.onDestruction: {
//        console.log("qrcode on Destruction")
        decodeSetting.codeType = comboBox_codeType.currentIndex
        decodeSetting.fileName = input_fileName.text
        separationSetting.fileType = comboBox_saveFileType.currentIndex
        separationSetting.filePath = input_filePath.text
    }
}
