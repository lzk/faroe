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

function constMediaType(){
    return [
            qsTr("Normal"),
            qsTr("Deposit Book"),
            qsTr("Card"),
            ]
}

function constPaperSize(){
    return [
            "Auto",
                "A4",
                "A5",
                "B5",
                "A6",
                "Letter",
                "Legal",
                "Long Page",
            ]
}

function constColorMode(){
    return [qsTr("Color") ,qsTr("GrayScale")]
}

function constAdfMode(){
    return [qsTr("Two Side") ,qsTr("One Side")]
}

function constCloudType(){
    return ["DropBox" ,"EverNote" ,"OneDrive"]
}

function constEmailAttachmentFileType(){
    return ["PDF" ,"JPG"]
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
        "autoColorDetection" : true ,
        "skipBlankPage" : true ,
        "gamma" : 18 ,
    }
    return ob
}

function defaultScanParameterSetting(){
    return {
        "scanSetting" : defaultScanSetting(),
        "cloudType" : 0,
        "emailAttachmentFileType" : 0,
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
    var ob = {
        "scanSetting" : defaultScanSetting(),
        "codeType" : 0 ,
        "outputResult" : "QRcodeBarcodeResult.html" ,
        "fileType" : 0 ,
        "filePath" : "" ,
    }
    return ob
}

function constQuickScanSid(){
    return ["Scan To Print" ,"Scan To File" ,"Scan To Application"
            ,"Scan To Email" ,"Scan To FTP" ,"Scan To Cloud"]
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

function newQuickScanToApplicationObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Application"
    ob.name = "Scan To Application"
    ob.fileName = ""
    ob.filePath = ""
    return ob
}

function newQuickScanToEmailObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Email"
    ob.name = "Scan To Email"
    ob.fileType = 0
    ob.recipient = ""
    ob.subject = ""
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

function newQuickScanToCloudObject() {
    var ob = newQuickScanObject()
    ob.sid = "Scan To Cloud"
    ob.name = "Scan To Cloud"
    ob.cloudType = 0
    ob.dropboxAccessToken = ""
    ob.dropboxFilePath = ""
    ob.oneDriveAccessToken = ""
    ob.oneDriveFilePath = ""
    ob.noteAccessToken = ""
    ob.noteTitle = ""
    ob.noteContent = ""
    return ob
}

