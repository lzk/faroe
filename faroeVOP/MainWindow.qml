import QtQuick 2.4
import QtQuick.Controls 2.2
import "../component"
import com.liteon.JKInterface 1.0
Item {
    id: root

    property color textcolor: "#75d7f9"
    property alias gridView: gridView
    property alias button_searchDevice: button_searchDevice

    FontLoader {
        id: digital7
        source: "fonts/digital-7.ttf"
    }
    JKImageButton {
        id: button_quickscan
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        pressedSource: "Images/screen_press.png"
        normalSource: "Images/screen_normal.png"
        hoverSource: "Images/screen_over.png"
    }

    Text {
        id: text_quickScan_index
        color: textcolor
        text: qsTr("1")
        font.family: digital7.name
        anchors.centerIn: button_quickscan
        font.pixelSize: 80
    }

    Text {
        id: text_quickScan_name
        color: textcolor
        text: qsTr("Scan To Printer")
        font.family: digital7.name
        verticalAlignment: Text.AlignVCenter
        anchors.bottom: button_quickscan.bottom
        anchors.top: text_quickScan_index.bottom
        anchors.horizontalCenter: button_quickscan.horizontalCenter
        font.pixelSize: 20
    }

    JKImageButton {
        id: button_minus
        anchors.verticalCenter: button_quickscan.verticalCenter
        anchors.right: button_quickscan.left
        anchors.rightMargin: 30
        pressedSource: "Images/left_press.png"
        normalSource: "Images/left_normal.png"
        hoverSource: "Images/left_over.png"
    }

    JKImageButton {
        id: button_plus
        anchors.verticalCenter: button_quickscan.verticalCenter
        anchors.left: button_quickscan.right
        anchors.leftMargin: 30
        pressedSource: "Images/right_press.png"
        normalSource: "Images/right_normal.png"
        hoverSource: "Images/right_over.png"
    }

    JKImageButton {
        id: button_searchDevice
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 100
        pressedSource: "Images/device_press_blue.png"
        normalSource: "Images/device_normal_blue.png"
        hoverSource: "Images/device_over_blue.png"
    }

    Image {
        id: backgroud
        anchors.rightMargin: 100
        anchors.leftMargin: 100
        anchors.topMargin: 50
        anchors.top: button_quickscan.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        source: "Images/panel_group.png"
    }

    ButtonView{
        id:gridView
        anchors.fill: backgroud
    }

    Popup{
        id:progressDialog
        modal:true
        focus:true
        closePolicy:Popup.NoAutoClose
//        property real value: 0.0

        x:Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)//Math.round((window.height - height) / 2)
        width:Math.round(window.width / 3 * 2)
        height: Math.round(window.height / 3 )

        contentItem: ProgressDialog{
            progress: jkInterface.progress
            buttonCancel.onClicked: jkInterface.cancelScan()
//            onVisibleChanged: visible
//            onProgressChanged:
//                if(progress > 0.99){
//                    progressDialog.close()
//                    stackView.push("ImageViewer.qml")
//                }
        }

//        Timer{
//            id:timer
//            interval: 100
//            running: progressDialog.visible
//            repeat: true
//            onTriggered: {
//                progressDialog.value += 0.01
//                if(progressDialog.value >= 1.0)
//                    progressDialog.close()
//            }
//        }
    }

    JKDialog{
        id:scanSettings
        toolbar.text: qsTr("Scan Settings")
        width:Math.round(window.width / 3 * 2)
        height: Math.round(window.height - 40 )
        ScanSettings{
            parent: scanSettings.container
            onOk: scanSettings.close()
        }
    }

    function gridViewClicked(index){
        switch(index){
        case 0://qrcode
        case 1://barcode
        case 2://scanto
            progressDialog.open()
            jkInterface.model.removeAll()
            jkInterface.scan()
            break;
        case 3://settings
//            stackView.push("ImageViewer.qml")
            scanSettings.open()
            scanSettings.moveToCenter()
            break
        }
    }

    function scanResult(result){
        console.log("scan result:" ,result)
        progressDialog.close()
        switch(result){
        case 0:
            stackView.push("ImageViewer.qml")
            break;
        default://error
            break;
        }
    }

    Connections {
        target: gridView
        onClicked:gridViewClicked(index)
    }

    Connections {
        target: jkInterface
        onScanResult:scanResult(result)
    }

}
