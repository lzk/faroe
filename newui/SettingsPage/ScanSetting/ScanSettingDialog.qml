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
                to:50
            }
            text:Math.floor(spin_gamma.slider.value) / 10
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
            ok()
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
        onActivated: setting.dpi = index
    }
    Connections{
        target: scanSettingView.comboBox_mediaType
        onActivated: setting.mediaType = index
    }
    Connections{
        target: scanSettingView.comboBox_scanAreaSize
        onActivated: setting.scanAreaSize = index
    }
    Connections{
        target: scanSettingView.spin_brightness
        onValueChanged: setting.brightness = target.value
    }
    Connections{
        target: scanSettingView.spin_contrast
        onValueChanged: setting.contrast = target.value
    }
    Connections{
        target: scanSettingView.spin_gamma
        onValueChanged: setting.gamma = target.value
    }
    Component.onCompleted: {
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
