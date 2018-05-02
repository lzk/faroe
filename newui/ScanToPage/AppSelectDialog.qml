import QtQuick 2.0
import "../component"
import QtQuick.Dialogs 1.2

JKParaDialog{
    id: root
    width: 500 + 20
    height: 250 + 20
//    toolbar{
//        text.text:qsTr("To FTP")
//        text.font.pixelSize: 15
//        text.color: "black"
//        text.font.bold: true
//    }

    Rectangle{
       parent: background
       anchors.fill: parent
       border.width: 4
       border.color: "#FF0E5686"
       color: "white"
    }

    Item{
        parent:container
        anchors.fill: parent

        Column{
            anchors.fill: parent
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            Item {
                id: item1
                width: parent.width
                height: 46

                JKText {
                    id: text1
                    text: qsTr("AP Name:")
                    font.bold: true
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_fileName
                    width: 250
                    height: 30
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Item {
                id: item2
                width: parent.width
                height: 46

                JKText {
                    id: text2
                    text: qsTr("AP Path:")
                    font.bold: true
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }

                JKTextInput {
                    id: textInput_path
                    width: 250
                    height: 30
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                }
            }
            Item {
                id: item3
                width: parent.width
                height: 60

                JKTextButton {
                    id: button_browse
                    text: qsTr("Browser...")
                    width: 100
                    height: 30
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: fileDialog.open()
                }
            }
        }

        Item{
            anchors.top: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: -60

            Item {
                id: item_btnOK
                width: 150
                height: 35
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Applications")
        folder: "file:///Applications"
//        nameFilters: JSData.constFileDialogSaveFileType()
//        selectFolder: true
        onAccepted: {
            var fullpath = decodeURIComponent(fileUrl).replace("file:///" ,"/")
            textInput_path.text = decodeURIComponent(fileUrl).replace("file:///" ,"/")
            textInput_path.cursorPosition = 0
        }
    }

//    Component.onCompleted: init()

//    function init(){
    function update(){
        textInput_fileName.text = setting.fileName
        textInput_fileName.cursorPosition = 0
        textInput_path.text = setting.filePath
        textInput_path.cursorPosition = 0
    }
    function ok(){
        setting.fileName = textInput_fileName.text
        setting.filePath = textInput_path.text
    }

    signal accepted
    JKTextButton{
        parent: item_btnOK
        anchors.fill: parent
        text: qsTr("OK")
        onClicked: {
            ok()
            root.accepted()
            root.close()
        }
    }
}
