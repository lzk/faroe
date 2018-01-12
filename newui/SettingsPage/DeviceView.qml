import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import "component"
import "../component"

Item {
    id: item1
    width: 495
    height: 460

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.bottomMargin: 10
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        Item {
            id: item3
            height: 50
            width: parent.width


            Item {
                id: item_btnCalibration
                width: 100
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10
            }
        }

        DividingLine{
            height: 6
            width: parent.width
        }

        Item {
            id: item2
            height: 135
            width: parent.width

            GroupBox {
                id: groupBox
                width: 400
                height: 115
                font.bold: true
                font.pixelSize: 12
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                title: qsTr("Auto Sleep Time")

                Column {
                    width: 360
                    height: 80
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Item {
                        id: item4
                        width: parent.width
                        height: 45
                        SpinView{
                            anchors.centerIn: parent
                            slider{
                                visible: false
                                to: 60
                                value: 0
                            }
                        }
                    }
                    JKText {
                        id: text3
                        text: qsTr("0-60 Minutes")
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 14
                    }
                }
            }
        }        

        Item {
            id: item5
            height: 135
            width: parent.width

            GroupBox {
                id: groupBox1
                width: 400
                height: 115
                font.bold: true
                font.pixelSize: 12
                spacing: 0
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                title: qsTr("Auto Off Time")

                Column {
                    width: 360
                    height: 80
                    anchors.left: parent.left
                    anchors.leftMargin: 20

                    Item {
                        id: item6
                        width: parent.width
                        height: 45
                        SpinView{
                            anchors.centerIn: parent
                            slider{
                                visible: false
                                to: 4
                                value: 0
                            }
                        }
                    }
                    JKText {
                        id: text4
                        text: qsTr("0-4 Hours")
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 14
                    }
                }
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Item{
            Layout.fillWidth: true
            height: 60
            JKTextButton {
                id: button_apply
                text.text: qsTr("Apply")
                width: 150
                height: 35
                anchors.centerIn: parent
            }
        }

    }

    property var qrcodeSetting
    Component.onCompleted: {
    }

    JKTextButton{
        parent: item_btnCalibration
        anchors.fill: parent
        text.text: qsTr("Calibration")
        onClicked: {
        }
    }
    Connections{
        target: button_apply
        onClicked: {
        }
    }

}
