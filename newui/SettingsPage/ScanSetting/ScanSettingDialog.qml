import QtQuick 2.0
import "../../component"
import "../../ScanData.js" as JSData
import "../../JSApi.js" as JSApi
import com.liteon.JKInterface 1.0

JKDialog{
    id: root
    width: 537 + 20
    height: mode === 1 ? 300:638 + 20

    property int mode: 0

    property var constPaperSize: JSData.constPaperSize()
    property var constPaperSizeMap: JSData.constPaperSizeMap()
    property var constPaperSize_noLongPage: JSData.constPaperSize().slice(0 ,-1)
    property var constPaperSize_onlyAuto: JSData.constPaperSize().slice(0 ,1)
    property var constPaperSize_onlyAutoNoMultiFeed: JSData.constPaperSize().slice(1 ,2)
    property var constMediaType: JSData.constMediaType()
    property var constMediaTypeMap: JSData.constMediaTypeMap()
    property var constMediaType_onlyNormal: JSData.constMediaType().slice(0 ,-2)

    property int powerMode: JKEnums.PowerMode_ACPower
    property var scanSetting:JSData.defaultScanSetting()
    property var setting:JSData.defaultScanSetting()

    property var constDPI:
        [
        "150 x 150dpi",
        "200 x 200dpi",
        "300 x 300dpi",
        "600 x 600dpi",
        ]
    property var constDPI_lessThan300: constDPI.slice(0 ,-1)
    toolbar{
        text.text:{
            if(scanData.deviceStatus){
                    switch(powerMode){
                        case JKEnums.PowerMode_ACPower:qsTr("ResStr_DocScan_scan_setting1");break
                        case JKEnums.PowerMode_usbBusPower:qsTr("ResStr_DocScan_scan_setting2");break
                        case JKEnums.PowerMode_PowerBank:qsTr("ResStr_DocScan_scan_setting3");break
                        default: qsTr("ResStr_DocScan_scan_setting4");break
                    }
            }else
                qsTr("ResStr_DocScan_scan_setting4")
}
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
    }

    Rectangle{
        parent:background
        anchors.fill: parent
        color: "white"
    }

//    Image{
//        parent: background
//        anchors.fill: parent
//        source: "qrc:/Images/popup_gnd_scan settings.png"
//    }


    ScanSettingView{
        id:scanSettingView
        parent: container
        anchors.fill: parent
        enabled: !refresh.visible
        decodeMode: mode === 1
        item_adfMode.enabled: powerMode !== JKEnums.PowerMode_usbBusPower
        item_colorMode.enabled: !radioButton_autoColorDetection_on.checked
        item_skipBlankPage.enabled:
//                                    comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                                    (comboBox_scanAreaSize.currentText === constPaperSizeMap.auto
                                    || comboBox_scanAreaSize.currentText === constPaperSizeMap.autoNoMultiFeed)
//                                    && radioButton_autoCropDeskew_on.checked
                                    && powerMode !== JKEnums.PowerMode_usbBusPower
        item_autoColorDetection.enabled:
//                                      comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                                         (comboBox_scanAreaSize.currentText === constPaperSizeMap.auto
                                         || comboBox_scanAreaSize.currentText === constPaperSizeMap.autoNoMultiFeed)
                                        && radioButton_color.checked
                                        && powerMode !== JKEnums.PowerMode_usbBusPower
                                         && powerMode !== JKEnums.PowerMode_PowerBank
        item_multiFeedDetection.enabled: comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                                         && comboBox_scanAreaSize.currentText !== constPaperSizeMap.autoNoMultiFeed
                                         && comboBox_scanAreaSize.currentText !== constPaperSizeMap.A6
                                         && comboBox_mediaType.currentText === constMediaTypeMap.normal
                                         && powerMode !== JKEnums.PowerMode_usbBusPower
//        item_autoCropDeskew.enabled: comboBox_scanAreaSize.currentIndex < 2
//                                     && !radioButton_skipBlankPage_on.checked
        radioButton_autoCropDeskew_on.checked:
                                        radioButton_skipBlankPage_on.checked
                                        || radioButton_autoColorDetection_on.checked
                                        || item_skipBlankPage.enabled
