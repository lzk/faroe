import QtQuick 2.0
import QtQuick.Controls 2.2
import "../component"
import "../ScanData.js" as JSData
import com.liteon.JKInterface 1.0

JKDialog {
    id:root
    width: 650 + 20
    height: 530 + 20
    property string title:qsTr("File Viewer")
    signal accepted(var para)
    property var setting: {
        var tmp = JSData.defaultCloudSettings()
        tmp.okButtonText = qsTr("OK")
        tmp.fileList = [{
                            "imageUrl":"qrc:/Images/Folder-icon.png"
                            ,"fileName": "folder"
                            ,"fileSize":0
                        },{
                            "imageUrl":"qrc:/Images/file.png"
                            ,"fileName": "file"
                            ,"fileSize":0
                        },]
    }
    property var model:setting.fileList

    toolbar{
        text.text:title
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
    }

    Rectangle{
       parent: background
       anchors.fill: parent
       color: "white"
    }

    Item{
        parent:container
        anchors.fill: parent
        Column{
            anchors.fill: parent
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 60

            Item{
                width: parent.width
                height: 70
                Row{
                    anchors.fill: parent
                    spacing: 10
                    Item{
                        width: 50
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        JKImageButton{
                            id:button_cdup
                            anchors.fill: parent
                            source_disable: "qrc:/Images/folder_with_arrow_up_disable.png"
                            source_normal: "qrc:/Images/folder_with_arrow_up.png"
                            source_press: "qrc:/Images/folder_with_arrow_up.png"
                        }
                    }

                    Item{
                        width: 50
                        height: 50
                        anchors.verticalCenter: parent.verticalCenter
                        JKImageButton{
                            id:button_mkdir
                            anchors.fill: parent
                            source_disable: "qrc:/Images/folder_add.png"
                            source_normal: "qrc:/Images/folder_add.png"
                            source_press: "qrc:/Images/folder_add.png"
                        }
                    }
                    Item{
                        width: 10
                        height: parent.height
                    }
                    JKText{
                        id:text_currentDir
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("")
                    }
                }
            }
            Item{
                width: parent.width
                height: 250
                ScrollView{
                    anchors.fill: parent
                    clip: true
                    ListView{
                        anchors.fill: parent
                        model: root.model
                        delegate: FilelistDelegate{
                            source: model.modelData.imageUrl
                            text: model.modelData.fileName
                            size: model.modelData.fileSize
                            onDoubleClicked: {

                            }
                        }
                    }
                }
                Rectangle{
                    anchors.fill: parent
                    border.color: "lightgray"
                    color: "transparent"
                }
            }
            Item{
                width: parent.width
                height: 50
            }
    //        Item{
    //            width: parent.width
    //        }
        }
        Item{
            width: parent.width
            height: 60
            anchors.bottom: parent.bottom
            JKTextButton{
                id:button_OK
                width: 100
                height: 35
                anchors.centerIn: parent
                text:setting.okButtonText
            }
        }
    }

    Connections{
        target: button_OK
        onClicked:{
//            root.close()
            root.accepted(setting)
        }
    }

    Component.onCompleted: {
        init()
    }

    function init(){
        var supportCloudType = JSData.supportCloudType()
        switch(setting.cloudTypeText){
        case supportCloudType.icloud:
            title = qsTr("iCloud File Viewer")
            button_cdup.visible = false
            button_mkdir.visible = false
            break
        }
    }

    Connections{
        target: jkInterface
        onCmdExtraResult:{

            var setting = JSON.parse(para)
            switch(cmd){
            case DeviceStruct.CMD_Cloud_upload:
                if(setting.success){
                    model = setting.fileList
                }
                break
            }
        }
    }
}
