import QtQuick 2.0
import "../component"
import QtQuick.Layouts 1.3

JKDialog{
    id: root
    width: 537 + 20
    height: 545 + 20
    property var setting
    property int mode: 0
    signal ok

    Item {
        parent: container
        anchors.fill: parent

        Image{
            anchors.fill: parent
            source: "qrc:/Images/popup_gnd_quick scan.png"
        }

        Item {
            id: item_title
            anchors.bottom: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: -30
        }

        Item {
            id: item1
            anchors.fill: parent
            anchors.margins: 30

            ColumnLayout {
                id: columnLayout
                spacing: 0
                anchors.fill: parent

                Item {
                    id: item_view
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Item {
                    id: item6
                    height: 60
                    Layout.fillWidth: true

                    Item {
                        id: item_btnOK
                        width: 150
                        height: 35
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }

    JKToolbar{
        id:toolbar
        parent: item_title
        text.text:mode === 0 && setting !== undefined?setting.sid :qsTr("Add Quick Scan Setting")
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
        anchors.fill: parent
        onClose: root.close()
        onMovedXChanged: root.x += movedX
        onMovedYChanged: root.y += movedY
    }

    JKTextButton{
        parent: item_btnOK
        anchors.fill: parent
        text.text: qsTr("OK")
        onClicked: {
            settingLoader.item.ok()
            root.close()
            root.ok()
        }
    }

    Loader{
        id:settingLoader
        parent: item_view
        anchors.fill: parent
//        source:{
//            switch(mode){
//            case 0:   "QuickScanSettingView.qml";  break
//            case 1:   "NewQuickScanView.qml";  break
//            default: "";break
//            }
//        }
    }

    onVisibleChanged: {
//    Component.onCompleted: {
        if(!visible)
            return
        var source = ""
        switch(mode){
        case 0:   source = "QuickScanSettingView.qml";  break
        case 1:   source = "NewQuickScanView.qml";  break
        default:
            break
        }
        settingLoader.setSource(source ,{"setting":setting})
    }
}