//                                    (comboBox_scanAreaSize.currentText === constPaperSizeMap.auto
//                                    || comboBox_scanAreaSize.currentText === constPaperSizeMap.autoNoMultiFeed)
//                                    && powerMode !== JKEnums.PowerMode_usbBusPower

        spin_gamma{
            from: 1
            to: 50
            decimals: 1
//            text:Math.floor(spin_gamma.value) / 10
        }
        function updateComboxModel_dpi(){
            var index = comboBox_dpi.currentIndex
            comboBox_dpi.model = comboBox_scanAreaSize.currentText === constPaperSizeMap.longPage
                                ? constDPI_lessThan300
                                : constDPI
            comboBox_dpi.currentIndex = index
        }
        function updateComboxModel_mediaType(){
            var index = comboBox_mediaType.currentIndex
            comboBox_mediaType.model =
                    comboBox_scanAreaSize.currentText === constPaperSizeMap.longPage
//                    (comboBox_scanAreaSize.currentText === constPaperSizeMap.auto
//                    || comboBox_scanAreaSize.currentText === constPaperSizeMap.autoNoMultiFeed)
                    || powerMode === JKEnums.PowerMode_usbBusPower
                    || powerMode === JKEnums.PowerMode_PowerBank
//                    || radioButton_multiFeedDetection_on.checked
                  ? constMediaType_onlyNormal
                  : constMediaType
            comboBox_mediaType.currentIndex = index
        }
        function updateComboxModel_scanAreaSize(){
            var index = 0
            if(comboBox_mediaType.currentText === constMediaTypeMap.normal){
                index = comboBox_scanAreaSize.currentIndex
                comboBox_scanAreaSize.model = (powerMode === JKEnums.PowerMode_usbBusPower
                                                || powerMode === JKEnums.PowerMode_PowerBank
                                               || mode !== 0) ?constPaperSize_noLongPage :constPaperSize
            }else{
                index = 0
                comboBox_scanAreaSize.model = constPaperSize_onlyAuto
            }
            comboBox_scanAreaSize.currentIndex = index
        }
        property bool lastMultiFeed:false
        function updateRadio_multiFeed(){
            if(comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                    && comboBox_scanAreaSize.currentText !== constPaperSizeMap.autoNoMultiFeed
                    && comboBox_scanAreaSize.currentText !== constPaperSizeMap.A6
                    && comboBox_mediaType.currentText === constMediaTypeMap.normal){
                radioButton_multiFeedDetection_on.checked = lastMultiFeed
            }else{
//                if(radioButton_multiFeedDetection_on.checked)
                    lastMultiFeed = radioButton_multiFeedDetection_on.checked
                radioButton_multiFeedDetection_on.checked = false
            }
        }
        property bool lastSkipBlank: false
        function updateRadio_skipBlank(){
            if(comboBox_scanAreaSize.currentText === constPaperSizeMap.auto
              || comboBox_scanAreaSize.currentText === constPaperSizeMap.autoNoMultiFeed
                    ){
                radioButton_skipBlankPage_on.checked = lastSkipBlank
            }else{
//                if(radioButton_skipBlankPage_on.checked)
                    lastSkipBlank = radioButton_skipBlankPage_on.checked
                radioButton_skipBlankPage_on.checked = false
            }
        }
        property bool lastAutoColorDetection: false
        function updateRadio_autoColorDetection(){
            if((comboBox_scanAreaSize.currentText === constPaperSizeMap.auto
              || comboBox_scanAreaSize.currentText === constPaperSizeMap.autoNoMultiFeed)
              && radioButton_color.checked === true
                    ){
                radioButton_autoColorDetection_on.checked = lastAutoColorDetection
            }else{
//                if(radioButton_autoColorDetection_on.checked)
                    lastAutoColorDetection = radioButton_autoColorDetection_on.checked
                radioButton_autoColorDetection_on.checked = false
            }
        }
    }

    JKTextButton{
        parent: scanSettingView.item_btnDefault
        anchors.fill: parent
        text: qsTr("ResStr_Default")
        onClicked: {
            scanSetting = JSData.defaultScanSetting()
            root.update()
        }
    }

    JKTextButton{
        parent: scanSettingView.item_btnOK
        anchors.fill: parent
        text: qsTr("ResStr_OK")
        onClicked: {
            confirm()
        }
    }

