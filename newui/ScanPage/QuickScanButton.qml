import QtQuick 2.0
import "../component"
import "../ScanData.js" as JSData

JKAbstractButton {
    id:root
    width: 460
    height: 282
//    property alias mode: mode.text
//    property alias dpi: dpi.text
//    property alias adf: adf.text
//    property alias name: name.text
//    property alias source: image.source
    dashRectange.anchors.margins: 2
    dashRectange.z:1

    property int index: 0
    property int adfMode: 0
    property int colorMode: 0
    property int dpi: 0
    property var sid: constScanIds.scanToPrint
    property var constScanIds: JSData.constScanIds()
    function getSidText(sid){
        switch(sid){
        case constScanIds.scanToPrint:          return qsTr("ResStr_DocScan_Scan_Print")
        case constScanIds.scanToFile:           return qsTr("ResStr_DocScan_Scan_File")
        case constScanIds.scanToApplication:    return qsTr("ResStr_DocScan_Scan_App")
        case constScanIds.scanToEmail:          return qsTr("ResStr_DocScan_Scan_Email")
        case constScanIds.scanToFTP:            return qsTr("ResStr_DocScan_Scan_FTP")
        case constScanIds.scanToCloud:          return qsTr("ResStr_DocScan_Scan_Cloud")
        }
    }
    function getQuickScanSource(sid){
        switch(sid){
        case constScanIds.scanToPrint:          return "qrc:/Images/main_img_print.png"
        case constScanIds.scanToFile:           return "qrc:/Images/main_img_file.png"
        case constScanIds.scanToApplication:    return "qrc:/Images/main_img_apps.png"
        case constScanIds.scanToEmail:          return "qrc:/Images/main_img_email.png"
        case constScanIds.scanToFTP:            return "qrc:/Images/main_img_ftp.png"
        case constScanIds.scanToCloud:          return "qrc:/Images/main_img_cloud.png"
        }
    }

    function getDPIText(dpi){
        switch(dpi){
        case 0:   return "150DPI"
        case 1:   return "200DPI"
        case 2:   return "300DPI"
        case 3:   return "600DPI"
        }
    }

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
            source: getQuickScanSource(sid)
        }
        Text{
            text:index + ". " + getSidText(sid)
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
                text: qsTr("ResStr_DocScan_ADF1")
                font.pixelSize: 12
                width: 90
            }

            Text {
                text: adfMode ?qsTr("ResStr_DocScan_2side") :qsTr("ResStr_DocScan_1side")
                font.pixelSize: 12
            }

            Text {
                text: qsTr("ResStr_DPI1")
                font.pixelSize: 12
            }

            Text {
                text: getDPIText(dpi)
                font.pixelSize: 12
            }

            Text {
                text: qsTr("ResStr_Color_Mode1")
                font.pixelSize: 12
            }

            Text {
                text: colorMode ?qsTr("ResStr_Color") :qsTr("ResStr_Grayscale")
                font.pixelSize: 12
            }
        }
    }

}
