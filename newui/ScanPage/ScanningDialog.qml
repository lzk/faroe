import QtQuick 2.0
import QtQuick.Window 2.2
import "../component"
import com.liteon.JKInterface 1.0

Window {
    flags: Qt.Dialog | Qt.FramelessWindowHint
    color: "transparent"
    property alias text: text1.text
    property alias canCancel: button_cancel.visible
    modality: Qt.ApplicationModal // Qt.WindowModal
    id:root
    width: 850
    height: 560
    signal cancel
    property var dialog

    Rectangle{
        anchors.fill: parent
        color: "#ccffffff"
    }

    Rectangle {
        id: rectangle
        width: 200
        height: 70
        color: "#ffe5e5e5"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Item {
            id: item1
            x: 0
            y: 40
            height: 30
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left

            Rectangle {
                id: rectangle1
                color: "transparent"
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.bottomMargin: 15
                anchors.fill: parent
                border.color: "#44444444"
                clip: true
                Rectangle {
                    id: rectangle2
                    width: 50
                    color: "green"
                    anchors.bottomMargin: 1
                    anchors.topMargin: 1
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top

                    NumberAnimation on x{
                        from: -rectangle2.width
                        to: rectangle1.width
                        duration: 1500
                        loops: Animation.Infinite
                    }
                }

            }
        }

        Item {
            id: item2
            anchors.bottom: item1.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            JKText {
                id: text1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 20
                text : qsTr("Scanning...")
            }
        }
    }

    JKAbstractButton{
        id:button_cancel
        width: 50
        height: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rectangle.bottom
        anchors.topMargin: 20
        dashRectange.radius: 26
        Image{
            width: 32
            height: 32
            anchors.centerIn: parent
            source: parent.enabled ?parent.mouseArea.pressed
                                     ?"qrc:/Images/cancelIcon.png"
                                     :"qrc:/Images/cancelPress.png"
            :"qrc:/Images/cancelDisable.png"
        }
        onClicked: cancel()

    }
    function open(){
        show()        
    }
    Connections{
        target: jkInterface
        onProgressChanged:{
            switch(progress){
            case DeviceStruct.ScanningProgress_Start:
                text1.text = qsTr("Scanning...")
                break
            case DeviceStruct.ScanningProgress_Upload:
                text1.text = qsTr("Uploading...")
                break
            case DeviceStruct.ScanningProgress_Completed:
                text1.text = qsTr("Page %1 finished.").arg(page + 1)
                break

            }
        }
    }
}
