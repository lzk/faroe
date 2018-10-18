import QtQuick 2.0
import QtQuick.Controls 2.2
import "SettingsPage"
import "component"

SettingsLayout {
    id: root
    width: 750
    height: 533

//    onActiveFocusChanged: {
//        console.log("focus:" ,activeFocus)
//        if(activeFocus){
//            button_back.focus = true
//        }
//    }

    JKImageButton {
        id: button_back
        parent:root.item_back
        anchors.fill: parent
        source_disable: "qrc:/Images/back_disable.png"
        source_normal: "qrc:/Images/back_normal.png"
        source_press: "qrc:/Images/back_press.png"
//        KeyNavigation.tab: listView
//        focus: true
    }

    ListView {
        id: listView
        parent:root.item_tab
        anchors.fill: parent
        snapMode:ListView.SnapOneItem
        interactive :false
//        KeyNavigation.right: loader.item
//        KeyNavigation.tab: loader.item
//        Keys.onPressed: {
//            switch(event.key){
//            case Qt.Key_Up:
//                if(currentIndex > 0)
//                    currentIndex --
//                break
//            case Qt.Key_Down:
//                if(currentIndex < 6)
//                    currentIndex ++
//                break
//            }
//        }

        delegate: ItemDelegate {
            id:delegate
            width: ListView.view.width
            height: ListView.view.height / 7
            JKDashRectange{
                id:dashRectange
                anchors.fill: delegate
                anchors.margins: -1
                radius: 0
                visible: delegate.activeFocus
            }
            Image {
                anchors.fill: parent
                source: delegate.ListView.isCurrentItem ?imageSource:"qrc:/Images/setting_img_disable.png"
                JKText {
                    id:tt
                    text: name
                    x:5
                    width: delegate.width - 10
                    font.pixelSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegate.ListView.isCurrentItem ?"white":"black"
                    wrapMode: Text.WordWrap
                    elide: Text.ElideRight
                    maximumLineCount : 1
                }
                Label{
                    text:name
                    x:5
                    padding: 5

                    font.family: "Verdana"
                    font.pixelSize: 14
//                    color: delegate.ListView.isCurrentItem ?"white":"black"
                    anchors.verticalCenter: parent.verticalCenter
                    visible: delegate.hovered && tt.truncated //&& index === 1

                    background: Rectangle{
                        color: "white"
                        border.color: "gray"
                    }
                }
            }
            onClicked:{
                ListView.view.currentIndex = index
                delegate.forceActiveFocus()
            }
        }
        model: ListModel {
            ListElement {
                name: qsTr("ResStr_QuickScanSettings")
                imageSource:"qrc:/Images/setting_img_quick scan.png"
                url:"SettingsPage/QuickScanSettings.qml"
            }

            ListElement {
                name: qsTr("ResStr_DecodeSettings")
                imageSource:"qrc:/Images/setting_img_qrcode barcode.png"
                url:"SettingsPage/QRCodeBarcodeView.qml"
            }

            ListElement {
                name: qsTr("ResStr_ScanToSettings")
                imageSource:"qrc:/Images/setting_img_scan to.png"
                url:"SettingsPage/ScanParameterView.qml"
            }

            ListElement {
                name: qsTr("ResStr_Printer_Wi_Fi")
                imageSource:"qrc:/Images/setting_img_wifi.png"
                url:"SettingsPage/WifiView.qml"
            }
            ListElement {
                name: qsTr("ResStr_TCP_IPv4")
                imageSource:"qrc:/Images/setting_img_IPv4.png"
                url:"SettingsPage/TcpipView.qml"
            }
            ListElement {
                name: qsTr("ResStr_Soft_AP")
                imageSource:"qrc:/Images/setting_img_softap.png"
                url:"SettingsPage/SoftapView.qml"
            }
            ListElement {
                name: qsTr("ResStr_Device")
                imageSource:"qrc:/Images/setting_img_device setting.png"
                url:"SettingsPage/DeviceView.qml"
            }
        }
    }

    Loader{
        id:loader
        parent:root.item_view
        source: listView.model.get(listView.currentIndex).url
    }
//    Component.onCompleted: {
//        listView.forceActiveFocus()
//    }

    Component.onDestruction: {
        loader.source = ""
    }

    Connections{
        target: button_back
        onClicked:{
            root.StackView.view.pop()
        }
    }


}
