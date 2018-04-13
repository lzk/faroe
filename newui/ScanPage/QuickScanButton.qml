import QtQuick 2.0
import "../component"

JKAbstractButton {
    id:root
    width: 460
    height: 282
    property alias mode: mode.text
    property alias dpi: dpi.text
    property alias adf: adf.text
    property alias name: name.text
    property alias source: image.source
    dashRectange.anchors.margins: 2
    dashRectange.z:1

    Image{
        id:background
        anchors.fill: parent
        source: mouseArea.containsPress ? "qrc:/Images/screen_press.png"
                                        :mouseArea.containsMouse ? "qrc:/Images/screen_over.png"
                                                                 : "qrc:/Images/screen_normal.png"
    }

    Column{
        id: column
        width: 400
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10

        Image {
            id: image
            width: 150
            height: 150
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/Images/main_img_print.png"
        }
        Text{
            id: name
            text:"1.Scan to Print"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 20
            font.bold: true

        }

        Grid {
            id: grid
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            columns: 2

            Text {
                id: adfTitle
                text: qsTr("ADF Mode:")
                font.pixelSize: 12
                width: 90
            }

            Text {
                id: adf
                text: qsTr("Two Side")
                font.pixelSize: 12
            }

            Text {
                id: dpiTitle
                text: qsTr("Resolution:")
                font.pixelSize: 12
            }

            Text {
                id: dpi
                text: qsTr("300dpi")
                font.pixelSize: 12
            }

            Text {
                id: modeTitle
                text: qsTr("Color Mode:")
                font.pixelSize: 12
            }

            Text {
                id: mode
                text: qsTr("Color")
                font.pixelSize: 12
            }
        }
    }

}