//    Connections{
//        target: scanSettingView.radioButton_autoCropDeskew_on
//        onCheckedChanged: {
////            scanSettingView.updateComboxModel_scanAreaSize()
////            scanSettingView.updateRadio_multiFeed()
//        }

//    }
    Connections{
        target: scanSettingView.comboBox_dpi
        onActivated: {
//            scanSettingView.updateComboxModel_scanAreaSize()
        }
    }

    Connections{
        target: scanSettingView.comboBox_mediaType
        onActivated: {
            switch(target.currentText){
            case constMediaTypeMap.depositBook:
            case constMediaTypeMap.card:
                openMediaTypePromptDialog()
                break;
            }
            scanSettingView.updateComboxModel_scanAreaSize()
            scanSettingView.updateRadio_multiFeed()
        }
    }

    Connections{
        target: scanSettingView.comboBox_scanAreaSize
        onActivated:{
            scanSettingView.updateComboxModel_dpi()
            scanSettingView.updateComboxModel_mediaType()
            scanSettingView.updateRadio_multiFeed()
            scanSettingView.updateRadio_skipBlank()
            scanSettingView.updateRadio_autoColorDetection()

        }
    }

    function confirm(){
        setting.adfMode = scanSettingView.radioButton_twoSide.checked
        setting.colorMode = scanSettingView.radioButton_color.checked
        setting.multiFeed = scanSettingView.radioButton_multiFeedDetection_on.checked
        setting.autoCropAndDeskew = scanSettingView.radioButton_autoCropDeskew_on.checked
        setting.autoColorDetection = scanSettingView.radioButton_autoColorDetection_on.checked
        setting.skipBlankPage = scanSettingView.radioButton_skipBlankPage_on.checked
        setting.dpi = scanSettingView.comboBox_dpi.currentIndex
        setting.mediaType = scanSettingView.comboBox_mediaType.currentIndex
        setting.scanAreaSize = scanSettingView.comboBox_scanAreaSize.currentIndex
        setting.brightness = Math.floor(scanSettingView.spin_brightness.value)
        setting.contrast = Math.floor(scanSettingView.spin_contrast.value)
        setting.gamma = Math.floor(scanSettingView.spin_gamma.value)
        close()
    }

    function update(){
        updateScanSetting(scanSetting ,powerMode)

        scanSettingView.radioButton_twoSide.checked = scanSetting.adfMode
        scanSettingView.radioButton_color.checked = scanSetting.colorMode
        scanSettingView.radioButton_multiFeedDetection_on.checked = scanSetting.multiFeed
//        scanSettingView.radioButton_autoCropDeskew_on.checked = scanSetting.autoCropAndDeskew
        scanSettingView.radioButton_autoColorDetection_on.checked = scanSetting.autoColorDetection
        scanSettingView.radioButton_skipBlankPage_on.checked = scanSetting.skipBlankPage

        scanSettingView.comboBox_dpi.currentIndex = scanSetting.dpi
        scanSettingView.comboBox_mediaType.currentIndex = scanSetting.mediaType
        scanSettingView.comboBox_scanAreaSize.currentIndex = scanSetting.scanAreaSize
        scanSettingView.spin_brightness.value = scanSetting.brightness
        scanSettingView.spin_contrast.value = scanSetting.contrast
        scanSettingView.spin_gamma.value = scanSetting.gamma

        scanSettingView.updateComboxModel_scanAreaSize()
        scanSettingView.updateComboxModel_dpi()
        scanSettingView.updateComboxModel_mediaType()
    }

    JKBusyIndicator{
        id:refresh
        visible: false
        width: 50
        height: 50
        parent: container
        anchors.centerIn: parent
        z:1
    }

    Component.onCompleted: {
        JSApi.deepCopy(setting ,scanSetting)
        update()
        jkInterface.setCmd(DeviceStruct.CMD_getPowerSupply)
        refresh.visible = true
    }
    Connections{
        target: jkInterface
        onCmdResultToUi:{
            if(phase !== JKEnums.CommandPhase_complelte)
                return
            refresh.visible = false
            switch(cmd){
            case DeviceStruct.CMD_getPowerSupply:
                if(!result){
                    var setting = JSON.parse(data)
                    powerMode = setting.powerSupply
                }else{
                    powerMode = JKEnums.PowerMode_unknown
                }
                update()
                break
            }
        }
    }
}
