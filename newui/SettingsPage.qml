import QtQuick 2.0
import QtQuick.Controls 2.2
import "SettingsPage"
import "component"

SettingsLayout {
    id: root
    width: 750
    height: 533

    JKImageButton {
        id: button_back
        parent:root.item_back
        anchors.fill: parent
        source_disable: "qrc:/Images/back_disable.png"
        source_normal: "qrc:/Images/back_normal.png"
        source_press: "qrc:/Images/back_press.png"
    }

    ListView {
        id: listView
        parent:root.item_tab
        anchors.fill: parent
        snapMode:ListView.SnapOneItem
        interactive :false
        delegate: JKAbstractButton {
            id:delegate
            width: ListView.view.width
            height: ListView.view.height / 7
//            enabled: index !== 6
            Image {
                anchors.fill: parent
                source: delegate.ListView.isCurrentItem ?imageSource:"qrc:/Images/setting_img_disable.png"
                JKText {
                    text: name
                    x:5
                    font.pixelSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegate.ListView.isCurrentItem ?"white":"black"
                }
            }
            onClicked:ListView.view.currentIndex = index
        }
        model: ListModel {
            ListElement {
                name: qsTr("Quick Scan Settings")
                imageSource:"qrc:/Images/setting_img_quick scan.png"
                url:"SettingsPage/QuickScanSettings.qml"
            }

            ListElement {
                name: qsTr("Decode/Separation...")
                imageSource:"qrc:/Images/setting_img_qrcode barcode.png"
                url:"SettingsPage/QRCodeBarcodeView.qml"
            }

            ListElement {
                name: qsTr("Scan To Setting")
                imageSource:"qrc:/Images/setting_img_scan to.png"
                url:"SettingsPage/ScanParameterView.qml"
            }

            ListElement {
                name: qsTr("Wi-Fi")
                imageSource:"qrc:/Images/setting_img_wifi.png"
                url:"SettingsPage/WifiView.qml"
            }
            ListElement {
                name: qsTr("Tcp/IPv4")
                imageSource:"qrc:/Images/setting_img_IPv4.png"
                url:"SettingsPage/TcpipView.qml"
            }
            ListElement {
                name: qsTr("Soft AP")
                imageSource:"qrc:/Images/setting_img_softap.png"
                url:"SettingsPage/SoftapView.qml"
            }
            ListElement {
                name: qsTr("Device Setting")
                imageSource:"qrc:/Images/setting_img_device setting.png"
                url:"SettingsPage/DeviceView.qml"
            }
        }
    }

    onFocusChanged: {
        console.log("root focus:" ,focus)
    }

    Loader{
        id:loader
        parent:root.item_view
        source: listView.model.get(listView.currentIndex).url
        focus: true
        onFocusChanged: {
            console.log("loader focus:" ,focus)
        }
    }
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
