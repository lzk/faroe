import QtQuick 2.0
import "../component"
import QtQuick.Layouts 1.3

JKDialog{
    id: root
    width: 537 + 20
    height: 545 + 20
    property var setting

    Item {
        id: item2
        parent: container
        anchors.fill: parent
//        property alias item_view: item_view
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

        onVisibleChanged: {
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

        JKToolbar{
            id:toolbar
            parent: item_title
            text.text:setting !== undefined ?setting.sid :""
            text.font.pixelSize: 15
            text.color: "black"
            text.font.bold: true
            anchors.fill: parent
            onClose: root.close()
            onMovedXChanged: root.x += movedX
            onMovedYChanged: root.y += movedY
        }

        JKTextButton{
            parent: item_btnSettings
            anchors.fill: parent
            text.text: qsTr("Settings...")
        }

        JKTextButton{
            parent: item_btnOK
            anchors.fill: parent
            text.text: qsTr("OK")
            onClicked: {
                settingLoader.item.ok()
                root.close()
            }
        }        
    }
}
