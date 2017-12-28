import QtQuick 2.0
import "../component"
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
                text: qsTr("Cloud Type:")
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin: 30
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKComboBox {
                id: comboBox
                width: 250
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                model: ["DropBox" ,"EverNote" ,"OneDrive"]
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60

            JKText {
                id: text2
                text: qsTr("Reset access token in cacle:")
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin: 30
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextButton {
                id: button_reset
                text.text: qsTr("Reset")
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Item{
            id: item3
            width: parent.width
            height: 120
            visible: comboBox.currentIndex === 1
            Column{
                anchors.fill: parent
                Item {
                    id: item31
                    width: parent.width
                    height: 60

                    JKText {
                        id: text31
                        text: qsTr("Note Title:")
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 30
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    JKTextInput {
                        id: textInput31
                        width: 250
                        height: 30
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Item {
                    id: item32
                    width: parent.width
                    height: 60

                    JKText {
                        id: text32
                        text: qsTr("Note Content:")
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 30
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    JKTextInput {
                        id: textInput32
                        width: 250
                        height: 30
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

        }
        Item{
            id: item4
            width: parent.width
            height: 120
            visible: !item3.visible
            Column{
                anchors.fill: parent
                Item {
                    id: item41
                    width: parent.width
                    height: 60

                    JKText {
                        id: text41
                        text: qsTr("Default Save Path:")
                        font.bold: true
                        anchors.left: parent.left
                        anchors.leftMargin: 30
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                    }

                    JKTextInput {
                        id: textInput411
                        width: 250
                        height: 30
                        visible: comboBox.currentIndex === 0
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    JKTextInput {
                        id: textInput412
                        width: 250
                        height: 30
                        visible: !textInput411.visible
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                Item {
                    id: item42
                    width: parent.width
                    height: 60


                    JKTextButton {
                        id: button_browse
                        text.text: qsTr("Browser...")
                        width: 100
                        height: 30
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }

    }


    property var setting
    Component.onCompleted:{
        init()
    }

    function init(){
        if(visible){
            comboBox.currentIndex = setting.cloudType
            textInput31.text = setting.noteTitle
            textInput32.text = setting.noteContent
            textInput411.text = setting.dropboxFilePath
            textInput412.text = setting.oneDriveFilePath
        }
    }
    function ok(){
        setting.cloudType = comboBox.currentIndex
        setting.noteTitle = textInput31.text
        setting.noteContent = textInput32.text
        setting.dropboxFilePath = textInput411.text
        setting.oneDriveFilePath = textInput412.text
    }
}
