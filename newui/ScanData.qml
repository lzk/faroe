import QtQuick 2.0
import "Settings"

QtObject {
    id:root

    property var supportSid: ["Scan To Print" ,"Scan To File" ,"Scan To Application"
                             ,"Scan To Email" ,"Scan To FTP" ,"Scan To Cloud"]

    property var settings:[toPrintSettingLoader.item
        ,toFileSettingLoader.item
        ,toApplicationSettingLoader.item
        ,toEmailSettingLoader.item
        ,toFtpSettingLoader.item
        ,toCloudSettingLoader.item
    ]

    property var toPrintSettingLoader:
    Loader{
        id:toPrintSettingLoader
        sourceComponent: toPrintSettingComponent
    }
    property var toFileSettingLoader:
    Loader{
        id:toFileSettingLoader
        sourceComponent: toFileSettingComponent
    }
    property var toApplicationSettingLoader:
    Loader{
        id:toApplicationSettingLoader
        sourceComponent: toApplicationSettingComponent
    }
    property var toEmailSettingLoader:
    Loader{
        id:toEmailSettingLoader
        sourceComponent: toEmailSettingComponent
    }
    property var toFtpSettingLoader:
    Loader{
        id:toFtpSettingLoader
        sourceComponent: toFtpSettingComponent
    }
    property var toCloudSettingLoader:
    Loader{
        id:toCloudSettingLoader
        sourceComponent: toCloudSettingComponent
    }
    property var toPrintSettingComponent:
    Component{
        id:toPrintSettingComponent
        ScanToSetting{
            sid:"Scan To Print"
            name:"Scan To Print"
            title:"Print Setting:"
            property string printerName
        }
    }
    property var toFileSettingComponent:
    Component{
        id:toFileSettingComponent
        ScanToSetting{
            sid:"Scan To File"
            name:"Scan To File"
            title:"File Setting:"
            property int fileType: 0
            property string fileName
            property string filePath
        }
    }
    property var toApplicationSettingComponent:
    Component{
        id:toApplicationSettingComponent
        ScanToSetting{
            sid:"Scan To Application"
            name:"Scan To Application"
            title:"Application Setting:"
            property string fileName
            property string filePath
        }
    }
    property var toEmailSettingComponent:
    Component{
        id:toEmailSettingComponent
        ScanToSetting{
            sid:"Scan To Email"
            name:"Scan To Email"
            title:"Email Setting:"
            property int fileType: 0
            property string recipient
            property string subject
        }
    }
    property var toFtpSettingComponent:
    Component{
        id:toFtpSettingComponent
        ScanToSetting{
            sid:"Scan To FTP"
            name:"Scan To FTP"
            title:"FTP Setting:"
            property string serverAddress
            property string userName
            property string password
            property string targetPath
        }
    }
    property var toCloudSettingComponent:
    Component{
        id:toCloudSettingComponent
        ScanToSetting{
            sid:"Scan To Cloud"
            name:"Scan To Cloud"
            title:"Cloud Setting:"
            property int cloudType: 0
            property string dropboxAccessToken
            property string dropboxFilePath
            property string oneDriveAccessToken
            property string oneDriveFilePath
            property string noteAccessToken
            property string noteTitle
            property string noteContent
        }
    }

    function create(sid){
        var newSetting = undefined
        switch(sid){
        case "Scan To Print":
            newSetting = toPrintSettingComponent.createObject(root)
            break
        case "Scan To File":
            newSetting = toFileSettingComponent.createObject(root)
            break
        }
        if(newSetting !== undefined){
            newSetting.canEdit = true
            newSetting.sid = sid
        }
        return newSetting
    }


    function add(newSetting){
        settings.push(newSetting)
    }
}
