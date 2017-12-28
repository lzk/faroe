import QtQuick 2.0
import "../component"
Item {
    id: root
    width: 495
    height: 460

    Column {
        id: column
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        anchors.fill: parent

        Item {
            id: item_text
            width: 300
            height: 50

            Text {
                id: text1
                text: qsTr("Quick Scan List:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }
        }

        Row {
            id: row
            height: 315
            anchors.right: parent.right
            anchors.left: parent.left

            Rectangle {
                id: item_listView
                width: 300
                height: 315
                border.color: "gray"
            }

            Column {
                id: column1
                width: 100
                height: 150
                anchors.top: parent.top
                anchors.topMargin: 75

                Item {
                    id: item2
                    width: 75
                    height: 75
                    anchors.left: parent.left
                    anchors.leftMargin: 25

                    Item {
                        id: item_btnMoveUp
                        width: 70
                        height: 70
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Item {
                    id: item3
                    width: 75
                    height: 75
                    anchors.leftMargin: 25
                    anchors.left: parent.left

                    Item {
                        id: item_btnMoveDown
                        width: 70
                        height: 70
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }

        Item {
            id: item4
            width: 300
            height: 75

            Row {
                id: row2
                spacing: 5
                anchors.topMargin: 5
                anchors.fill: parent

                Item {
                    id: item_btnAdd
                    width: 70
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }

                Item {
                    id: item_btnEdit
                    width: 70
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }

                Item {
                    id: item_btnDelete
                    width: 70
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }

                Item {
                    id: item_btnSettings
                    width: 70
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    JKImageButton{
        id:button_moveUp
        parent: item_btnMoveUp
        anchors.fill: parent
        source_normal: "qrc:/Images/quick scan_btn_move up.png"
        source_press: "qrc:/Images/quick scan_btnf_move up.png"
        source_disable: "qrc:/Images/move up_disable.png"
    }

    JKImageButton{
        id:button_moveDown
        parent: item_btnMoveDown
        anchors.fill: parent
        source_normal: "qrc:/Images/quick scan_btn_move down.png"
        source_press: "qrc:/Images/quick scan_btnf_move down.png"
        source_disable: "qrc:/Images/move down_disable.png"
    }

    JKImageButton{
        id:button_add
        parent: item_btnAdd
        anchors.fill: parent
        source_normal: "qrc:/Images/quick scan_btn_new.png"
        source_press: "qrc:/Images/quick scan_btnf_new.png"
        source_disable: "qrc:/Images/quick scan_btn_new_disable.png"
        onClicked: openDialog(1 ,undefined)
    }
    JKImageButton{
        id:button_edit
        parent: item_btnEdit
        anchors.fill: parent
        source_normal: "qrc:/Images/quick scan_btn_edit.png"
        source_press: "qrc:/Images/quick scan_btnf_edit.png"
        source_disable: "qrc:/Images/edit_disable.png"
        enabled: listview.currentIndex >= 0 ?root.settings[listview.currentIndex].canEdit :false
    }
    JKImageButton{
        id:button_delete
        parent: item_btnDelete
        anchors.fill: parent
        source_normal: "qrc:/Images/quick scan_btn_delete.png"
        source_press: "qrc:/Images/quick scan_btnf_delete.png"
        source_disable: "qrc:/Images/delete_disable.png"
        enabled: listview.currentIndex >= 0 ?root.settings[listview.currentIndex].canEdit :false
    }
    JKImageButton{
        id:button_settings
        parent: item_btnSettings
        anchors.fill: parent
        source_normal: "qrc:/Images/quick scan_btn_setting.png"
        source_press: "qrc:/Images/quick scan_btnf_setting.png"
        source_disable: "qrc:/Images/quick scan_btn_setting.png"
        onClicked: openDialog(0 ,root.settings[listview.currentIndex])
    }

    ListView{
        id:listview
        parent: item_listView
        anchors.fill: parent
        anchors.margins: 1
        snapMode:ListView.SnapOneItem
        interactive :false
        spacing:2
        focus: true
        delegate: JKAbstractButton {
            id:delegate
            width: ListView.view.width
            height: 18
            Rectangle {
                anchors.fill: parent
                color: delegate.ListView.isCurrentItem ?"LightBlue":delegate.mouseArea.containsMouse ?"#FFCCEBF5":"white"
                JKText {
                    text: (index+1)+". " + model.modelData.name
                    x:5
                    font.pixelSize: 11
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            onClicked:ListView.view.currentIndex = index
        }
        model:settings
    }
    QuickScanDialog{
        id:dialog
        onOk: listview.model = settings
//        setting: listview.currentIndex > 0 ?settings[listview.currentIndex] :undefined
    }
//    Loader{
//        id:loader
//    }

    property var settings:scanData.settings

    Component.onCompleted: {
//        for(var i=0 ;i < settings.length ;i++){
//            model.append({"name":settings[i].name})
//        }
    }

    function openDialog(mode ,setting){
//        loader.source = "ScanToPrintDialog.qml"
//        var dialog = loader.item
        dialog.setting = setting
        dialog.mode = mode
        dialog.open()
    }
}
