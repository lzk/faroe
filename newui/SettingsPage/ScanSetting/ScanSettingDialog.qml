import QtQuick 2.0
import "../../component"
import "../../ScanData.js" as JSData
import "../../JSApi.js" as JSApi
import com.liteon.JKInterface 1.0

JKDialog{
    id: root
    width: 537 + 20
    height: 655 + 20

    property var constPaperSize: JSData.constPaperSize()
    property var constPaperSizeMap: JSData.constPaperSizeMap()
    property var constPaperSize_noLongPage: JSData.constPaperSize().slice(0 ,-1)
    property var constPaperSize_onlyAuto: JSData.constPaperSize().slice(0 ,2)
    property var constDPIName: JSData.constDPIName()
    property var constDPI: JSData.constDPI()
    property var constDPI_lessThan300: JSData.constDPI().slice(0 ,-1)
    property var constMediaType: JSData.constMediaType()
    property var constMediaTypeMap: JSData.constMediaTypeMap()
    property var constMediaType_onlyNormal: JSData.constMediaType().slice(0 ,-2)

    property int powerMode: JKEnums.PowerMode_ACPower
    property var scanSetting:JSData.defaultScanSetting()
    property var setting:JSData.defaultScanSetting()

    toolbar{
        text.text:{
            if(scanData.deviceStatus){
                    switch(powerMode){
                        case JKEnums.PowerMode_ACPower:qsTr("Scan Settings(AC Power)");break
                        case JKEnums.PowerMode_usbBusPower:qsTr("Scan Settings(USB Bus Power)");break
                        case JKEnums.PowerMode_PowerBank:qsTr("Scan Settings(Power Bank)");break
                        default: qsTr("Scan Settings");break
                    }
            }else
                qsTr("Scan Settings")
}
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
    }

    Image{
        parent: background
        anchors.fill: parent
        source: "qrc:/Images/popup_gnd_scan settings.png"
    }


    ScanSettingView{
        id:scanSettingView
        parent: container
        anchors.fill: parent
        enabled: !refresh.visible
        item_adfMode.enabled: powerMode !== JKEnums.PowerMode_usbBusPower
        item_colorMode.enabled: !radioButton_autoColorDetection_on.checked
        item_skipBlankPage.enabled: comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                                    && radioButton_autoCropDeskew_on.checked
        item_autoColorDetection.enabled: comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
        item_multiFeedDetection.enabled: comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                                         && comboBox_scanAreaSize.currentText !== constPaperSizeMap.autoNoMultiFeed
                                         && comboBox_scanAreaSize.currentText !== constPaperSizeMap.A6
        item_autoCropDeskew.enabled: comboBox_scanAreaSize.currentIndex < 2
                                     && !radioButton_skipBlankPage_on.checked

        spin_gamma{
            slider{
                from: 1
                to:50
            }
            text:Math.floor(spin_gamma.value) / 10
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
            comboBox_mediaType.model = comboBox_scanAreaSize.currentText === constPaperSizeMap.longPage
                                      ? constMediaType_onlyNormal
                                      : constMediaType
            comboBox_mediaType.currentIndex = index
        }
        function updateComboxModel_scanAreaSize(){
            var index = comboBox_scanAreaSize.currentIndex
            comboBox_scanAreaSize.model= (comboBox_mediaType.currentText === constMediaTypeMap.normal
                                          &&!radioButton_autoCropDeskew_on.checked
                                          )?
                constDPIName[comboBox_dpi.currentIndex] === "600DPI" ?constPaperSize_noLongPage :constPaperSize
                :constPaperSize_onlyAuto
            comboBox_scanAreaSize.currentIndex = index
        }
        property bool lastMultiFeed
        function updateRadio_multiFeed(){
            if(comboBox_scanAreaSize.currentText !== constPaperSizeMap.longPage
                    && comboBox_scanAreaSize.currentText !== constPaperSizeMap.autoNoMultiFeed
                    && comboBox_scanAreaSize.currentText !== constPaperSizeMap.A6){
                lastMultiFeed = radioButton_multiFeedDetection_on.checked
            }else{
                radioButton_multiFeedDetection_on.checked = false
            }
        }
    }

    JKTextButton{
        parent: scanSettingView.item_btnDefault
        anchors.fill: parent
        text.text: qsTr("Default")
        onClicked: {
            scanSetting = JSData.defaultScanSetting()
            root.update()
        }
    }

    JKTextButton{
        parent: scanSettingView.item_btnOK
        anchors.fill: parent
        text.text: qsTr("OK")
        onClicked: {
            confirm()
        }
    }

    Connections{
        target: scanSettingView.radioButton_autoCropDeskew_on
        onCheckedChanged: {
            scanSettingView.updateComboxModel_scanAreaSize()
            scanSettingView.updateRadio_multiFeed()
        }

    }
    Connections{
        target: scanSettingView.comboBox_dpi
        onActivated: {
            scanSettingView.updateComboxModel_scanAreaSize()
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
        }
    }

    Connections{
        target: scanSettingView.comboBox_scanAreaSize
        onActivated:{
            scanSettingView.updateComboxModel_dpi()
            scanSettingView.updateComboxModel_mediaType()
            scanSettingView.updateRadio_multiFeed()
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
        switch(powerMode){
        case JKEnums.PowerMode_usbBusPower:
//            scanSetting.multiFeed = false
            scanSetting.adfMode = false
//            scanSetting.autoCropAndDeskew = false
            scanSetting.mediaType = 0
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

        scanSettingView.updateComboxModel_scanAreaSize()
        scanSettingView.updateComboxModel_dpi()
        scanSettingView.updateComboxModel_mediaType()

        scanSettingView.radioButton_twoSide.checked = scanSetting.adfMode
        scanSettingView.radioButton_color.checked = scanSetting.colorMode
        scanSettingView.radioButton_multiFeedDetection_on.checked = scanSetting.multiFeed
        scanSettingView.radioButton_autoCropDeskew_on.checked = scanSetting.autoCropAndDeskew
        scanSettingView.radioButton_autoColorDetection_on.checked = scanSetting.autoColorDetection
        scanSettingView.radioButton_skipBlankPage_on.checked = scanSetting.skipBlankPage

        scanSettingView.comboBox_dpi.currentIndex = scanSetting.dpi
        scanSettingView.comboBox_mediaType.currentIndex = scanSetting.mediaType
        scanSettingView.comboBox_scanAreaSize.currentIndex = scanSetting.scanAreaSize
        scanSettingView.spin_brightness.value = scanSetting.brightness
        scanSettingView.spin_contrast.value = scanSetting.contrast
        scanSettingView.spin_gamma.value = scanSetting.gamma

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
        onCmdResult:{
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
