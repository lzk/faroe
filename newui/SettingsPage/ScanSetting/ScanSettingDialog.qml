import QtQuick 2.0
import "../../component"
import "../../ScanData.js" as JSData

JKParaDialog{
    id: root
    width: 537 + 20
    height: 655 + 20
    toolbar{
        text.text:qsTr("Scan Setting(AC Power)")
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
        comboBox_dpi.model: JSData.constDPI()
        comboBox_mediaType.model: JSData.constMediaType()
        comboBox_scanAreaSize.model: JSData.constPaperSize()
        spin_gamma{
            slider{
                from: 1
                to:50
            }
            text:Math.floor(spin_gamma.value) / 10
        }
    }

    JKTextButton{
        parent: scanSettingView.item_btnDefault
        anchors.fill: parent
        text.text: qsTr("Default")
        onClicked: {
            setting = JSData.defaultScanSetting()
            root.update()
        }
    }

    JKTextButton{
        parent: scanSettingView.item_btnOK
        anchors.fill: parent
        text.text: qsTr("OK")
        onClicked: {
            confirm()
            close()
        }
    }
    Connections{
        target: scanSettingView.radioButton_twoSide
        onCheckedChanged: setting.adfMode = scanSettingView.radioButton_twoSide.checked

    }
    Connections{
        target: scanSettingView.radioButton_color
        onCheckedChanged: setting.colorMode = scanSettingView.radioButton_color.checked

    }
    Connections{
        target: scanSettingView.radioButton_multiFeedDetection_on
        onCheckedChanged: setting.multiFeed = scanSettingView.radioButton_multiFeedDetection_on.checked

    }
    Connections{
        target: scanSettingView.radioButton_autoCropDeskew_on
        onCheckedChanged: setting.autoCropAndDeskew = scanSettingView.radioButton_autoCropDeskew_on.checked

    }
    Connections{
        target: scanSettingView.radioButton_autoColorDetection_on
        onCheckedChanged: setting.autoColorDetection = scanSettingView.radioButton_autoColorDetection_on.checked

    }
    Connections{
        target: scanSettingView.radioButton_skipBlankPage_on
        onCheckedChanged: setting.skipBlankPage = scanSettingView.radioButton_skipBlankPage_on.checked
    }
    Connections{
        target: scanSettingView.comboBox_dpi
        onActivated: {
            setting.dpi = index
        }
    }
    Connections{
        target: scanSettingView.comboBox_mediaType
        onActivated: {
            var constMediaTypeMap = JSData.constMediaTypeMap()
            switch(target.currentText){
            case constMediaTypeMap.depositBook:
            case constMediaTypeMap.card:
                openMediaTypePromptDialog()
                break;
            }
            setting.mediaType = index
        }
    }

//    property int lastRes:-1
//    property int lastPaperSize:-1
//    property int lastPaperType: -1
//    property var constPaperSizeMap: JSData.constPaperSizeMap()
    Connections{
        target: scanSettingView.comboBox_scanAreaSize
        onActivated:{
//            if(index === lastPaperSize)
//                return

//            var dpimodel = JSData.constDPI()
//            var mediamodel = JSData.constMediaType()
//            if(target.currentText === constPaperSizeMap.longPage){

//                dpimodel.pop()
//                scanSettingView.comboBox_dpi.model = dpimodel
//                lastRes = scanSettingView.comboBox_dpi.currentIndex
//                scanSettingView.comboBox_dpi.currentIndex = 1
//                scanSettingView.comboBox_dpi.enabled = false

//                mediamodel.pop()
//                mediamodel.pop()
//                scanSettingView.comboBox_mediaType.model = mediamodel

//                scanSettingView.radioButton_multiFeedDetection_on.checked = true
//                scanSettingView.item_multiFeedDetection.enabled = false

//                scanSettingView.radioButton_autoCropDeskew_on.checked = false
//                scanSettingView.item_autoCropDeskew.enabled = false

//                scanSettingView.radioButton_skipBlankPage_on.checked = false
//                scanSettingView.item_skipBlankPage.enabled = false

//                scanSettingView.radioButton_autoColorDetection_on.checked = true
//                scanSettingView.item_autoColorDetection.enabled = false

//            }else{
//                scanSettingView.comboBox_dpi.model = dpimodel
//                scanSettingView.comboBox_dpi.currentIndex = lastRes
//                scanSettingView.comboBox_dpi.enabled = true

//                scanSettingView.comboBox_mediaType.model = mediamodel

//                if(target.currentText === constPaperSizeMap.A6
//                        || target.currentText === constPaperSizeMap.autoNoMultiFeed){
//                    scanSettingView.radioButton_multiFeedDetection_on.checked = false
//                    scanSettingView.item_multiFeedDetection.enabled = false
//                }else{
//                    scanSettingView.radioButton_multiFeedDetection_on.checked = setting.multiFeed
//                    scanSettingView.item_multiFeedDetection.enabled = true
//                }

//                if(target.currentText !== constPaperSizeMap.auto
//                        ||target.currentText !== constPaperSizeMap.autoNoMultiFeed){
//                    scanSettingView.radioButton_autoCropDeskew_on.checked = false
//                    scanSettingView.item_autoCropDeskew.enabled = false
//                }else{
//                    scanSettingView.radioButton_autoCropDeskew_on.checked = setting.autoCropAndDeskew
//                    scanSettingView.item_autoCropDeskew.enabled = true
//                }

//                scanSettingView.radioButton_skipBlankPage_on.checked = setting.skipBlankPage
//                scanSettingView.item_skipBlankPage.enabled = true

//                scanSettingView.radioButton_autoColorDetection_on.checked = setting.autoColorDetection
//                scanSettingView.item_autoColorDetection.enabled = true
//            }

//            lastPaperSize = index
        }
    }
    Connections{
        target: scanSettingView.spin_brightness
        onValueChanged: setting.brightness = Math.floor(target.value)
    }
    Connections{
        target: scanSettingView.spin_contrast
        onValueChanged: setting.contrast = Math.floor(target.value)
    }
    Connections{
        target: scanSettingView.spin_gamma
        onValueChanged: setting.gamma = Math.floor(target.value)
    }
    Component.onCompleted: {
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
        ok()
    }

    function update(){
        if(setting.adfMode)
            scanSettingView.radioButton_twoSide.checked = true
        else
            scanSettingView.radioButton_oneSide.checked = true
        if(setting.colorMode)
            scanSettingView.radioButton_color.checked = true
        else
            scanSettingView.radioButton_gray.checked = true
        if(setting.multiFeed)
            scanSettingView.radioButton_multiFeedDetection_on.checked = true
        else
            scanSettingView.radioButton_multiFeedDetection_off.checked = true
        if(setting.autoCropAndDeskew)
            scanSettingView.radioButton_autoCropDeskew_on.checked = true
        else
            scanSettingView.radioButton_autoCropDeskew_off.checked = true
        if(setting.autoColorDetection)
            scanSettingView.radioButton_autoColorDetection_on.checked = true
        else
            scanSettingView.radioButton_autoColorDetection_off.checked = true
        if(setting.skipBlankPage)
            scanSettingView.radioButton_skipBlankPage_on.checked = true
        else
            scanSettingView.radioButton_skipBlankPage_off.checked = true
        scanSettingView.comboBox_dpi.currentIndex = setting.dpi
        scanSettingView.comboBox_mediaType.currentIndex = setting.mediaType
        scanSettingView.comboBox_scanAreaSize.currentIndex = setting.scanAreaSize
        scanSettingView.spin_brightness.value = setting.brightness
        scanSettingView.spin_contrast.value = setting.contrast
        scanSettingView.spin_gamma.value = setting.gamma
    }

}
