import QtQuick 2.0
import QtQuick.Controls 2.2
import "../component"
import "../../component"

Item {
    id: item9
    width: 537
    height: 638 - 30

    property bool decodeMode: false

    property alias spin_contrast: spin_contrast
    property alias spin_gamma: spin_gamma
    property alias spin_brightness: spin_brightness

    property alias comboBox_dpi: comboBox_dpi
    property alias comboBox_mediaType: comboBox_mediaType
    property alias comboBox_scanAreaSize: comboBox_scanAreaSize

    property alias item_btnDefault: item_btnDefault
    property alias item_btnOK: item_btnOK

    property alias item_adfMode: item_adfMode
    property alias radioButton_twoSide: radioButton_twoSide
    //    property alias radioButton_oneSide: radioButton_oneSide

    property alias item_colorMode: item_colorMode
    //    property alias radioButton_gray: radioButton_gray
    property alias radioButton_color: radioButton_color

    property alias item_multiFeedDetection: item_multiFeedDetection
    property alias radioButton_multiFeedDetection_on: radioButton_multiFeedDetection_on
    //    property alias radioButton_multiFeedDetection_off: radioButton_multiFeedDetection_off

    property alias item_autoCropDeskew: item_autoCropDeskew
    property alias radioButton_autoCropDeskew_on: radioButton_autoCropDeskew_on
    //    property alias radioButton_autoCropDeskew_off: radioButton_autoCropDeskew_off

    //    property alias radioButton_skipBlankPage_off: radioButton_skipBlankPage_off

    property alias item_autoColorDetection: item_autoColorDetection
    property alias radioButton_autoColorDetection_on: radioButton_autoColorDetection_on
    //    property alias radioButton_autoColorDetection_off: radioButton_autoColorDetection_off

    Item {
        id: item1
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.fill: parent

        Grid {
            id: grid
            anchors.bottomMargin: 60
            columns: 2
            anchors.fill: parent

            Column {
                id: column
                spacing: 15
                width: 240
                height: parent.height

                ScanSettingBackground{
                    id:item_adfMode
                    width: 220
                    height: 75
                    visible: !decodeMode
                    text: qsTr("ResStr_DocScan_ADF")
                    anchors.horizontalCenter: parent.horizontalCenter

                    Row{
                        anchors.centerIn: parent
                        spacing: 10
                        JKRadioButton {
                            id: radioButton_twoSide
                            width: 90
                            height: 20
                            text: qsTr("ResStr_DocScan_2side")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        JKRadioButton {
                            id: radioButton_oneSide
                            width: 90
                            height: 20
                            text: qsTr("ResStr_DocScan_1side")
                            font.pointSize: 14
                            checked: !radioButton_twoSide.checked
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                ScanSettingBackground {
                    id: scanSettingBackground
                    width: 220
                    height: 80
                    visible: !decodeMode
                    text: qsTr("ResStr_DPI")
                    anchors.horizontalCenter: parent.horizontalCenter

                    JKComboBox {
                        id: comboBox_dpi
                        width: 180
                        height: 24
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                ScanSettingBackground {
                    width: 220
                    height: 80
                    text: qsTr("ResStr_Type")
                    parent: decodeMode ?column1 :column
                    anchors.horizontalCenter: parent.horizontalCenter
                    JKComboBox {
                        id: comboBox_mediaType
                        width: 180
                        height: 24
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                ScanSettingBackground {
                    width: 220
                    height: 80
                    text: qsTr("ResStr_Scan_Size")
                    anchors.horizontalCenter: parent.horizontalCenter
                    JKComboBox {
                        id: comboBox_scanAreaSize
                        width: 180
                        height: 24
                        font.pointSize: 13
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                ScanSettingBackground {
                    id: item_autoColorDetection
                    width: 220
                    height: 75
                    visible: !decodeMode
                    text: qsTr("ResStr_Auto_Color_Detect")
                    anchors.horizontalCenter: parent.horizontalCenter

                    Row{
                        anchors.centerIn: parent
                        spacing: 10
                        JKRadioButton {
                            id: radioButton_autoColorDetection_on
                            width: 90
                            height: 20
                            text: qsTr("ResStr_On")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        JKRadioButton {
                            id: radioButton_autoColorDetection_off
                            width: 90
                            height: 20
                            text: qsTr("ResStr_Off")
                            checked: !radioButton_autoColorDetection_on.checked
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                ScanSettingBackground {
                    id: item_autoCropDeskew
                    width: 220
                    height: 75
                    visible: false
                    text: qsTr("ResStr_DocScan_autocrop")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    Row{
                        anchors.centerIn: parent
                        spacing: 10
                        JKRadioButton {
                            id: radioButton_autoCropDeskew_on
                            width: 90
                            height: 20
                            text: qsTr("ResStr_On")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        JKRadioButton {
                            id: radioButton_autoCropDeskew_off
                            width: 90
                            height: 20
                            text: qsTr("ResStr_Off")
                            checked: !radioButton_autoCropDeskew_on.checked
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                ScanSettingBackground {
                    id: item_multiFeedDetection
                    width: 220
                    height: 75
                    text: qsTr("ResStr_DocScan_multifeed")
                    Row {
                        spacing: 10
                        JKRadioButton {
                            id: radioButton_multiFeedDetection_on
                            width: 90
                            height: 20
                            text: qsTr("ResStr_On")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        JKRadioButton {
                            id: radioButton_multiFeedDetection_off
                            width: 90
                            height: 20
                            text: qsTr("ResStr_Off")
                            anchors.verticalCenter: parent.verticalCenter
                            checked: !radioButton_multiFeedDetection_on.checked
                        }
                        anchors.centerIn: parent
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            Column {
                id: column1
                spacing: 10
                width: 240
                height: parent.height

                ScanSettingBackground {
                    id:item_colorMode
                    width: 220
                    height: 75
                    visible: !decodeMode
                    text: qsTr("ResStr_Color_Mode")
                    anchors.horizontalCenter: parent.horizontalCenter

                    Row{
                        anchors.centerIn: parent
                        spacing: 10
                        JKRadioButton {
                            id: radioButton_color
                            width: 90
                            height: 20
                            indicator.x:10
                            contentItem:  Item{
                                anchors.fill: parent
                                Image{
                                    x:50
                                    width: 30
                                    height: 30
                                    source: "qrc:/Images/Color.png"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        JKRadioButton {
                            id: radioButton_gray
                            width: 90
                            height: 20
                            indicator.x:10
                            checked: !radioButton_color.checked
                            contentItem:  Item{
                                anchors.fill: parent
                                Image{
                                    x:50
                                    width: 30
                                    height: 30
                                    source: "qrc:/Images/Grayscale.png"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                }

                ScanSettingBackground {
                    width: 220
                    height: 130
                    visible: !decodeMode
                    text: qsTr("ResStr_Contrast")
                    anchors.horizontalCenter: parent.horizontalCenter

                    Column {
                        id: column2
                        anchors.fill: parent
                        anchors.topMargin: 5
                        spacing: 5

                        Image {
                            id: image
                            width: 156
                            height: 12
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "qrc:/Images/Constrast.png"
                        }

                        SpinView{
                            id: spin_contrast
                            width:parent.width
                            height: 65
                            maximumLength: 3
                        }
                    }
                }

                ScanSettingBackground {
                    width: 220
                    height: 130
                    visible: !decodeMode
                    text: qsTr("ResStr_Brightness")
                    anchors.horizontalCenter: parent.horizontalCenter
                    Column {
                        id: column3
                        anchors.fill: parent
                        anchors.topMargin: 5
                        spacing: 5

                        Image {
                            id: image1
                            width: 156
                            height: 12
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "qrc:/Images/Brightness.png"
                        }

                        SpinView{
                            id: spin_brightness
                            width:parent.width
                            height: 65
                            maximumLength: 3
                        }
                    }
                }

                ScanSettingBackground {
                    width: 220
                    height: 110
                    visible: !decodeMode
                    text: qsTr("ResStr_Gamma")
                    anchors.horizontalCenter: parent.horizontalCenter
                    SpinView{
                        id: spin_gamma
                        anchors.fill: parent
                        anchors.topMargin: 5
                    }
                }
            }
        }
    }

    Item {
        id: item8
        width: 320
        height: 60
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        Row {
            id: row
            spacing: 20
            anchors.fill: parent

            Item {
                id: item_btnDefault
                width: 150
                height: 35
                anchors.verticalCenter: parent.verticalCenter
            }

            Item {
                id: item_btnOK
                width: 150
                height: 35
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }



}
