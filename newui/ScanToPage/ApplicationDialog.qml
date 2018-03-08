import QtQuick 2.0
import "../component"
import QtQuick.Dialogs 1.2

JKParaDialog{
    id: root
    width: 550 + 20
    height: 450 + 20
    toolbar{
        text.text:qsTr("To Application")
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
                    appName:qsTr("Others")
                }
            }
            delegate:JKAbstractButton {
                id:wrapper
                height:76
                width: ListView.view.width
                activeFocusOnTab: ListView.isCurrentItem
                dashRectange.z: 2
                Rectangle{
                    anchors.fill: parent
                    anchors.margins: 1
                    z:1
                    opacity: wrapper.ListView.isCurrentItem || mouseArea.containsMouse ?0.5 :1
                    border.color: wrapper.ListView.isCurrentItem ? "green"
                                                         :"Transparent"
                    border.width: 2
                    color: mouseArea.containsMouse ? "LightSlateGray"
                                                   : "LightSteelBlue"
                    Image{
                        x:10
                        width: 50
                        height:50
                        source: imageSource
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    JKText{
                        x:100
                        text: appName
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                onFocusChanged:
                    activeFocusOnTab = focus
                onClicked: {
                    wrapper.ListView.view.currentIndex = index
                    if(index === 1){
                        fileDialog.open()
                    }
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
        nameFilters: ["APP (*.app)"]
//        selectFolder: true
        onAccepted: {
            var fullpath = fileUrl.toString().replace("file:///" ,"/")
            setting.fullFileName = fullpath
            acceptOK()
        }
    }

    function acceptOK(){
        if(listView.currentIndex === 0){
            setting.fullFileName = "/Applications/Preview.app"
        }
        setting.fileType = listView.currentIndex
        ok()
        root.close()
        root.accepted()
    }

    signal accepted
    JKTextButton{
        parent: item_btnOK
        anchors.fill: parent
        text.text: qsTr("OK")
        onClicked: {
            acceptOK()
        }
    }
}
