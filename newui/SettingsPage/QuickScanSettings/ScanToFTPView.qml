import QtQuick 2.0
import "../../component"
FocusScope {
    width: 477
    height: 309
    KeyNavigation.tab: textInput_serverAddress

//    onActiveFocusChanged:{
//        if(activeFocus){
//            textInput_serverAddress.focus = true
//        }
//    }

    Column{
        anchors.fill: parent
        Item {
            id: item1
            width: parent.width
            height: 60

            JKText {
                id: text1
                text: qsTr("ResStr_DocScan_server_addr")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput_serverAddress
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 255
                KeyNavigation.tab: textInput_userName
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60

            JKText {
                id: text2
                text: qsTr("ResStr_DocScan_username")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput_userName
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 30
                KeyNavigation.tab: textInput_password
            }
        }
        Item {
            id: item3
            width: parent.width
            height: 60

            JKText {
                id: text3
                text: qsTr("ResStr_Password_")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput_password
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 32
                echoMode:TextInput.Password
                KeyNavigation.tab: textInput_targetPath
            }
        }
        Item {
            id: item4
            width: parent.width
            height: 60

            JKText {
                id: text4
                text: qsTr("ResStr_DocScan_targetPath")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKTextInput {
                id: textInput_targetPath
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                maximumLength: 255
                KeyNavigation.tab: textInput_serverAddress
            }
        }
    }


    property var setting
    Component.onCompleted:{
        init()
    }

    function init(){
        textInput_serverAddress.text = setting.serverAddress
        textInput_serverAddress.cursorPosition = 0
        textInput_userName.text = setting.userName
        textInput_userName.cursorPosition = 0
        textInput_password.text = setting.password
        textInput_password.cursorPosition = 0
        textInput_targetPath.text = setting.targetPath
        textInput_targetPath.cursorPosition = 0

        textInput_serverAddress.forceActiveFocus()
    }
    function ok(){
        var serverAddress = textInput_serverAddress.text
        var userName = textInput_userName.text
        var password = textInput_password.text
        var targetPath = textInput_targetPath.text
        if(serverAddress === ""){
            warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_DocScan_server_addr1")))
            textInput_serverAddress.forceActiveFocus()
//            textInput_serverAddress.focus = true
            return false
        }else if(userName === ""){
            warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_DocScan_username1")))
            textInput_userName.forceActiveFocus()
//            textInput_userName.focus = true
            return false
        }else if(password === ""){
            warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_DocScan_password1")))
            textInput_password.forceActiveFocus()
//            textInput_password.focus = true
            return false
        }else if(targetPath === ""){
            warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_DocScan_targetPath1")))
            textInput_targetPath.forceActiveFocus()
//            textInput_targetPath.focus = true
            return false
        }else if(!serverAddress.match(/ftp:\/\/[^\s]+$/i)){
            warningWithImage(qsTr("ResStr_specify_incorrect").arg(qsTr("ResStr_DocScan_server_addr1")))
            textInput_serverAddress.forceActiveFocus()
//            textInput_serverAddress.focus = true
            return false
        }else if(!targetPath.match(/\/[^\\\?\s\*:<>|\"]*$/)){
            warningWithImage(qsTr("ResStr_specify_incorrect").arg(qsTr("ResStr_DocScan_targetPath1")))
            textInput_targetPath.forceActiveFocus()
//            textInput_targetPath.focus = true
            return false
        }
        setting.serverAddress = serverAddress
        setting.userName = userName
        setting.password = password
        setting.targetPath = targetPath
        return true
    }
}
