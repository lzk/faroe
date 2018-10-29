.pragma library
function defaultFilePath(){
    return ""
}

function constMediaTypeMap(){
    return {
        "normal":qsTr("ResStr_Type_Normal"),
        "depositBook":qsTr("ResStr_Bankbook"),
        "card":qsTr("ResStr_Card"),
    }
}

function constMediaType(){
    var map = constMediaTypeMap()
    return [
                map.normal,
                map.depositBook,
                map.card,
            ]
}

function constPaperSizeMap(){
    return {
        "auto":qsTr("ResStr_DocScan_ScanSize_Auto"),
        "autoNoMultiFeed":qsTr("ResStr_DocScan_ScanSize_AutoNo"),
        "A4":"A4(210 x 297mm)",
        "A5":"A5(148 x 210mm)",
        "B5":"B5(182 x 257mm)",
        "A6":"A6(105 x 148mm)",
        "Letter":"Letter(8.5 x 11\")",
        "Legal":"Legal(8.5 x 14\")",
        "longPage":qsTr("ResStr_LongPage_"),
    }
}

function constPaperSize(){
    var map = constPaperSizeMap()
    return [
                map.auto,
                map.autoNoMultiFeed,
                map.A4,
                map.A5,
                map.B5,
                map.A6,
                map.Letter,
                map.Legal,
                map.longPage,
            ]
}

function constCloudType(){
    var cloudTypes = supportCloudType()
    return [
                cloudTypes.icloud,
            ]
}

function supportCloudType(){
    return {
        "icloud":"iCloud",
        "dropbox":"DropBox",
        "evernote":"EverNote" ,
        "onedrive":"OneDrive"
    }
}

function constEmailAttachmentFileType(){
    return ["PDF" ,"JPG"]
}

function constScanIds(){
    var ob = {
        "scanTo" : "scanTo",
        "scanToPrint" : "Scan To Print",
        "scanToFile" : "Scan To File" ,
        "scanToApplication" : "Scan To Application" ,
        "scanToEmail" : "Scan To Email" ,
        "scanToFTP" : "Scan To FTP" ,
        "scanToCloud" : "Scan To Cloud" ,
        "decodeScan" : "decodeScan" ,
        "separationScan" : "separationScan" ,
    }
    return ob
}

function defaultScanSetting(){
    var ob = {
        "adfMode" : true ,
        "dpi" : 1 ,
        "colorMode" : true ,
        "scanAreaSize" : 0 ,
        "mediaType" : 0 ,
        "multiFeed" : false ,
        "contrast" : 50 ,
        "brightness" : 50 ,
        "autoCropAndDeskew" : true ,
        "autoColorDetection" : false ,
        "skipBlankPage" : false ,
        "gamma" : 18 ,
    }
    return ob
}

function defaultScanToSetting(){
    var sid = constScanIds()
    return {
        "sid":sid.scanTo,
        "scanSetting" :defaultScanSetting() ,
        "cloudTypeText" :supportCloudType().icloud ,
        "emailAttachmentFileType" :constEmailAttachmentFileType()[0]
    }
}

function constFileDialogSaveFileType(){
    return ["TIF (*.tif)", "PDF (*.pdf)", "JPG (*.jpg)", "BMP (*.bmp)"]
}

function constQrcodeCodeType(){
    return [qsTr("ResStr_QRCode") ,qsTr("ResStr_BarCode"),qsTr("ResStr_All")]
}
function constQrcodeFileType(){
    return ["PDF" ,"TIFF"]
}

function defaultDecodeSetting(){
    var sid = constScanIds()
    var ob = {
        "sid":sid.decodeScan,
        "scanSetting" : defaultScanSetting(),
        "codeType" : 0 ,
        "fileName" : "QRcodeBarcodeResult.html" ,
    }
    return ob
}

function defaultSeparationSetting(){
    var sid = constScanIds()
    var ob = {
        "sid":sid.separationScan,
        "scanSetting" : defaultScanSetting(),
        "fileType" : 0 ,
        "filePath" : defaultFilePath(),
    }
    return ob
}


