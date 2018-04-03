import QtQuick 2.0
import Qt.labs.settings 1.0
import "ScanData.js" as JSData
import "JSApi.js" as JSApi

QtObject {
    id:root    

    property var constQuickScanSids:JSData.constQuickScanSid()
    property var constQuickScanSettings:JSData.constQuickScanSettings()
    property var quickScanSettings:JSData.defaultQuickScanSettings()
    property var scanToParameter:JSData.defaultScanToSetting()
    property var decodeSetting:JSData.defaultDecodeSetting()
    property var separationSetting:JSData.defaultSeparationSetting()
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
        if(!storageSettings.settings)
            return
        var tmp = JSON.parse(storageSettings.settings)
        if(verifyStorageQuickScanSetting(tmp.quickScanSettings)){
            JSApi.deepCopy(tmp.quickScanSettings ,quickScanSettings)
            quickScanSettings = quickScanSettings
//            quickScanSettings = tmp.quickScanSettings
        }
        if(tmp.scanToParameter){
            JSApi.deepCopy(tmp.scanToParameter ,scanToParameter)
            scanToParameter = scanToParameter
//            scanToParameter = tmp.scanToParameter
        }
        if(tmp.decodeSetting){
            JSApi.deepCopy(tmp.decodeSetting ,decodeSetting)
            decodeSetting = decodeSetting
        }
        if(tmp.separationSetting){
            JSApi.deepCopy(tmp.separationSetting ,separationSetting)
            separationSetting = separationSetting
        }
        if(tmp.currentDevice){
            currentDevice = tmp.currentDevice
        }
//        if(tmp.emailSetting)
//            emailSetting.fileType = tmp.emailSetting.fileType
//        if(tmp.cloudSetting)
//            cloudSetting.cloudType = tmp.cloudSetting.cloudType
    }

    Component.onDestruction: {
        var settings = {}
        settings.quickScanSettings = quickScanSettings
        settings.scanToParameter = scanToParameter
        settings.decodeSetting = decodeSetting
        settings.separationSetting = separationSetting
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
            newSetting = JSData.defaultQuickScanSetting_toPrint()
            break
        case "Scan To File":
            newSetting = JSData.defaultQuickScanSetting_toFile()
            break
        case "Scan To Application":
            newSetting = JSData.defaultQuickScanSetting_toApplication()
            break
        case "Scan To Email":
            newSetting = JSData.defaultQuickScanSetting_toEmail()
            break
        case "Scan To FTP":
            newSetting = JSData.defaultQuickScanSetting_toFTP()
            break
        case "Scan To Cloud":
            newSetting = JSData.defaultQuickScanSetting_toCloud()
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

    function findQuickScanSettingViaName(name){
        var index = -1
        for(var i = 0 ;i < quickScanSettings.length ;i++){
            if(quickScanSettings[i].name === name){
                index = i
                break
            }
        }
        if(index === -1)
            return null
        return quickScanSettings[index]
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
