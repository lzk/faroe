import QtQuick 2.0
import QtQuick.Layouts 1.3
import "../component"

Item {
    width: 477
    height: 425
//    property var setting
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            id: item3
            height: 60
            Layout.fillWidth: true

            Text {
                id: text1
                text: qsTr("Scan Setting:")
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
            }

            Item {
                id: item_btnSettings
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: item4
            height: 6
            Layout.fillWidth: true
            Canvas{
                anchors.fill: parent
                antialiasing: true
                property color strokeColor: "lightgray"
                property real lineWidth: 2

                onPaint: {
                    var ctx = getContext('2d')
                    ctx.save()
                    ctx.clearRect(0 ,0 ,width ,height)

                    ctx.strokeStyle = strokeColor
                    ctx.lineWidth = lineWidth
                    ctx.beginPath()
                    ctx.moveTo(0 ,4)
                    ctx.lineTo(width ,2)
                    ctx.stroke()
                    ctx.closePath()
                    ctx.restore()
                }

            }
        }

        Item {
            id: item_view
            Layout.fillHeight: true
            Layout.fillWidth: true
            Item{
                anchors.fill: parent
                anchors.bottomMargin: parent.height - 50
                JKText{
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 14
                    font.bold: true
                    text: setting !== undefined ?setting.title :""
                }
            }
            Loader{
                id:settingLoader
                anchors.fill: parent
                anchors.topMargin: 50
            }
        }
    }

    JKTextButton{
        parent: item_btnSettings
        anchors.fill: parent
        text.text: qsTr("Settings...")
    }

    onVisibleChanged: {
//    Component.onCompleted: {
        if(!visible)
            return
        var source = ""
        switch(setting.sid){
        case "Scan To Print":   source = "ScanToPrintView.qml";  break
        case "Scan To File":   source = "ScanToFileView.qml";  break
        case "Scan To Application":   source = "ScanToApplicationView.qml";  break
        case "Scan To Email":   source = "ScanToEmailView.qml";  break
        case "Scan To FTP":   source = "ScanToFTPView.qml";  break
        case "Scan To Cloud":   source = "ScanToCloudView.qml";  break
        default:
            break
        }
        settingLoader.setSource(source ,{"setting":setting})
    }

    function ok(){
        settingLoader.item.ok()
    }
}
