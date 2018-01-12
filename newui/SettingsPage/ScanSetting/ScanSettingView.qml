import QtQuick 2.0
import QtQuick.Controls 2.2
import "../component"

Item {
    id: item9
    width: 537
    height: 655 - 30
    property alias spin_contrast: spin_contrast
    property alias spin_gamma: spin_gamma
    property alias spin_brightness: spin_brightness
    property alias comboBox_dpi: comboBox_dpi
    property alias comboBox_mediaType: comboBox_mediaType
    property alias comboBox_scanAreaSize: comboBox_scanAreaSize
    property alias radioButton_skipBlankPage_on: radioButton_skipBlankPage_on
    property alias radioButton_skipBlankPage_off: radioButton_skipBlankPage_off
    property alias radioButton_autoColorDetection_on: radioButton_autoColorDetection_on
    property alias radioButton_autoColorDetection_off: radioButton_autoColorDetection_off
    property alias radioButton_multiFeedDetection_on: radioButton_multiFeedDetection_on
    property alias radioButton_multiFeedDetection_off: radioButton_multiFeedDetection_off
    property alias radioButton_autoCropDeskew_on: radioButton_autoCropDeskew_on
    property alias radioButton_autoCropDeskew_off: radioButton_autoCropDeskew_off
    property alias radioButton_gray: radioButton_gray
    property alias radioButton_color: radioButton_color
    property alias item_btnDefault: item_btnDefault
    property alias item_btnOK: item_btnOK
    property alias radioButton_twoSide: radioButton_twoSide
    property alias radioButton_oneSide: radioButton_oneSide

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

            Item {
                id: item2
                width: 240
                height: 90

                ScanSettingBackground{
                    width: 220
                    height: 75
                    text: qsTr("ADF Mode")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    RadioButton {
                        id: radioButton_twoSide
                        x: 65
                        y: 35
                        width: 90
                        height: 20
                        text: qsTr("Two Side")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: - parent.width / 4
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    RadioButton {
                        id: radioButton_oneSide
                        x: 65
                        y: 35
                        width: 90
                        height: 20
                        text: qsTr("One Side")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: parent.width / 4
                        anchors.verticalCenter: parent.verticalCenter
                    }

                }
            }

            Item {
                id: item3
                width: 240
                height: 90
                ScanSettingBackground {
                    width: 220
                    height: 75
                    text: qsTr("Color Mode")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    RadioButton {
                        id: radioButton_color
                        x: 65
                        y: 35
                        width: 90
                        height: 20
                        text: qsTr("color")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: - parent.width / 4
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    RadioButton {
                        id: radioButton_gray
                        x: 66
                        y: 36
                        width: 90
                        height: 20
                        text: qsTr("gray")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: parent.width / 4
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Item {
                id: item4
                width: 240
                height: 385

                Column {
                    id: column
                    spacing: 10
                    anchors.fill: parent

                    ScanSettingBackground {
                        id: scanSettingBackground
                        width: 220
                        height: 80
                        text: qsTr("DPI")
                        anchors.horizontalCenter: parent.horizontalCenter

                        ComboBox {
                            id: comboBox_dpi
                            width: 145
                            height: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    ScanSettingBackground {
                        width: 220
                        height: 80
                        text: qsTr("Media Type")
                        anchors.horizontalCenter: parent.horizontalCenter
                        ComboBox {
                            id: comboBox_mediaType
                            width: 145
                            height: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    ScanSettingBackground {
                        width: 220
                        height: 80
                        text: qsTr("Scan Area Size")
                        anchors.horizontalCenter: parent.horizontalCenter
                        ComboBox {
                            id: comboBox_scanAreaSize
                            width: 145
                            height: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    ScanSettingBackground {
                        width: 220
                        height: 100
                        text: qsTr("Gamma")
                        anchors.horizontalCenter: parent.horizontalCenter
                        SpinView{
                            id: spin_gamma
                            anchors.fill: parent
                            anchors.topMargin: 5
                        }
                    }
                }
            }

            Item {
                id: item5
                width: 240
                height: 385

                Column {
                    id: column1
                    spacing: 4
                    ScanSettingBackground {
                        width: 220
                        height: 110
                        text: qsTr("Constract")
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
                                height: 55
                            }
                        }
                    }

                    ScanSettingBackground {
                        width: 220
                        height: 110
                        text: qsTr("Brightness")
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
                                height: 55
                            }
                        }
                    }

                    ScanSettingBackground {
                        width: 220
                        height: 75
                        text: qsTr("Skip Blank Page")
                        anchors.horizontalCenter: parent.horizontalCenter

                        RadioButton {
                            id: radioButton_skipBlankPage_on
                            x: 65
                            y: 35
                            width: 90
                            height: 20
                            text: qsTr("On")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: - parent.width / 4
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        RadioButton {
                            id: radioButton_skipBlankPage_off
                            x: 66
                            y: 36
                            width: 90
                            height: 20
                            text: qsTr("Off")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: parent.width / 4
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }

                    ScanSettingBackground {
                        width: 220
                        height: 75
                        text: qsTr("Auto Color Detection")
                        anchors.horizontalCenter: parent.horizontalCenter

                        RadioButton {
                            id: radioButton_autoColorDetection_on
                            x: 65
                            y: 35
                            width: 90
                            height: 20
                            text: qsTr("On")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: - parent.width / 4
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                        RadioButton {
                            id: radioButton_autoColorDetection_off
                            x: 66
                            y: 36
                            width: 90
                            height: 20
                            text: qsTr("Off")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenterOffset: parent.width / 4
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                    anchors.fill: parent
                }
            }

            Item {
                id: item6
                width: 240
                height: 90
                ScanSettingBackground {
                    width: 220
                    height: 75
                    text: qsTr("Multi Feed Detection")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    RadioButton {
                        id: radioButton_multiFeedDetection_on
                        x: 65
                        y: 35
                        width: 90
                        height: 20
                        text: qsTr("On")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: - parent.width / 4
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    RadioButton {
                        id: radioButton_multiFeedDetection_off
                        x: 66
                        y: 36
                        width: 90
                        height: 20
                        text: qsTr("Off")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: parent.width / 4
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            Item {
                id: item7
                width: 240
                height: 90
                ScanSettingBackground {
                    width: 220
                    height: 75
                    text: qsTr("Auto Crop&Deskew")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    RadioButton {
                        id: radioButton_autoCropDeskew_on
                        x: 65
                        y: 35
                        width: 90
                        height: 20
                        text: qsTr("On")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: - parent.width / 4
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    RadioButton {
                        id: radioButton_autoCropDeskew_off
                        x: 66
                        y: 36
                        width: 90
                        height: 20
                        text: qsTr("Off")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenterOffset: parent.width / 4
                        anchors.horizontalCenter: parent.horizontalCenter
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