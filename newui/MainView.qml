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
            MouseArea{
                anchors.fill: parent
                onClicked: openDialog("About.qml" ,{})
            }
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

    Component.onCompleted: {
            jkInterface.setScanDataHandle(scanData)
    }
    Component.onDestruction: {
        jkInterface.setScanDataHandle(null)
    }

    property int originCenterX
    property int originCenterY
//    property int centerx: ApplicationWindow.window.x + ApplicationWindow.window.width / 2
//    property int centery: ApplicationWindow.window.y + ApplicationWindow.window.height / 2
    property int centerx:originCenterX
    property int centery:originCenterY
    function moveDialogToCenter(dialog){
        var x = centerx - dialog.width / 2
        var y = centery - dialog.height / 2
//        dialog.x = root.width / 2 - dialog.width / 2
//        dialog.y = root.height / 2 - dialog.height / 2
        dialog.x = x
        dialog.y = y
    }

    property var dialog
    property var window:Application.window
    function information(message ,acceptCallback ,rejectCallback){
        informationWithProperty({"message.text":message} ,acceptCallback ,rejectCallback)
    }

    function informationWithProperty(prpty ,acceptCallback ,rejectCallback){
        dialog = openDialog("component/JKMessageBox_information.qml" ,prpty ,function(dialog){
            if(typeof(acceptCallback) === "function")
                dialog.accepted.connect(acceptCallback)
            if(typeof(rejectCallback) === "function")
                dialog.rejected.connect(rejectCallback)
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
//        dialog.dialog = dialog
        if(!dialog){
            console.log("fail to create object")
        }

        if(typeof(init) === "function"){
            init(dialog)
        }
        dialog.open()
//        if(window){
//            moveDialogToCenter(dialog)
//        }

        return dialog
    }

    function openPreviewDialog(init){
        return openDialog("ImageViewer/ImagePreviewDialog.qml" ,{} ,init)
    }

    function openScanSettingDialog(setting ,mode){
        dialog = openDialog("SettingsPage/ScanSetting/ScanSettingDialog.qml" ,{"setting":setting ,"mode":mode})
    }

    function setCmd(cmd ,setting){
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
    }

    function setScanCmd(cmd ,setting){
        jkImageModel.removeAll()
        switch(cmd){
        case DeviceStruct.CMD_DecodeScan:
            setting.scanSetting.adfMode = false
            setting.scanSetting.dpi = 2
            setting.scanSetting.colorMode = true
            setting.scanSetting.mediaType = 0
            if(setting.fileName === ""){
                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("Html File Name")))
                return
//            }else if(!setting.fileName.match(/^[0-9a-zA-Z\-_.]{1,250}.html$/)){
            }else if(!setting.fileName.match(/^[^\/\\\?\*:<>|\"\(\)\[\]]{1,250}.html$/)
                     || !setting.fileName.match(/^[\x20-\x7e]*$/)){
                warningWithImage(qsTr("Invalid %1!").arg(qsTr("Html File Name")))
                return
            }
            break
        case DeviceStruct.CMD_SeperationScan:
            if(setting.filePath === ""){
                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("File Path")))
                return
            }else if(!jkInterface.pathExist(setting.filePath)){
                warningWithImage(qsTr("Invalid %1!").arg(qsTr("File Path")))
                return
            }
            break
        case DeviceStruct.CMD_QuickScan_ToFile:
            if(setting.filePath === ""){
                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("File Path")))
                return
            }else if(setting.fileName === ""){
                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("File Name")))
                return
            }else if(!jkInterface.pathExist(setting.filePath)){
                warningWithImage(qsTr("Invalid %1!").arg(qsTr("File Path")))
                return
//            }else if(!setting.fileName.match(/^[0-9a-zA-Z\-_.]{1,230}$/)){
            }else if(!setting.fileName.match(/^[^\/\\\?\*:<>|\"\(\)\[\]]{1,230}$/)
                     || !setting.fileName.match(/^[\x20-\x7e]*$/)){
                warningWithImage(qsTr("Invalid %1!").arg(qsTr("File Name")))
                return
            }
            break
        }

        setCmd(cmd ,setting)
    }

    function setSetterCmd(cmd ,setting){
        setCmd(cmd ,setting)
    }

    function setScanToCmd(cmd ,selectList ,setting)
    {
        if(cmd === DeviceStruct.CMD_ScanTo_ToFile){
            jkInterface.setScanToCmd(cmd ,selectList , setting)
        }else{
            jkInterface.setScanToCmd(cmd ,selectList , JSON.stringify(setting))
        }
    }

    property var extraCallback
    function setCmdExtra(cmd ,setting){
        console.log("quick scan extra cmd:" ,cmd)
        console.log("setting:" ,JSON.stringify(setting))
        switch(cmd){
        case DeviceStruct.CMD_Cloud_getFileList:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            extraCallback = setting.callback
            jkInterface.cmdExtra(cmd ,JSON.stringify(setting))
            break
        case DeviceStruct.CMD_Cloud_isExist:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            jkInterface.cmdExtra(cmd ,JSON.stringify(setting))
            break
        case DeviceStruct.CMD_Cloud_upload:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            jkInterface.cmdExtra(cmd ,JSON.stringify(setting))
            break
        }
    }
    function cloudUpload(setting){
        var uploadsetting = JSON.parse(setting)
        setCmdExtra(DeviceStruct.CMD_Cloud_upload ,uploadsetting)
    }

    Connections{
        target: jkInterface
        onCmdExtraResult:{
            console.log("quick scan extra cmd result:" ,cmd)
            console.log("para" ,para)
            var setting = JSON.parse(para)
            switch(cmd){

            case DeviceStruct.CMD_Cloud_getFileList:
                dialog.close()
                if(setting.isLogin){
                    setting.dialog = openDialog("ScanToPage/CloudDialog.qml" ,{"setting":setting}
                                        ,function(dialog){
                                            dialog.accepted.connect(extraCallback)
                                        })
                }else{
                    warningWithImage(qsTr("ICloud not login ,please login iCloud on MAC System Settings."))
                }
                break
            case DeviceStruct.CMD_Cloud_isExist:
                dialog.close()
                if(setting.isExist){
                    informationWithProperty({"message.text":qsTr("The file is exist ,replace it?") ,"para":JSON.stringify(setting)} ,cloudUpload)
                }else{
                    setCmdExtra(DeviceStruct.CMD_Cloud_upload ,setting)
                }

                break
            case DeviceStruct.CMD_Cloud_upload:
                dialog.close()
                break
            }
        }
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
        case sid.decodeScan:
            scanData.decodeSetting.scanSetting = setting.scanSetting
            break
        case sid.separationScan:
            scanData.separationSetting.scanSetting = setting.scanSetting
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

    function updateScanSetting(scanSetting ,powerMode){
        var constPaperSize = JSData.constPaperSize()
        var constPaperSizeMap = JSData.constPaperSizeMap()
        switch(powerMode){
        case JKEnums.PowerMode_usbBusPower:
            scanSetting.multiFeed = false
            scanSetting.adfMode = false
            scanSetting.autoCropAndDeskew = false
            scanSetting.skipBlankPage = false
            scanSetting.mediaType = 0
            scanSetting.autoColorDetection = false
            if(scanSetting.scanAreaSize === constPaperSize.indexOf(constPaperSizeMap.longPage))
                scanSetting.scanAreaSize = 0
            break
        case JKEnums.PowerMode_PowerBank:
//            scanSetting.multiFeed = false
//            scanSetting.adfMode = false
//            scanSetting.autoCropAndDeskew = false
            scanSetting.mediaType = 0
            if(scanSetting.scanAreaSize === constPaperSize.indexOf(constPaperSizeMap.longPage))
                scanSetting.scanAreaSize = 0
            break
        }
    }

    function callbackScan(para){
        console.log("callback:" ,para)
        var data = JSON.parse(para)
        var setting = JSON.parse(data.setting)
        updateScanSetting(setting.scanSetting ,data.powerMode)
        updateSetting(data.cmd ,setting)
        setScanCmd(data.cmd ,setting)
    }

    Connections{
        target: jkInterface

        onImagesCmdStart:{
            console.log("cmd to images:" ,cmd)
            var message = qsTr("processing")
            switch(cmd){
            case DeviceStruct.CMD_ScanTo_ToFTP:
            case DeviceStruct.CMD_ScanTo_ToCloud:
                message = qsTr("Uploading ,please wait...")
                break
            case DeviceStruct.CMD_ScanTo_ToPrint:
                message = qsTr("Printing picture,please wait...")
                break
            case DeviceStruct.CMD_ScanTo_ToFile:
                message = qsTr("Saving picture to file,please wait...")
                break
            case DeviceStruct.CMD_ScanTo_ToEmail:
                message = qsTr("Saving,please wait...")
                break
            case DeviceStruct.CMD_ScanTo_ToApplication:
                break

            case DeviceStruct.CMD_QuickScan_ToFTP:
                message = qsTr("Connecting to FTP Server...")
                break
            default:
                return
            }
            dialog = openDialog("component/JKMessageBox_refresh.qml" ,{"message.text":message})
        }

        onCmdToDevice:{
            console.log("cmd to device:" ,cmd)
            switch(cmd){
            case DeviceStruct.CMD_SCAN:
            case DeviceStruct.CMD_ScanTo:
                if(dialog && dialog.visible)
                    dialog.close()
                dialog = openDialog("ScanPage/ScanningDialog.qml" ,{} ,function(dialog){
                    dialog.cancel.connect(jkInterface.cancelScan)
                })
                break
            case DeviceStruct.CMD_doCalibration:
                if(dialog && dialog.visible)
                    dialog.close()
                dialog = openDialog("ScanPage/ScanningDialog.qml" ,{"text":qsTr("Calibrating...") ,"canCancel":false})
                break
            case DeviceStruct.CMD_getPowerSupply:
                break
            case DeviceStruct.CMD_getIpv4:
            case DeviceStruct.CMD_getSoftap:
            case DeviceStruct.CMD_getWifiInfo:
            case DeviceStruct.CMD_getDeviceSetting:
            case DeviceStruct.CMD_getOffTime:
            case DeviceStruct.CMD_getSaveTime:
            default:
                dialog = openDialog("component/JKRefresh.qml" ,{})
                break
            }
        }

        onSignal_deviceCmdResult:{
            console.log("scan over:" ,cmd)
            if(result && result !== JKEnums.ImageCommandResult_NoError)
                return
            var message = qsTr("processing")
            switch(cmd){
            case DeviceStruct.CMD_QuickScan_ToFTP:
            case DeviceStruct.CMD_QuickScan_ToCloud:
                message = qsTr("Uploading ,please wait...")
                break
            case DeviceStruct.CMD_QuickScan_ToPrint:
                message = qsTr("Printing picture,please wait...")
                break
            case DeviceStruct.CMD_QuickScan_ToFile:
                message = qsTr("Saving picture to file,please wait...")
                break
            case DeviceStruct.CMD_QuickScan_ToEmail:
                message = qsTr("Saving,please wait...")
                break
            case DeviceStruct.CMD_QuickScan_ToApplication:
                break

            case DeviceStruct.CMD_DecodeScan:
            case DeviceStruct.CMD_SeperationScan:
                message = qsTr("decoding,please wait...")
                break
            default:
                return
            }
            if(dialog && dialog.visible)
                dialog.close()
            dialog = openDialog("component/JKMessageBox_refresh.qml" ,{"message.text":message})
        }

        onCmdResult:{
            console.log("cmd back:" ,cmd)
            if(cmd === DeviceStruct.CMD_getPowerSupply)
                return
            console.log("result:" ,result)
            if(dialog && dialog.visible)
                dialog.close()

            switch(cmd){
            case DeviceStruct.CMD_getIpv4:
            case DeviceStruct.CMD_getSoftap:
            case DeviceStruct.CMD_getWifiInfo:
            case DeviceStruct.CMD_getDeviceSetting:
            case DeviceStruct.CMD_getOffTime:
            case DeviceStruct.CMD_getSaveTime:
                break

            case DeviceStruct.CMD_setSoftap:
            case DeviceStruct.CMD_setIpv4:
            case DeviceStruct.CMD_setWifi:
                switch(result){
                case DeviceStruct.ERR_ACK:
                    information_1button(qsTr("Configuration completed. Restart the device to apply changes."))
                    break
                case DeviceStruct.ERR_wifi_have_not_been_inited:
                    errorWithImage(qsTr("Wi-Fi not enabled ,please enable first"))
                    break
                default:
                    errorWithImage(qsTr("Configuration Failed.Please check and try again."))
                    break
                }
                break
            case DeviceStruct.CMD_setDeviceSetting:
            case DeviceStruct.CMD_setPassword:
            case DeviceStruct.CMD_setOffTime:
            case DeviceStruct.CMD_setPowerSaveTime:
            case DeviceStruct.CMD_setSaveTime:
            case DeviceStruct.CMD_clearACMCount:
            case DeviceStruct.CMD_clearRollerCount:
                switch(result){
                case DeviceStruct.ERR_ACK:
                    information_1button(qsTr("Configuration completed!Please restart device to apply settings."))
                    break
                default:
                    errorWithImage(qsTr("Configuration Failed.Please check your setting and device,then try again."))
                    break
                }
                break

            case DeviceStruct.CMD_QuickScan_ToPrint:
            case DeviceStruct.CMD_ScanTo_ToPrint:
                switch(result){
                case JKEnums.ImageCommandResult_error_invalidPrinter:
                    warningWithImage(qsTr("Not find printer!"))
                    break
                case DeviceStruct.ERR_ACK:
                case JKEnums.ImageCommandResult_NoError:
                    if(DeviceStruct.CMD_ScanTo_ToPrint === cmd)
                        break
                    if(jkImageModel.count > 0){
                        dialog = openPreviewDialog(function(dialog){
                            dialog.title = qsTr("Quick Scan Preview")
                            dialog.showExtra = true
                            dialog.index = 0
                            dialog.save = false
                            dialog.name = qsTr("Scan to Print completed")
                            dialog.requestImage(0 ,0)
                        })
                    }
                    break;
                default:
                    scanResult(cmd ,result ,data)
                    break
                }
                break
            case DeviceStruct.CMD_QuickScan_ToFile:
            case DeviceStruct.CMD_ScanTo_ToFile:
                switch(result){
                case JKEnums.ImageCommandResult_error_saveFile:
                    warningWithImage(qsTr("Fail to save files."))
                    break
                case JKEnums.ImageCommandResult_error_invalidFilePath:
                    warningWithImage(qsTr("invalid File Path."))
                    break
                case DeviceStruct.ERR_ACK:
                case JKEnums.ImageCommandResult_NoError:
                    if(DeviceStruct.CMD_ScanTo_ToFile === cmd){
                        break
                    }
                    if(jkImageModel.count > 0){
                        dialog = openPreviewDialog(function(dialog){
                            dialog.title = qsTr("Quick Scan Preview")
                            dialog.showExtra = true
                            dialog.index = 0
                            dialog.save = false
                            dialog.name = qsTr("Save files completed")
                            dialog.requestImage(0 ,0)
                        })
                    }
                    break
                default:
                    if(!scanResult(cmd ,result ,data))
                        errorWithImage(qsTr("Scan Fail!"))
                    break
                }
                break
            case DeviceStruct.CMD_QuickScan_ToEmail:
                switch(result){
                case DeviceStruct.ERR_ACK:
                case JKEnums.ImageCommandResult_NoError:
                    if(jkImageModel.count > 0){
                        dialog = openPreviewDialog(function(dialog){
                            dialog.title = qsTr("Quick Scan Preview")
                            dialog.showExtra = true
                            dialog.index = 0
                            dialog.save = false
                            dialog.name = qsTr("Scan to Email completed")
                            dialog.requestImage(0 ,0)
                        })
                    }
                    break
                default:
                    if(!scanResult(cmd ,result ,data))
                        errorWithImage(qsTr("Scan Fail!"))
                    break
                }
                break
            case DeviceStruct.CMD_QuickScan_ToFTP:
            case DeviceStruct.CMD_ScanTo_ToFTP:
                switch(result){
                case JKEnums.ImageCommandResult_error_ftpTimeout:
                    warningWithImage(qsTr("Time out.Unable to connet to the remote server."))
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

                case DeviceStruct.ERR_ACK:
                case JKEnums.ImageCommandResult_NoError:
                    if(DeviceStruct.CMD_ScanTo_ToFTP === cmd){
                        information_1button(qsTr("Upload complete"))
                        break
                    }
                    if(jkImageModel.count > 0){
                        dialog = openPreviewDialog(function(dialog){
                            dialog.title = qsTr("Quick Scan Preview")
                            dialog.showExtra = true
                            dialog.index = 0
                            dialog.save = false
                            dialog.name = qsTr("Upload completed")
                            dialog.requestImage(0 ,0)
                        })
                    }
                    break
                default:
                    if(!scanResult(cmd ,result ,data))
                        errorWithImage(qsTr("Scan Fail!"))
                    break
                }
                break
            case DeviceStruct.CMD_ScanTo_ToCloud:
            case DeviceStruct.CMD_QuickScan_ToCloud:
                switch(result){
                case JKEnums.ImageCommandResult_error_icloudNotLogin:
                    warningWithImage(qsTr("ICloud not login ,please login iCloud on MAC System Settings."))
                    break
                case JKEnums.ImageCommandResult_error_icloudeUpload:
                    warningWithImage(qsTr("ICloud upload fail."))
                    break
                case DeviceStruct.ERR_ACK:
                case JKEnums.ImageCommandResult_NoError:
                    if(DeviceStruct.CMD_ScanTo_ToCloud === cmd){
                        information_1button(qsTr("Upload complete"))
                        break
                    }
                    if(jkImageModel.count > 0){
                        dialog = openPreviewDialog(function(dialog){
                            dialog.title = qsTr("Quick Scan Preview")
                            dialog.showExtra = true
                            dialog.index = 0
                            dialog.save = false
                            dialog.name = qsTr("Upload completed")
                            dialog.requestImage(0 ,0)
                        })
                    }
                    break
                default:
                    if(!scanResult(cmd ,result ,data))
                        errorWithImage(qsTr("Scan Fail!"))
                    break
                }
                break
            case DeviceStruct.CMD_doCalibration:
                switch(result){
                case DeviceStruct.ERR_ACK:
                    information_1button(qsTr("Calibration completed! Please restart your device."))
                    break

                default:
                    if(!scanResult(cmd ,result ,data))
                        errorWithImage(qsTr("Calibration Failed!"))
                    break
                }
                break
            case DeviceStruct.CMD_ScanTo_ToEmail:
            case DeviceStruct.CMD_QuickScan_ToApplication:
            case DeviceStruct.CMD_ScanTo_ToApplication:
            case DeviceStruct.CMD_DecodeScan:
            case DeviceStruct.CMD_SeperationScan:
            case DeviceStruct.CMD_SCAN:
            case DeviceStruct.CMD_ScanTo://handler in ScanPage.qml too
            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("Scan Fail!"))
                break
            }
        }
    }

    function scanResult(cmd ,result ,data){
        var ret = true
        switch(result){
        case DeviceStruct.ERR_ACK:
        case JKEnums.ImageCommandResult_NoError:
            break
        case DeviceStruct.ERR_RETSCAN_OPENFAIL:
        case DeviceStruct.ERR_RETSCAN_OPENFAIL_NET:
            errorWithImage(qsTr("The Device is not ready! Please check your computer setting or power on and plug in your device."))
            break
        case DeviceStruct.ERR_RETSCAN_BUSY:
        case DeviceStruct.ERR_RETSCAN_JOB_GOGING:
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
        case DeviceStruct.ERR_RETSCAN_ADFPATH_NOT_READY:
        case DeviceStruct.ERR_RETSCAN_ADFDOC_NOT_READY:
            errorWithImage(qsTr("ADF is not ready"))
            break
        case DeviceStruct.ERR_RETSCAN_ADFCOVER_NOT_READY:
            errorWithImage(qsTr("Cover(ADF) is opened!"))
            break
        case DeviceStruct.ERR_RETSCAN_HOME_NOT_READY:
            errorWithImage(qsTr("Home is not ready"))
            break
        case DeviceStruct.ERR_RETSCAN_ULTRA_SONIC:
            errorWithImage(qsTr("Multi-feed error"))
            break
        case DeviceStruct.ERR_RETSCAN_CANCEL:
            information_1button("The scan job has been canceled")
            break
        case DeviceStruct.ERR_RETSCAN_ERROR_POWER1:
            var para1 = {}
            para1.powerMode = JKEnums.PowerMode_PowerBank
            para1.cmd = cmd
            para1.setting = data
            informationWithProperty({"message.text":qsTr("The scan job could not be continued, because the Power Bank mode does not support the following settings.

    Media Type:      \tDeposit Book or Card
    Scan Area Size:  \tLong Page Mode

If you select 'Yes', the scan job will be continued, but the following settings will be changed
If you select 'No', the scan job will be canceled!

    Media Type:      \tNormal
    Scan Area Size:  \tAuto")
                                        ,"message.horizontalAlignment":Text.AlignLeft
                                        ,"para":JSON.stringify(para1)
                                        ,"height":380
                                        },callbackScan )
            break
        case DeviceStruct.ERR_RETSCAN_ERROR_POWER2:
            if(scanData.currentDevice.match(/^usb+/i)){
                var para = {}
                para.powerMode = JKEnums.PowerMode_usbBusPower
                para.cmd = cmd
                para.setting = data
                informationWithProperty({"message.text":qsTr("The scan job could not be continued, because the USB Bus Power mode does not support the following settings.

    ADF Mode:              \tTwo Side
    Media Type:            \tDeposit Book or Card
    Scan Area Size:        \tLong Page Mode
    Multi Feed Detection:  \tOn
    Skip Blank Page:       \tOn
    Auto Color Detection:  \tOn

If you select 'Yes', the scan job will be continued, but the following settings will be changed
If you select 'No', the scan job will be canceled!

    ADF Mode:              \tOne Side
    Media Type:            \tNormal
    Scan Area Size:        \tAuto
    Multi Feed Detection:  \tOff
    Skip Blank Page:       \tOff
    Auto Color Detection:  \tOff")
                                            ,"message.horizontalAlignment":Text.AlignLeft
                                            ,"para":JSON.stringify(para)
                                            ,"height":480
                                            },callbackScan )
            }else{
                errorWithImage(qsTr("The scan job could not be continued, because the USB Bus power mode do not support WIFI scanning."))
            }
            break

        case DeviceStruct.ERR_RETSCAN_MEMORY_FULL:
            errorWithImage(qsTr("Device Memory is full!"))
            break
        case DeviceStruct.ERR_RETSCAN_USB_TRANSFERERROR:
        case DeviceStruct.ERR_RETSCAN_WIFI_TRANSFERERROR:
            errorWithImage(qsTr("The Device transfering has some error!"))
            break
        case DeviceStruct.ERR_RETSCAN_GETINFO_FAIL:
        case DeviceStruct.ERR_RETSCAN_CREATE_JOB_FAIL:
        case DeviceStruct.ERR_RETSCAN_ERRORPARAMETER:
        case DeviceStruct.ERR_RETSCAN_ERROR:
        case DeviceStruct.ERR_communication:
            errorWithImage(qsTr("The device is disconnected, the scanning will be canceled!"))
            break
        case DeviceStruct.ERR_scanImagesAreAllBlank:
            warningWithImage(qsTr("Scan Images are all blank!"))
            break
        default:
            ret = false
            console.log("err:" ,result)
//            errorWithImage(qsTr("Scan Fail!"))
            break;
        }
        return ret
    }
}
