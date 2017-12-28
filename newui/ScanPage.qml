import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import "component"
import "ScanPage"

ScanPageLayout {
    id: root
    width: 750
    height: 533

    Row{
        parent: item_search
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.left: parent.left
        spacing: 5
        JKImageButton{
            id:button_search
            source_normal: "qrc:/Images/device_normal.png"
            source_press: "qrc:/Images/device_press.png"
            width: 38
            height: 38
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.push("SearchDevice.qml")
        }
        Text {
            text: qsTr("Disconnected")
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    QuickScanButton{
        id:button_quickScan
        parent: item_quickscan
        anchors.fill: parent
        mode: qsTr("Color")
        dpi: qsTr("300dpi")
        adf: qsTr("Two Side")
        name: "1.Scan to Print"
        source: "qrc:/Images/main_img_print.png"
    }

    JKImageButton{
        id:button_left
        parent: item_leftButton
        anchors.fill: parent
        source_disable: "qrc:/Images/left_disable.png"
        source_normal: "qrc:/Images/left_normal.png"
        source_press: "qrc:/Images/left_press.png"
    }
    JKImageButton{
        id:button_right
        parent: item_rightButton
        anchors.fill: parent
        source_disable: "qrc:/Images/right_disable.png"
        source_normal: "qrc:/Images/right_normal.png"
        source_press: "qrc:/Images/right_press.png"
    }
    JKFunctionButton{
        id:button_decode
        parent: item_decode
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width:110
        height: 150
        source_disable: "qrc:/Images/Main_btn_decode.png"
        source_normal: "qrc:/Images/Main_btn_decode.png"
        source_press: "qrc:/Images/Main_btn_decode.png"
        jktext.text:qsTr("Decode")
    }
    JKFunctionButton{
        id:button_separation
        parent: item_separation
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width:110
        height: 150
        source_disable: "qrc:/Images/main_btn_separation.png"
        source_normal: "qrc:/Images/main_btn_separation.png"
        source_press: "qrc:/Images/main_btn_separation.png"
        jktext.text:qsTr("Separation")
    }
    JKFunctionButton{
        id:button_scanto
        parent: item_scanto
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width:110
        height: 150
        source_disable: "qrc:/Images/main_btn_scanto.png"
        source_normal: "qrc:/Images/main_btn_scanto.png"
        source_press: "qrc:/Images/main_btn_scanto.png"
        jktext.text:qsTr("Scan To")
    }
    JKFunctionButton{
        id:button_settings
        parent: item_settings
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width:110
        height: 150
        source_disable: "qrc:/Images/main_btn_settings.png"
        source_normal: "qrc:/Images/main_btn_settings.png"
        source_press: "qrc:/Images/main_btn_settings.png"
        jktext.text:qsTr("Settings")
    }

    ScanningDialog{
        id:scanningDialog
        onCancel: close()
    }

    Connections{
        target: button_scanto
        onClicked: {
//            scanningDialog.show()
            root.StackView.view.push("ScanToPage.qml")
            }
    }

    Connections{
        target: button_settings
        onClicked: root.StackView.view.push("SettingsPage.qml")
    }
}
