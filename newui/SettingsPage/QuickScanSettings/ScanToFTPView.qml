import QtQuick 2.0
import "../../component"
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
                text: qsTr("Server Address:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput1
                focus:true
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 255
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60

            JKText {
                id: text2
                text: qsTr("User Name:")
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
                maximumLength: 30
            }
        }
        Item {
            id: item3
            width: parent.width
            height: 60

            JKText {
                id: text3
                text: qsTr("Password:")
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
                maximumLength: 32
                echoMode:TextInput.Password
            }
        }
        Item {
            id: item4
            width: parent.width
            height: 60

            JKText {
                id: text4
                text: qsTr("Target Path:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput4
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 255
            }
        }
    }


    property var setting
    Component.onCompleted:{
        init()
    }

    function init(){
        textInput1.text = setting.serverAddress
        textInput1.cursorPosition = 0
        textInput2.text = setting.userName
        textInput2.cursorPosition = 0
        textInput3.text = setting.password
        textInput3.cursorPosition = 0
        textInput4.text = setting.targetPath
        textInput4.cursorPosition = 0
    }
    function ok(){
        var serverAddress = textInput1.text
        var userName = textInput2.text
        var password = textInput3.text
        var targetPath = textInput4.text
        if(serverAddress === ""){
            warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("Server Address")))
            textInput1.forceActiveFocus()
//            textInput1.focus = true
            return false
        }else if(userName === ""){
            warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("User Name")))
            textInput2.forceActiveFocus()
//            textInput2.focus = true
            return false
        }else if(password === ""){
            warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("Password")))
            textInput3.forceActiveFocus()
//            textInput3.focus = true
            return false
        }else if(targetPath === ""){
            warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("Target Path")))
            textInput4.forceActiveFocus()
//            textInput4.focus = true
            return false
        }else if(!serverAddress.match(/ftp:\/\/[^\s]+$/i)){
            warningWithImage(qsTr("The %1 format is incorrect, Please check your %1 and enter again.").arg(qsTr("Server Address")))
            textInput1.forceActiveFocus()
//            textInput1.focus = true
            return false
        }else if(!targetPath.match(/\/[^\\\?\s\*:<>|\"]*$/)){
            warningWithImage(qsTr("The %1 format is incorrect, Please check your %1 and enter again.").arg(qsTr("Target Path")))
            textInput4.forceActiveFocus()
//            textInput4.focus = true
            return false
        }
        setting.serverAddress = serverAddress
        setting.userName = userName
        setting.password = password
        setting.targetPath = targetPath
        return true
    }
}
