import QtQuick 2.0
import "../component"
import QtQuick.Dialogs 1.2
import "../ScanData.js" as ScanData
JKDialog{
    id: root
    width: 550 + 20
    height: 450 + 20

    property var setting: ScanData.defaultApplicationSettings()
    toolbar{
        text.text:qsTr("ResStr_DocScan_To_Application")
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
    }

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

        ListView{
            id:listView
            anchors.fill: parent
            anchors.leftMargin: 30
            anchors.rightMargin: 30
            anchors.bottomMargin: 60
            clip: true
            model: ListModel{
                ListElement{
                    imageSource:"qrc:/Images/Preview.png"
//                    imageSource:"file:/Applications/Preview/Contents/Resources/Preview.icns"
                    appName:"Preview"
                }
                ListElement{
                    imageSource:"qrc:/Images/others.png"
                    appName:qsTr("ResStr_DocScan_AP_Others")
                }
            }
            delegate: FilelistDelegate{
                source: imageSource
                text: appName
                onDoubleClicked: {
                    switch(index){
                    case 0:
                        acceptOK()
                        break
                    case 1:
                    default:
                        fileDialog.open()
                        break
                    }
                }
            }

//            delegate:JKAbstractButton {
//                id:wrapper
//                height:76
//                width: ListView.view.width
//                activeFocusOnTab: ListView.isCurrentItem
//                dashRectange.z: 2
//                Rectangle{
//                    anchors.fill: parent
//                    anchors.margins: 1
//                    z:1
//                    opacity: wrapper.ListView.isCurrentItem ?0.7 :mouseArea.containsMouse ?0.5 :1
//                    border.color: mouseArea.containsMouse || wrapper.ListView.isCurrentItem ?"#2996e0"
//                                                         :"Transparent"
//                    border.width: 2
//                    color: mouseArea.containsMouse || wrapper.ListView.isCurrentItem ?"lightblue"
//                                                   : "white"
//                    Image{
//                        x:10
//                        width: 50
//                        height:50
//                        source: imageSource
//                        anchors.verticalCenter: parent.verticalCenter
//                    }
//                    JKText{
//                        x:100
//                        text: appName
//                        anchors.verticalCenter: parent.verticalCenter
//                    }
//                }

//                onFocusChanged:
//                    activeFocusOnTab = focus
//                onClicked: {
//                    wrapper.ListView.view.currentIndex = index
//                }
//                onDoubleClicked: {
//                    wrapper.ListView.view.currentIndex = index
//                    switch(index){
//                    case 0:
//                        acceptOK()
//                        break
//                    case 1:
//                    default:
//                        fileDialog.open()
//                        break
//                    }
//                }
//            }
        }
        Rectangle{
            anchors.fill: listView
            border.color: "lightgray"
            color: "transparent"
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
        nameFilters: ["APP (*.app)"]
//        selectFolder: true
        onAccepted: {
            var fullpath = decodeURIComponent(fileUrl).replace("file:///" ,"/")
            setting.fullFileName = fullpath
            acceptOK()
        }
    }

    function acceptOK(index){
        if(listView.currentIndex === 0){
            setting.fullFileName = "/Applications/Preview.app"
        }
        setting.fileType = listView.currentIndex
        root.close()
        root.accepted()
    }

    signal accepted
    JKTextButton{
        parent: item_btnOK
        anchors.fill: parent
        text: qsTr("ResStr_OK")
        onClicked: {
            switch(listView.currentIndex){
            case 0:
                acceptOK()
                break
            case 1:
            default:
                fileDialog.open()
                break
            }
        }
    }
}
