import QtQuick 2.0
import "../../component"
import "../../ScanData.js" as JSData
Item {
    id:root
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
                text: qsTr("Attachment File Type:")
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
                model: JSData.constEmailAttachmentFileType()
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60

            JKText {
                id: text2
                text: qsTr("Recipient:")
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
                input{
                    maximumLength:255
                    validator: RegExpValidator{
                        regExp: /^[a-zA-Z0-9_.@]*$/
                    }
                }

            }
        }
        Item {
            id: item3
            width: parent.width
            height: 60

            JKText {
                id: text3
                text: qsTr("Subject:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput3
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                input{
                    maximumLength:255
                }
            }
        }
    }

    property var setting
    Component.onCompleted:{
        init()
    }

    function init(){
        comboBox.currentIndex = setting.fileType
        textInput2.text = setting.recipient
        textInput3.text = setting.subject
    }
    function ok(){
        var regExp = /^[a-zA-Z0-9_.]+@[a-zA-Z0-9]+\.([a-zA-Z0-9]{1,}\.)*[a-zA-Z]{2,}$/
//        var regExp = /^[a-zA-Z0-9_.]+@[a-zA-Z0-9]+(([a-zA-Z0-9]*\.)*[a-zA-Z]{2,15})*$/
        if(textInput2.text === ""){
            warningWithImage(qsTr("The Recipient cannot be empty!"))
            textInput2.input.focus = true
            return false
        }else if(!textInput2.text.match(regExp)){
            warningWithImage(qsTr("The E-mail address format is incorrect,Please check your E-mail address and enter again."))
            textInput2.input.focus = true
            return false
        }

        setting.fileType = comboBox.currentIndex
        setting.recipient = textInput2.text
        setting.subject = textInput3.text
        return true
    }
}
