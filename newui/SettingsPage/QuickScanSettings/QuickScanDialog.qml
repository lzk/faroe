import QtQuick 2.0
import "../../component"
import QtQuick.Layouts 1.3

JKParaDialog{
    id: root
    width: 537 + 20
    height: 545 + 20
    property int mode: 0
    signal accepted

    toolbar{
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
        text.text:
        switch(mode){
        case 0:
            return setting !== undefined && setting !== null ?setting.sid :"";
        case 1:
            return qsTr("Add Quick Scan Setting");
        case 2:
            return qsTr("Edit Quick Scan Setting");
        }
    }
    Image{
        parent: background
        anchors.fill: parent
        source: "qrc:/Images/popup_gnd_quick scan.png"
    }

    Item {
        parent: container
        anchors.fill: parent

        Item {
            id: item1
            anchors.fill: parent
            anchors.rightMargin: 30
            anchors.leftMargin: 30
            anchors.bottomMargin: 30

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

    JKTextButton{
        parent: item_btnOK
        anchors.fill: parent
        text.text: qsTr("OK")
        onClicked: {
            if(settingLoader.item.ok()){
                if(mode === 1){
                    scanData.addQuickScanSetting(setting)
                }else{
                    root.ok()
                }
                root.accepted()
                root.close()
            }
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
        case 2:   source = "NewQuickScanView.qml";  break
        default:
            break
        }
        settingLoader.source = source
//        settingLoader.setSource(source ,{"setting":setting})
    }

    function initWithPara(setting ,mode){
        root.mode = mode
        if(mode === 1 && setting === undefined)
            ;
        else
            initWithSetting(setting)
    }

}
