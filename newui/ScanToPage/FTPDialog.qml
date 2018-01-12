import QtQuick 2.0
import "../component"
import "../ScanData.js" as JSData
import "../SettingsPage/QuickScanSettings"

JKParaDialog{
    id: root
    width: 550 + 20
    height: 450 + 20
    toolbar{
        text.text:qsTr("To FTP")
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

        ScanToFTPView{
            id:settingLoader
            setting:root.setting
            anchors.fill: parent
            anchors.topMargin: 30
            anchors.leftMargin: 70
            anchors.rightMargin: 70
        }

        Item{
            anchors.top: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: -80

            Item {
                id: item_btnOK
                width: 150
                height: 35
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

//    Component.onCompleted: settingLoader.source = "../SettingsPage/QuickScanSettings/ScanToFTPView.qml"

    signal accepted
    property var setting: JSData.defaultFTPSettings()
    JKTextButton{
        parent: item_btnOK
        anchors.fill: parent
        text.text: qsTr("OK")
        onClicked: {
//            if(settingLoader.item.ok()){
            if(settingLoader.ok()){
                root.accepted()
                root.close()
            }
        }
    }
}
