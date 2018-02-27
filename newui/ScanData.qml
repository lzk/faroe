import QtQuick 2.0
import Qt.labs.settings 1.0
import "ScanData.js" as JSData

QtObject {
    id:root    

    property var constQuickScanSids:JSData.constQuickScanSid()
    property var constQuickScanSettings:JSData.constQuickScanSettings()
    property var quickScanSettings:JSData.defaultQuickScanSettings()
    property var scanToParameter:JSData.defaultScanToSetting()
    property var qrcodeSetting:JSData.defaultQrcodeSetting()
    property var ftpSetting: JSData.defaultFTPSettings()
    property var applicationSetting: JSData.defaultApplicationSettings()
//    property var emailSetting: JSData.defaultEmailSettings()
//    property var cloudSetting: JSData.defaultCloudSettings()
    property var model_deviceList
    property string currentDevice:""
    property bool deviceStatus:false

    property var storageSettings:
    Settings{
        property string settings
    }

    Component.onCompleted: {

        var tmp = JSON.parse(storageSettings.settings)
        if(verifyStorageQuickScanSetting(tmp.quickScanSettings)){
            quickScanSettings = tmp.quickScanSettings
        }
        if(tmp.scanToParameter)
            scanToParameter = tmp.scanToParameter
        if(tmp.qrcodeSetting)
            qrcodeSetting = tmp.qrcodeSetting
        if(tmp.currentDevice)
            currentDevice = tmp.currentDevice
//        if(tmp.emailSetting)
//            emailSetting.fileType = tmp.emailSetting.fileType
//        if(tmp.cloudSetting)
//            cloudSetting.cloudType = tmp.cloudSetting.cloudType
    }

    Component.onDestruction: {
        var settings = {}
        settings.quickScanSettings = quickScanSettings
        settings.scanToParameter = scanToParameter
        settings.qrcodeSetting = qrcodeSetting
        settings.currentDevice = currentDevice
//        settings.emailSetting = emailSetting
//        settings.cloudSetting = cloudSetting
        storageSettings.settings = JSON.stringify(settings)
    }

    function verifyStorageQuickScanSetting(array){
        if(!array)
            return false
        if(array.length < constQuickScanSids.length)
            return false
        for(var i in constQuickScanSids){
            if(!containName(constQuickScanSids[i] ,array))
                return false
        }
        return true
    }

    function containName(name ,array){
        for(var i in array){
            if(array[i].name === name)
                return true
        }
        return false
    }

    function createQuickScanSetting(sid){
        var newSetting = undefined
        switch(sid){
        case "Scan To Print":
            newSetting = JSData.newQuickScanToPrintObject()
            break
        case "Scan To File":
            newSetting = JSData.newQuickScanToFileObject()
            break
        case "Scan To Application":
            newSetting = JSData.newQuickScanToApplicationObject()
            break
        case "Scan To Email":
            newSetting = JSData.newQuickScanToEmailObject()
            break
        case "Scan To FTP":
            newSetting = JSData.newQuickScanToFTPObject()
            break
        case "Scan To Cloud":
            newSetting = JSData.newQuickScanToCloudObject()
            break
        }
        if(newSetting){
            newSetting.sid = sid
        }
        return newSetting
    }

    function addQuickScanSetting(newSetting){
        quickScanSettings.push(newSetting)
    }

    function replaceQuickScanSetting(oldSetting ,newSetting){
        quickScanSettings.splice(quickScanSettings.indexOf(oldSetting) ,1 ,newSetting)
    }

    function verifyQuickScanSettingName(name){
        var ok = true
        for(var i = 0 ;i < quickScanSettings.length ;i++){
            if(quickScanSettings[i].name === name){
                ok = false
                break
            }
        }
        return ok
    }

    function deleteQuickScanSetting(index){
        quickScanSettings.splice(index ,1)
    }

    function upQuickScanSetting(index){
        if(index <= 0)
            return
        var setting = quickScanSettings[index]
        quickScanSettings.splice(index ,1)
        quickScanSettings.splice(index - 1 ,0 ,setting)
    }

    function downQuickScanSetting(index){
        if(index >= quickScanSettings.length)
            return
        var setting = quickScanSettings[index]
        quickScanSettings.splice(index ,1)
        quickScanSettings.splice(index + 1 ,0 ,setting)
    }

    function getQuickScanSetting(index){
        return quickScanSettings[index]
    }

    function getQuickScanSource(sid){
        return constQuickScanSettings[sid].source
    }

    function checkQuickScanSettingreadOnly(setting){
//        return constQuickScanSids.includes(setting.name)
        return setting.name === "Scan To Print"
                || setting.name === "Scan To File"
                || setting.name === "Scan To Application"
                || setting.name === "Scan To Email"
                || setting.name === "Scan To FTP"
                || setting.name === "Scan To Cloud"
    }

    function getQuickScanSettingTitle(setting){
        return  setting !== undefined ?constQuickScanSettings[setting.sid].title :""
    }
}
