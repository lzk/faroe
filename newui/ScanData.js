.pragma library
function constDPIName() {
    return [
            "150DPI" ,
            "200DPI" ,
            "300DPI" ,
            "600DPI" ,
            ]
}
function constDPI() {
    return [
            "150 x 150dpi",
            "200 x 200dpi",
            "300 x 300dpi",
            "600 x 600dpi",
            ]
}

function constMediaTypeMap(){
    return {
        "normal":qsTr("Normal"),
        "depositBook":qsTr("Deposit Book"),
        "card":qsTr("Card"),
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
        "auto":"Auto",
        "autoNoMultiFeed":"Auto(No Multi-Feed)",
        "A4":"A4(210 x 297mm)",
        "A5":"A5(148 x 210mm)",
        "B5":"B5(182 x 257mm)",
        "A6":"A6(105 x 148mm)",
        "Letter":"Letter(8.5 x 11\")",
        "Legal":"Legal(8.5 x 14\")",
        "longPage":"Long Page",
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

function constColorMode(){
    return [qsTr("Color") ,qsTr("GrayScale")]
}

function constAdfMode(){
    return [qsTr("Two Side") ,qsTr("One Side")]
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
        "qrcodeScan" : "qrcodeScan" ,
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
        "multiFeed" : true ,
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
    return ["QR Code" ,"Bar Code" ,"All"]
}
function constQrcodeFileType(){
    return ["PDF" ,"TIFF"]
}
function defaultQrcodeSetting(){
    var sid = constScanIds()
    var ob = {
        "sid":sid.qrcodeScan,
        "scanSetting" : defaultScanSetting(),
        "codeType" : 0 ,
        "outputResult" : "QRcodeBarcodeResult.html" ,
        "fileType" : 0 ,
        "filePath" : "" ,
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

function constQuickScanSettings(){
    return {
    "Scan To Print" : {"title" : qsTr("Print Setting:") ,"source" : "main_img_print.png"}
    ,"Scan To File" : {"title" : qsTr("File Setting:") ,"source" : "main_img_file.png"}
    ,"Scan To Application" : {"title" : qsTr("Application Setting:") ,"source" : "main_img_apps.png"}
    ,"Scan To Email" : {"title" : qsTr("Email Setting:") ,"source" : "main_img_email.png"}
    ,"Scan To FTP" : {"title" : qsTr("FTP Setting:") ,"source" : "main_img_ftp.png"}
    ,"Scan To Cloud" :{"title" : qsTr("Cloud Setting:") ,"source" : "main_img_cloud.png"}
    }
}

function defaultQuickScanSettings(){
    return [
            newQuickScanToPrintObject(),
            newQuickScanToFileObject(),
            newQuickScanToApplicationObject(),
            newQuickScanToEmailObject(),
            newQuickScanToFTPObject(),
            newQuickScanToCloudObject(),
            ]
}

function newQuickScanObject() {
    var ob = {
        "sid" : "Scan To Print" ,
        "name" : "Scan To Print" ,
        "scanSetting" : defaultScanSetting()
    }
    return ob
}

function newQuickScanToPrintObject() {
    var ob = newQuickScanObject()
    ob.printerName = ""
    return ob
}

function newQuickScanToFileObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To File"
    ob.name = "Scan To File"
    ob.fileType = 0
    ob.fileName = ""
    ob.filePath = ""
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

function newQuickScanToApplicationObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Application"
    ob.name = "Scan To Application"
    ob.fileType = 0
    ob.fileName = "Preview.app"
    ob.filePath = "/Application"
    ob.fullFileName = "/Applications/Preview.app"
    return ob
}

function defaultEmailSettings(){
    var ob = {}
    ob.fileType = 0
    ob.recipient = ""
    ob.subject = ""
    return ob
}

function newQuickScanToEmailObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Email"
    ob.name = "Scan To Email"
    ob.fileType = 0
    ob.recipient = ""
    ob.subject = "Scan Pictures"
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

function newQuickScanToFTPObject() {
    var ob = defaultFTPSettings()
    ob.sid = "Scan To FTP"
    ob.name = "Scan To FTP"
    ob.scanSetting = defaultScanSetting()
    return ob
}

function defaultCloudSettings(){
    var ob = {}
    ob.cloudTypeText = supportCloudType().icloud
    return ob
}

function newQuickScanToCloudObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Cloud"
    ob.name = "Scan To Cloud"
    ob.cloudTypeText = supportCloudType().icloud
    ob.dropboxAccessToken = ""
    ob.dropboxFilePath = ""
    ob.oneDriveAccessToken = ""
    ob.oneDriveFilePath = ""
    ob.noteAccessToken = ""
    ob.noteTitle = ""
    ob.noteContent = ""
    return ob
}

