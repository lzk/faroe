import QtQuick 2.7
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

//    focus: true
//    KeyNavigation.tab: image

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
            source: "qrc:/Images/icon-20@2x.png"

//            KeyNavigation.tab: button_minimize
            activeFocusOnTab: true
            Keys.onEnterPressed: about()
            Keys.onReturnPressed: about()

            JKDashRectange{
                id:dashRectange
                anchors.fill: parent
                anchors.margins: -2
                radius: 0
                visible: image.activeFocus
            }

            MouseArea{
                anchors.fill: parent
                onClicked: about()
            }
        }

        Text {
            id: text1
            text: qsTr("ResStr_DocScan")
            Layout.fillWidth: true
            font.pixelSize: 16
        }

        Row{
            JKToolButton{
                id: button_minimize
//                KeyNavigation.tab: button_close
                JKPath_minimize{
                    anchors.fill: parent
                }
            }
            JKToolButton{
                id: button_close
//                KeyNavigation.tab: stackview
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
//        activeFocusOnTab: true
//        KeyNavigation.tab: image
        initialItem: ScanPage{
            id:scanpage
        }
//        onActiveFocusChanged: {
//            currentItem.focus = activeFocus
//        }
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
            quit()
        }
    }

    function about(){
        openDialog("About.qml" ,{})
    }
    function quit(){
        information(qsTr("ResStr_Do_you_want_to_exit") ,toexit)
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
                    "toolbar.text.text":qsTr("ResStr_DocScan_Info")
                    ,"message.horizontalAlignment":Text.AlignLeft
                    ,"message.text":message
                })
    }

    function warningWithImage(message){
        warning({"showImage":true,"message.text":message})
    }
    function errorWithImage(message){
        warning({"showImage":true,"message.text":message,"image.source":"qrc:/Images/warning.png","toolbar.text.text":qsTr("ResStr_Error")})
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
        case DeviceStruct.CMD_ScanTo:
        case DeviceStruct.CMD_QuickScan_ToPrint:
        case DeviceStruct.CMD_QuickScan_ToFile:
        case DeviceStruct.CMD_QuickScan_ToApplication:
        case DeviceStruct.CMD_QuickScan_ToEmail:
        case DeviceStruct.CMD_QuickScan_ToFTP:
        case DeviceStruct.CMD_QuickScan_ToCloud:
        case DeviceStruct.CMD_DecodeScan:
        case DeviceStruct.CMD_SeperationScan:
            if(!scanData.deviceStatus){
                warningWithImage(qsTr("ResStr_DocScan_scan_conn_fail"))
                return
            }
            break
//        case DeviceStruct.CMD_getIpv4:
//        case DeviceStruct.CMD_getSoftap:
//        case DeviceStruct.CMD_getWifiInfo:
//        case DeviceStruct.CMD_getDeviceSetting:
//        case DeviceStruct.CMD_getOffTime:
//        case DeviceStruct.CMD_getSaveTime:
        default:
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
//                warningWithImage(qsTr("The %1 cannot be empty!").arg(qsTr("Html File Name")))
                warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_Output_Result")))
                return
//            }else if(!setting.fileName.match(/^[0-9a-zA-Z\-_.]{1,250}.html$/)){
            }else if(!setting.fileName.match(/^[^\/\\\?\*:<>|\"\(\)\[\]]{1,250}$/)
                     || !setting.fileName.match(/^[\x20-\x7e]*$/)){
//                warningWithImage(qsTr("Invalid %1!").arg(qsTr("Html File Name")))
                warningWithImage(qsTr("ResStr_Invalid_xxx").arg(qsTr("ResStr_Output_Result")))
                return
            }
            break
        case DeviceStruct.CMD_SeperationScan:
            if(setting.filePath === ""){
                warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_DocScan_File_Path1")))
                return
            }else if(!jkInterface.pathExist(setting.filePath)){
                warningWithImage(qsTr("ResStr_Invalid_xxx").arg(qsTr("ResStr_DocScan_File_Path1")))
                return
            }
            break
        case DeviceStruct.CMD_QuickScan_ToFile:
            if(setting.filePath === ""){
                warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_DocScan_File_Path1")))
                return
            }else if(setting.fileName === ""){
                warningWithImage(qsTr("ResStr_could_not_be_empty").arg(qsTr("ResStr_File_Name1")))
                return
            }else if(!jkInterface.pathExist(setting.filePath)){
                warningWithImage(qsTr("ResStr_Invalid_xxx").arg(qsTr("ResStr_DocScan_File_Path1")))
                return
//            }else if(!setting.fileName.match(/^[0-9a-zA-Z\-_.]{1,230}$/)){
            }else if(!setting.fileName.match(/^[^\/\\\?\*:<>|\"\(\)\[\]]{1,230}$/)
                     || !setting.fileName.match(/^[\x20-\x7e]*$/)){
                warningWithImage(qsTr("ResStr_Invalid_xxx").arg(qsTr("ResStr_File_Name1")))
                return
            }
            break
           case DeviceStruct.CMD_QuickScan_ToPrint:
               if(setting.printerName === ""){
                    setting.printerName = jkInterface.getDefaultPrinterName()
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
        jkInterface.setCmd(cmd ,JSON.stringify(setting) ,jkImageModel.getFileList(selectList))
    }

    property var extraCallback
    function setCmdExtra(cmd ,setting){
        switch(cmd){
        case DeviceStruct.CMD_Cloud_getFileList:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            extraCallback = setting.callback
            jkInterface.setCmd(cmd ,JSON.stringify(setting))
            break
        case DeviceStruct.CMD_Cloud_isExist:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            jkInterface.setCmd(cmd ,JSON.stringify(setting))
            break
        case DeviceStruct.CMD_Cloud_upload:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            jkInterface.setCmd(cmd ,JSON.stringify(setting))
            break
        }
    }
    function cloudUpload(setting){
        var uploadsetting = JSON.parse(setting)
        setCmdExtra(DeviceStruct.CMD_Cloud_upload ,uploadsetting)
    }

    function commonResult(cmd ,para){
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
        default:
            break
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
            scanSetting.autoColorDetection = false
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

    function scanResult(cmd ,result ,data){
        var ret = true
        switch(result){
        case JKEnums.ImageCommandResult_error_cancel:
            switch(cmd){
            case DeviceStruct.CMD_ScanTo_ToPrint:
            case DeviceStruct.CMD_ScanTo_ToFile:
            case DeviceStruct.CMD_ScanTo_ToFTP:
            case DeviceStruct.CMD_ScanTo_ToCloud:
            case DeviceStruct.CMD_ScanTo_ToEmail:
            case DeviceStruct.CMD_ScanTo_ToApplication:
                console.log("image handle cancel")
                break
            default:
                information_1button(qsTr("ResStr_Scanning_is_canceled_on_machine"))
                break
            }
            break
        case DeviceStruct.ERR_ACK:
        case JKEnums.ImageCommandResult_NoError:
            break
        case DeviceStruct.ERR_RETSCAN_OPENFAIL:
        case DeviceStruct.ERR_RETSCAN_OPENFAIL_NET:
            errorWithImage(qsTr("ResStr_DocScan_Device_Not_Ready"))
            break
        case DeviceStruct.ERR_RETSCAN_BUSY:
        case DeviceStruct.ERR_RETSCAN_JOB_GOGING:
            errorWithImage(qsTr("ResStr_DocScan_net_scanner_busy"))
            break
        case DeviceStruct.ERR_RETSCAN_PAPER_JAM:
            errorWithImage(qsTr("ResStr_DocScan_paper_jam"))
            break
        case DeviceStruct.ERR_RETSCAN_COVER_OPEN:
            errorWithImage(qsTr("ResStr_DocScan_cover_open"))
            break
        case DeviceStruct.ERR_RETSCAN_PAPER_NOT_READY:
            errorWithImage(qsTr("ResStr_DocScan_paper_not_ready"))
            break
        case DeviceStruct.ERR_RETSCAN_ADFPATH_NOT_READY:
        case DeviceStruct.ERR_RETSCAN_ADFDOC_NOT_READY:
            errorWithImage(qsTr("ResStr_DocScan_adf_not_ready"))
            break
        case DeviceStruct.ERR_RETSCAN_ADFCOVER_NOT_READY:
            errorWithImage(qsTr("ResStr_DocScan_Cover_Open"))
            break
        case DeviceStruct.ERR_RETSCAN_HOME_NOT_READY:
            errorWithImage(qsTr("ResStr_DocScan_home_not_ready"))
            break
        case DeviceStruct.ERR_RETSCAN_ULTRA_SONIC:
            errorWithImage(qsTr("ResStr_DocScan_multifeed_error"))
            break
        case DeviceStruct.ERR_RETSCAN_CANCEL:
            information_1button(qsTr("ResStr_Scanning_is_canceled_on_machine"))
            break
        case DeviceStruct.ERR_RETSCAN_ERROR_POWER1:
            var para1 = {}
            para1.powerMode = JKEnums.PowerMode_PowerBank
            para1.cmd = cmd
            para1.setting = data
            informationWithProperty({"message.text":qsTr("ResStr_DocScan_Power_Bank")
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
                informationWithProperty({"message.text":qsTr("ResStr_DocScan_Power_Bus")
                                            ,"message.horizontalAlignment":Text.AlignLeft
                                            ,"para":JSON.stringify(para)
                                            ,"height":480
                                            },callbackScan )
            }else{
                errorWithImage(qsTr("ResStr_DocScan_Power_Bus_Wifi"))
            }
            break

        case DeviceStruct.ERR_RETSCAN_MEMORY_FULL:
            errorWithImage(qsTr("ResStr_DocScan_memory_full"))
            break
        case DeviceStruct.ERR_RETSCAN_USB_TRANSFERERROR:
        case DeviceStruct.ERR_RETSCAN_WIFI_TRANSFERERROR:
            errorWithImage(qsTr("ResStr_DocScan_Transfer_Error"))
            break
        case DeviceStruct.ERR_RETSCAN_GETINFO_FAIL:
        case DeviceStruct.ERR_RETSCAN_CREATE_JOB_FAIL:
        case DeviceStruct.ERR_RETSCAN_ERRORPARAMETER:
        case DeviceStruct.ERR_RETSCAN_ERROR:
        case DeviceStruct.ERR_communication:
            errorWithImage(qsTr("ResStr_DocScan_Device_Disconnected"))
            break
        case DeviceStruct.ERR_scanImagesAreAllBlank:
            warningWithImage(qsTr("ResStr_DocScan_scan_all_blank"))
            break
        case DeviceStruct.ERR_RETSCAN_CANCEL_LAST:
            errorWithImage(qsTr("ResStr_Scanning_is_canceled_Last"))
            break
        default:
            ret = false
            console.log("err:" ,result)
//            errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
            break;
        }
        return ret
    }

    function startPhase(cmd ,result){
        console.log("start phase:" ,cmd)
        if(result){
            return
        }

        switch(cmd){
        case DeviceStruct.CMD_DecodeScan:
        case DeviceStruct.CMD_SeperationScan:
        case DeviceStruct.CMD_QuickScan_ToPrint:
        case DeviceStruct.CMD_QuickScan_ToFile:
        case DeviceStruct.CMD_QuickScan_ToEmail:
        case DeviceStruct.CMD_QuickScan_ToApplication:
        case DeviceStruct.CMD_QuickScan_ToFTP:
        case DeviceStruct.CMD_QuickScan_ToCloud:
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
            dialog = openDialog("ScanPage/ScanningDialog.qml" ,{"text":qsTr("ResStr_Calibrating") ,"canCancel":false})
            break
        case DeviceStruct.CMD_getIpv4:
        case DeviceStruct.CMD_getSoftap:
        case DeviceStruct.CMD_getWifiInfo:
        case DeviceStruct.CMD_getDeviceSetting:
        case DeviceStruct.CMD_getOffTime:
        case DeviceStruct.CMD_getSaveTime:
            dialog = openDialog("component/JKRefresh.qml" ,{})
            break
        case DeviceStruct.CMD_ScanTo_ToPrint:
        case DeviceStruct.CMD_ScanTo_ToFile:
        case DeviceStruct.CMD_ScanTo_ToEmail:
        case DeviceStruct.CMD_ScanTo_ToApplication:
        case DeviceStruct.CMD_ScanTo_ToCloud:
        case DeviceStruct.CMD_ScanTo_ToFTP:
            break
        case DeviceStruct.CMD_getPowerSupply:
            break
        default:
            break
        }
    }

    function processPhase(cmd ,result){
        console.log("process phase:" ,cmd)
        if(result){
            return
        }
        var message = qsTr("processing")
        switch(cmd){
        case DeviceStruct.CMD_QuickScan_ToFTP:
        case DeviceStruct.CMD_QuickScan_ToCloud:
        case DeviceStruct.CMD_ScanTo_ToFTP:
        case DeviceStruct.CMD_ScanTo_ToCloud:
//            message = qsTr("Connecting to FTP Server...")
            message = qsTr("ResStr_DocScan_upload_wait")
            break
        case DeviceStruct.CMD_QuickScan_ToPrint:
        case DeviceStruct.CMD_ScanTo_ToPrint:
            message = qsTr("ResStr_DocScan_print_pic_wait")
            break
        case DeviceStruct.CMD_QuickScan_ToFile:
        case DeviceStruct.CMD_ScanTo_ToFile:
            message = qsTr("ResStr_DocScan_Saving_pic_TIFF")
            break
        case DeviceStruct.CMD_QuickScan_ToEmail:
        case DeviceStruct.CMD_ScanTo_ToEmail:
            message = qsTr("ResStr_DocScan_Saving_pic_TIFF")
            break
        case DeviceStruct.CMD_QuickScan_ToApplication:
        case DeviceStruct.CMD_ScanTo_ToApplication:
            break

        case DeviceStruct.CMD_DecodeScan:
        case DeviceStruct.CMD_SeperationScan:
            message = qsTr("ResStr_DocScan_Decoding")
            break
        default:
            return
        }
        if(dialog && dialog.visible)
            dialog.close()
        dialog = openDialog("component/JKMessageBox_refresh.qml" ,{"message.text":message} ,function(dialog){
            dialog.cancelClick.connect(jkInterface.cancelImageHandle)
        })
    }

    function endPhase(cmd ,result ,data){
        console.log("end phase:" ,cmd)

        switch(cmd){
        case DeviceStruct.CMD_getPowerSupply:
            break
        case DeviceStruct.CMD_getIpv4:
        case DeviceStruct.CMD_getSoftap:
        case DeviceStruct.CMD_getWifiInfo:
        case DeviceStruct.CMD_getDeviceSetting:
        case DeviceStruct.CMD_getOffTime:
        case DeviceStruct.CMD_getSaveTime:
            dialog.close()
            break

        case DeviceStruct.CMD_setSoftap:
        case DeviceStruct.CMD_setIpv4:
        case DeviceStruct.CMD_setWifi:
            dialog.close()
            switch(result){
            case DeviceStruct.ERR_ACK:
                information_1button(qsTr("ResStr_Setting_Successfully_"))
                break
            case DeviceStruct.ERR_wifi_have_not_been_inited:
                errorWithImage(qsTr("Wi-Fi not enabled ,please enable first"))
                break
            default:
                errorWithImage(qsTr("ResStr_Setting_Fail"))
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
            dialog.close()
            switch(result){
            case DeviceStruct.ERR_ACK:
                information_1button(qsTr("ResStr_Setting_Successfully_"))
                break
            default:
                errorWithImage(qsTr("ResStr_Setting_Fail"))
                break
            }
            break

        case DeviceStruct.CMD_QuickScan_ToPrint:
        case DeviceStruct.CMD_ScanTo_ToPrint:
            dialog.close()
            switch(result){
            case JKEnums.ImageCommandResult_error_invalidPrinter:
                warningWithImage(qsTr("ResStr_Not_Find_Printer"))
                break
            case DeviceStruct.ERR_ACK:
            case JKEnums.ImageCommandResult_NoError:
                if(DeviceStruct.CMD_ScanTo_ToPrint === cmd)
                    break
                if(jkImageModel.count > 0){
                    dialog = openPreviewDialog(function(dialog){
                        dialog.title = qsTr("ResStr_Quick_Preview")
                        dialog.showExtra = true
                        dialog.index = 0
                        dialog.save = false
                        dialog.name = qsTr("ResStr_DocScan_print_ok")
                        dialog.requestImage(0 ,0)
                    })
                }
                break;
            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
                break
            }
            break
        case DeviceStruct.CMD_QuickScan_ToFile:
        case DeviceStruct.CMD_ScanTo_ToFile:
            dialog.close()
            switch(result){
            case JKEnums.ImageCommandResult_error_saveFile:
                warningWithImage(qsTr("ResStr_DocScan_Fail_save"))
                break
            case JKEnums.ImageCommandResult_error_invalidFilePath:
                warningWithImage(qsTr("ResStr_Specify_File_Path_not_exist"))
                break
            case DeviceStruct.ERR_ACK:
            case JKEnums.ImageCommandResult_NoError:
                if(DeviceStruct.CMD_ScanTo_ToFile === cmd){
                    break
                }
                if(jkImageModel.count > 0){
                    dialog = openPreviewDialog(function(dialog){
                        dialog.title = qsTr("ResStr_Quick_Preview")
                        dialog.showExtra = true
                        dialog.index = 0
                        dialog.save = false
                        dialog.name = qsTr("ResStr_DocScan_save_file_ok")
                        dialog.requestImage(0 ,0)
                    })
                }
                break
            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
                break
            }
            break
        case DeviceStruct.CMD_QuickScan_ToEmail:
            dialog.close()
            switch(result){
            case DeviceStruct.ERR_ACK:
            case JKEnums.ImageCommandResult_NoError:
                if(jkImageModel.count > 0){
                    dialog = openPreviewDialog(function(dialog){
                        dialog.title = qsTr("ResStr_Quick_Preview")
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
                    errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
                break
            }
            break
        case DeviceStruct.CMD_QuickScan_ToFTP:
        case DeviceStruct.CMD_ScanTo_ToFTP:
            dialog.close()
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
                    information_1button(qsTr("ResStr_DocScan_upload_ok"))
                    break
                }
                if(jkImageModel.count > 0){
                    dialog = openPreviewDialog(function(dialog){
                        dialog.title = qsTr("ResStr_Quick_Preview")
                        dialog.showExtra = true
                        dialog.index = 0
                        dialog.save = false
                        dialog.name = qsTr("ResStr_DocScan_upload_ok")
                        dialog.requestImage(0 ,0)
                    })
                }
                break
            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
                break
            }
            break
        case DeviceStruct.CMD_ScanTo_ToCloud:
        case DeviceStruct.CMD_QuickScan_ToCloud:
            dialog.close()
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
                    information_1button(qsTr("ResStr_DocScan_upload_ok"))
                    break
                }
                if(jkImageModel.count > 0){
                    dialog = openPreviewDialog(function(dialog){
                        dialog.title = qsTr("ResStr_Quick_Preview")
                        dialog.showExtra = true
                        dialog.index = 0
                        dialog.save = false
                        dialog.name = qsTr("ResStr_DocScan_upload_ok")
                        dialog.requestImage(0 ,0)
                    })
                }
                break
            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
                break
            }
            break
        case DeviceStruct.CMD_doCalibration:
            dialog.close()
            switch(result){
            case DeviceStruct.ERR_ACK:
                information_1button(qsTr("ResStr_DoCalibration_Completed"))
                break

            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("ResStr_DocScan_calibration_fail"))
                break
            }
            break
        case DeviceStruct.CMD_ScanTo://handler in ScanPage.qml too
            dialog.close()
            switch(result){
            case DeviceStruct.ERR_ACK:
                if(jkImageModel.count > 0)
                    stackview.push("ScanToPage.qml")
                break;
            case DeviceStruct.ERR_SCAN_CANCEL:
                jkImageModel.removeAll()
                break;
            default:
                if(!scanResult(cmd ,result ,data))
                    errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
                break
            }
            break
        case DeviceStruct.CMD_ScanTo_ToEmail:
        case DeviceStruct.CMD_QuickScan_ToApplication:
        case DeviceStruct.CMD_ScanTo_ToApplication:
        case DeviceStruct.CMD_DecodeScan:
        case DeviceStruct.CMD_SeperationScan:
            dialog.close()
            if(!scanResult(cmd ,result ,data))
                errorWithImage(qsTr("ResStr_DocScan_scan_fail"))
            break
        default:
            commonResult(cmd ,data)
            break
        }
    }

    Connections{
        target: jkInterface
        onCmdResultToUi:{
            switch(phase){
            case JKEnums.CommandPhase_start:
                startPhase(cmd ,result ,resultType)
                break

            case JKEnums.CommandPhase_processing:
                processPhase(cmd ,result ,resultType)
                break

            case JKEnums.CommandPhase_complelte:
                endPhase(cmd  ,result ,data ,resultType)
                break
            default:
                break
            }
        }
    }
}
