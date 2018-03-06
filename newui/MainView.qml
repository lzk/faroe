import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import "component"
import "ScanPage"
import "component/path"
import com.liteon.JKInterface 1.0
import "ScanData.js" as JSData

Item {
    id:root
    width: 850
    height: 638

    signal minimized
    signal closed
    signal move(real dx ,real dy)

    Image {
        id: image_background
        anchors.fill: parent
        source: "qrc:/Images/background3.png"
    }

    RowLayout {
        id: rowLayout
        height: 45
        spacing: 5
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        Image {
            id: image
            Layout.maximumHeight: 32
            Layout.minimumHeight: 32
            Layout.maximumWidth: 32
            Layout.minimumWidth: 32
            source: "qrc:/Images/VOPicon.png"
        }

        Text {
            id: text1
            text: qsTr("VOP")
            Layout.fillWidth: true
            font.pixelSize: 16
        }

        Row{
            JKToolButton{
                id: button_minimize
                JKPath_minimize{
                    anchors.fill: parent
                }
            }
            JKToolButton{
                id: button_close
                JKPath_close{
                    anchors.fill: parent
                }
            }
        }
    }

    StackView{
        id:stackview
        anchors.rightMargin: 50
        anchors.leftMargin: 50
        anchors.bottomMargin: 30
        anchors.top: rowLayout.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 30
        initialItem: ScanPage{

        }
    }

    MouseArea{
        id:mouseArea
        z:-1
        anchors.fill: rowLayout
    }

    Connections{
        target: button_minimize
        onClicked: minimized()
    }
    Connections{
        target: button_close
        onClicked:{
            information(qsTr("Do you want to exit the VOP?") ,toexit)
        }
    }
    function toexit(){
        closed()
        jkImageModel.removeAll()
    }

    Connections{
        target: mouseArea
        property real dx
        property real dy
        onPositionChanged: {
            move(mouse.x - dx ,mouse.y - dy)
            originCenterX += mouse.x - dx
            originCenterY += mouse.y - dy
        }
        onPressed: {
            dx = mouse.x
            dy = mouse.y
        }
    }

    ScanData{
        id:scanData
        objectName: "scanData"
    }

    property int originCenterX
    property int originCenterY
//    property int centerx: ApplicationWindow.window.x + ApplicationWindow.window.width / 2
//    property int centery: ApplicationWindow.window.y + ApplicationWindow.window.height / 2
    property int centerx:originCenterX
    property int centery:originCenterY
    function moveDialogToCenter(dialog){
        dialog.x = centerx - dialog.width / 2
        dialog.y = centery - dialog.height / 2
    }

    property var dialog
    property var window:Application.window
    function information(message ,acceptCallback){
        informationWithProperty({"message.text":message} ,acceptCallback)
    }

    function informationWithProperty(prpty ,acceptCallback){
        dialog = openDialog("component/JKMessageBox_information.qml" ,prpty ,function(dialog){
            if(typeof(acceptCallback) === "function")
                dialog.accepted.connect(acceptCallback)
            })
    }

    function information_1button(message){
        warning({
                    "toolbar.text.text":qsTr("Information")
                    ,"message.horizontalAlignment":Text.AlignLeft
                    ,"message.text":message
                })
    }

    function warningWithImage(message){
        warning({"showImage":true,"message.text":message})
    }
    function errorWithImage(message){
        warning({"showImage":true,"message.text":message,"image.source":"qrc:/Images/warning.png","toolbar.text.text":qsTr("Error")})
    }

    function warning(properties){
        dialog = openDialog("component/JKMessageBox_warning.qml" ,properties)
    }

    function openMediaTypePromptDialog(){
        dialog = openDialog("SettingsPage/ScanSetting/MediaTypePrompt.qml" ,{})
    }

    function openDialog(source ,properties ,init){
        var component = Qt.createComponent(source)
        var dialog = component.createObject(window ,properties)
        dialog.dialog = dialog
        if(typeof(init) === "function"){
            init(dialog)
        }
        dialog.open()
        if(!window)
            moveDialogToCenter(dialog)
        return dialog
    }

    function openScanSettingDialog(setting){
        dialog = openDialog("SettingsPage/ScanSetting/ScanSettingDialog.qml" ,{"setting":setting})
    }

    function setScanCmd(cmd ,setting){
        if(!scanData.deviceStatus){
            warningWithImage(qsTr("Scanning connection failed"))
            return
        }
        jkInterface.setCmd(cmd ,JSON.stringify(setting));
        dialog = openDialog("ScanPage/ScanningDialog.qml" ,{} ,function(dialog){
            dialog.cancel.connect(jkInterface.cancelScan)
        })
    }

    function setSetterCmd(cmd ,setting){
        switch(cmd){
        case DeviceStruct.CMD_getIpv4:
        case DeviceStruct.CMD_getSoftap:
        case DeviceStruct.CMD_getWifiInfo:
        case DeviceStruct.CMD_getDeviceSetting:
        case DeviceStruct.CMD_getOffTime:
        case DeviceStruct.CMD_getSaveTime:
            break

        default:
            if(!scanData.deviceStatus){
                warningWithImage(qsTr("Scanning connection failed"))
                return
            }
            break
        }
        jkInterface.setCmd(cmd ,JSON.stringify(setting));
        dialog = openDialog("component/JKRefresh.qml" ,{})
    }

    function setScanToCmd(cmd ,selectList ,setting)
    {
        if(cmd === DeviceStruct.CMD_ScanTo_ToFile){
            jkInterface.setScanToCmd(cmd ,selectList , setting)
        }else{
            jkInterface.setScanToCmd(cmd ,selectList , JSON.stringify(setting))
        }

        var message = qsTr("processing")
        switch(cmd){
        case DeviceStruct.CMD_ScanTo_ToFTP:
        case DeviceStruct.CMD_ScanTo_ToCloud:
            message = qsTr("Upload,please wait!")
            break
        case DeviceStruct.CMD_ScanTo_ToPrint:
            return
        }
        dialog = openDialog("component/JKMessageBox_refresh.qml" ,{"message.text":message})
    }

    function updateSetting(cmd ,setting){
//        switch(cmd){
//        case DeviceStruct.CMD_ScanTo:
//        case DeviceStruct.CMD_ScanTo_ToPrint:
//        case DeviceStruct.CMD_ScanTo_ToFile:
//        case DeviceStruct.CMD_ScanTo_ToApplication:
//        case DeviceStruct.CMD_ScanTo_ToEmail:
//        case DeviceStruct.CMD_ScanTo_ToFTP:
//        case DeviceStruct.CMD_ScanTo_ToCloud:
//            break;
//        case DeviceStruct.CMD_QuickScan_ToPrint:
//        case DeviceStruct.CMD_QuickScan_ToFile:
//        case DeviceStruct.CMD_QuickScan_ToApplication:
//        case DeviceStruct.CMD_QuickScan_ToEmail:
//        case DeviceStruct.CMD_QuickScan_ToFTP:
//        case DeviceStruct.CMD_QuickScan_ToCloud:
//        case DeviceStruct.CMD_DecodeScan:
//        case DeviceStruct.CMD_SeperationScan:
//            break;
//        default:
//                break
//        }
        var sid = JSData.constScanIds()
        switch(setting.sid){
        case sid.scanTo:
            scanData.scanToParameter.scanSetting = setting.scanSetting
            break
        case sid.qrcodeScan:
            scanData.qrcodeSetting.scanSetting = setting.scanSetting
            break
        case sid.scanToPrint:
        case sid.scanToFile:
        case sid.scanToApplication:
        case sid.scanToEmail:
        case sid.scanToFTP:
        case sid.scanToCloud:
            scanData.findQuickScanSettingViaName(setting.name).scanSetting = setting.scanSetting
            break
        }
    }

    function callbackScan(para){
        console.log("callback:" ,para)
        var data = JSON.parse(para)
        var setting = JSON.parse(data.setting)
        var scanSetting = setting.scanSetting
        var constPaperSize = JSData.constPaperSize()
        var constPaperSizeMap = JSData.constPaperSizeMap()
        switch(data.id){
        case 0:
            scanSetting.multiFeed = false
            scanSetting.adfMode = false
            scanSetting.autoCropAndDeskew = false
            scanSetting.mediaType = 0
            if(scanSetting.scanAreaSize === constPaperSize.indexOf(constPaperSizeMap.longPage))
                scanSetting.scanAreaSize = 0
            break
        case 1:
//            scanSetting.multiFeed = false
//            scanSetting.adfMode = false
            scanSetting.autoCropAndDeskew = false
            scanSetting.mediaType = 0
            if(scanSetting.scanAreaSize === constPaperSize.indexOf(constPaperSizeMap.longPage))
                scanSetting.scanAreaSize = 0
            break
        }
        updateSetting(data.cmd ,setting)
        setScanCmd(data.cmd ,setting)
    }

    Connections{
        target: jkInterface
        onCmdResult:{
            console.log("result:" ,result)
            switch(cmd){
            case DeviceStruct.CMD_getPowerSupply:
                return
            }
            dialog.close()

            switch(cmd){
            case DeviceStruct.CMD_getIpv4:
            case DeviceStruct.CMD_getSoftap:
            case DeviceStruct.CMD_getWifiInfo:
            case DeviceStruct.CMD_getDeviceSetting:
            case DeviceStruct.CMD_getOffTime:
            case DeviceStruct.CMD_getSaveTime:
                return
            }

            switch(result){
            case DeviceStruct.ERR_RETSCAN_OPENFAIL:
                errorWithImage(qsTr("The Device is not ready!"))
                break
            case DeviceStruct.ERR_RETSCAN_OPENFAIL_NET:
                errorWithImage(qsTr("The Device is not ready!"))
                break
            case DeviceStruct.ERR_RETSCAN_BUSY:
                errorWithImage(qsTr("The Device is currently in use. Confirm that the device is available and try again."))
                break
            case DeviceStruct.ERR_RETSCAN_PAPER_JAM:
                errorWithImage(qsTr("Paper jam"))
                break
            case DeviceStruct.ERR_RETSCAN_COVER_OPEN:
                errorWithImage(qsTr("Cover is opened"))
                break
            case DeviceStruct.ERR_RETSCAN_PAPER_NOT_READY:
                errorWithImage(qsTr("Paper is not ready!"))
                break
            case DeviceStruct.ERR_RETSCAN_ADFCOVER_NOT_READY:
            case DeviceStruct.ERR_RETSCAN_ADFPATH_NOT_READY:
            case DeviceStruct.ERR_RETSCAN_ADFDOC_NOT_READY:
                errorWithImage(qsTr("ADF is not ready"))
                break
            case DeviceStruct.ERR_RETSCAN_HOME_NOT_READY:
                errorWithImage(qsTr("Home is not ready"))
                break
            case DeviceStruct.ERR_RETSCAN_ULTRA_SONIC:
                errorWithImage(qsTr("Multi-feed error"))
                break
            case DeviceStruct.ERR_RETSCAN_CANCEL:
                break
            case DeviceStruct.ERR_RETSCAN_ERROR_POWER1:
                var para1 = {}
                para1.id = 1
                para1.cmd = cmd
                para1.setting = data
                informationWithProperty({"message.text":qsTr("The scan job could not be continued, because the Power Bank mode do not support the following settings.

\tMediaType:   \tDeposit Book or Card
\tScan Size:   \tLong Page Mode

If you select 'Yes', the scan job will be continue, but the following settings will be changed

If you select 'No', the scan job will be canceled!

\tMediaType:   \tNormal
\tScan Size:   \tAuto")
                                            ,"message.horizontalAlignment":Text.AlignLeft
                                            ,"para":JSON.stringify(para1)
                                            ,"height":380
                                            },callbackScan )
                break
            case DeviceStruct.ERR_RETSCAN_ERROR_POWER2:
                if(scanData.currentDevice.match(/^usb+/i)){
                    var para = {}
                    para.id = 0
                    para.cmd = cmd
                    para.setting = data
                    informationWithProperty({"message.text":qsTr("The scan job could not be continued, because the USB Bus power mode do not support the follow settings.

\tADF Mode:   \tTwo Side
\tMediaType:   \tDeposit Book or Card
\tScan Size:   \tLong Page Mode

If you select 'Yes', the scan job will be continue, but the following settings will be changed

If you select 'No', the scan job will be canceled!

\tADF Mode:   \tOne Side
\tMediaType:   \tNormal
\tScan Size:   \tAuto")
                                                ,"message.horizontalAlignment":Text.AlignLeft
                                                ,"para":JSON.stringify(para)
                                                ,"height":440
                                                },callbackScan )
                }else{
                    errorWithImage(qsTr("The scan job could not be continued, because the USB Bus power mode do not support WIFI scanning."))
                }
                break
            case DeviceStruct.ERR_wifi_have_not_been_inited:
                errorWithImage(qsTr("Wi-Fi not enabled ,please enable first"))
                break
            case DeviceStruct.ERR_ACK:
            case JKEnums.ImageCommandResult_NoError:
                switch(cmd){
                case DeviceStruct.CMD_ScanTo_ToFTP:
                case DeviceStruct.CMD_QuickScan_ToFTP:
                case DeviceStruct.CMD_ScanTo_ToCloud:
                case DeviceStruct.CMD_QuickScan_ToCloud:
                    information_1button(qsTr("Upload complete"))
                    break
                }
                break
            case JKEnums.ImageCommandResult_error_ftpConnect:
                warningWithImage(qsTr("Upload failed.Unable to connet to the remote server."))
                break
            case JKEnums.ImageCommandResult_error_ftpLogin:
                warningWithImage(qsTr("Upload failed.The remote server returned an error:(530) Not logged in."))
                break
            case JKEnums.ImageCommandResult_error_ftpCd:
            case JKEnums.ImageCommandResult_error_ftpPut:
                warningWithImage(qsTr("Upload failed.The remote server returned an error:(553) File name not allowed."))
                break
            case JKEnums.ImageCommandResult_error_icloudNotLogin:
                warningWithImage(qsTr("ICloud not login ,please login iCloud on MAC System Settings."))
                break
            case JKEnums.ImageCommandResult_error_icloudeUpload:
                warningWithImage(qsTr("ICloud upload fail."))
                break

            default:
                console.log("err:" ,result)
                errorWithImage(qsTr("Fail!"))
                break;
            }

        }
    }
}