function constQuickScanSid(){
    var sid = constScanIds()
    return [sid.scanToPrint ,sid.scanToFile ,sid.scanToApplication
            ,sid.scanToEmail ,sid.scanToFTP ,sid.scanToCloud ]
//    return ["Scan To Print" ,"Scan To File" ,"Scan To Application"
//            ,"Scan To Email" ,"Scan To FTP" ,"Scan To Cloud"]
}

function constQuickScanName(){
    return [
                qsTr("ResStr_DocScan_Scan_Print"),
                qsTr("ResStr_DocScan_Scan_File"),
                qsTr("ResStr_DocScan_Scan_App"),
                qsTr("ResStr_DocScan_Scan_Email"),
                qsTr("ResStr_DocScan_Scan_FTP"),
                qsTr("ResStr_DocScan_Scan_Cloud")
            ]
}

function defaultQuickScanSettings(){
    return [
            defaultQuickScanSetting_toPrint(),
            defaultQuickScanSetting_toFile(),
            defaultQuickScanSetting_toApplication(),
            defaultQuickScanSetting_toEmail(),
            defaultQuickScanSetting_toFTP(),
            defaultQuickScanSetting_toCloud(),
            ]
}

function newQuickScanObject() {
    var ob = {
        "sid" : "Scan To Print" ,
        "scanSetting" : defaultScanSetting()
    }
    return ob
}

function defaultQuickScanSetting_toPrint() {
    var ob = newQuickScanObject()
    ob.printerName = ""
    ob.name = qsTr("ResStr_DocScan_Scan_Print")
    return ob
}

function defaultQuickScanSetting_toFile() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To File"
    ob.fileType = 0
    ob.fileName = "ScanPictures"
    ob.filePath = defaultFilePath()
    ob.name = qsTr("ResStr_DocScan_Scan_File")
    return ob
}

function defaultApplicationSettings(){
    var ob = {}
    ob.fileType = 0
    ob.fileName = "Preview.app"
    ob.filePath = "/Applications"
    ob.fullFileName = "/Applications/Preview.app"
    return ob
}

function defaultQuickScanSetting_toApplication() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Application"
    ob.fileType = 0
    ob.fileName = "Preview.app"
    ob.filePath = "/Application"
    ob.fullFileName = "/Applications/Preview.app"
    ob.name = qsTr("ResStr_DocScan_Scan_App")
    return ob
}

function defaultEmailSettings(){
    var ob = {}
    ob.fileType = 0
    ob.recipient = ""
    ob.subject = ""
    return ob
}

function defaultQuickScanSetting_toEmail() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Email"
    ob.fileType = 0
    ob.recipient = ""
    ob.subject = "Scan Pictures"
    ob.name = qsTr("ResStr_DocScan_Scan_Email")
    return ob
}

function defaultFTPSettings(){
    var ob = {}
    ob.serverAddress = "ftp://localhost"
    ob.userName = ""
    ob.password = ""
    ob.targetPath = "/files"
    return ob
}

function defaultQuickScanSetting_toFTP() {
    var ob = defaultFTPSettings()
    ob.sid = "Scan To FTP"
    ob.scanSetting = defaultScanSetting()
    ob.name = qsTr("ResStr_DocScan_Scan_FTP")
    return ob
}

function defaultCloudSettings(){
    var ob = {}
    ob.cloudTypeText = supportCloudType().icloud
    return ob
}

function defaultQuickScanSetting_toCloud() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Cloud"
    ob.cloudTypeText = supportCloudType().icloud
    ob.dropboxAccessToken = ""
    ob.dropboxFilePath = ""
    ob.oneDriveAccessToken = ""
    ob.oneDriveFilePath = ""
    ob.noteAccessToken = ""
    ob.noteTitle = ""
    ob.noteContent = ""
    ob.name = qsTr("ResStr_DocScan_Scan_Cloud")
    return ob
}

