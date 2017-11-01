import QtQuick 2.4
import QtQuick.Controls 2.2


import QtQuick.Controls 2.2

Item {
    id: root
    property alias buttonCancel: buttonCancel
    property alias progress: progressBar.value

    JKProgressBar {
        id: progressBar
        height: 30
        anchors.rightMargin: 100
        anchors.leftMargin: 100
        value: 0.5
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.left: parent.left
    }

    JKImageButton {
        id: buttonCancel
        width: 30
        height: 30
        anchors.top: progressBar.bottom
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        source: "Images/cancelIcon.png"
    }

    Text {
        id: text1
        x: 309
        y: 129
        text: qsTr("Scanning")
        anchors.bottom: progressBar.top
        anchors.bottomMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 30
    }

    //    ProgressBar {
    //        id: control
    //        anchors.horizontalCenter: parent.horizontalCenter
    //        anchors.top: buttonCancel.bottom
    //        anchors.topMargin: 20
    //        value: 0.8
    //        padding:2
    //        background: Rectangle {
    //            implicitWidth: 200
    //            implicitHeight: 30
    //            color: "darkgray"
    //            radius: height / 2
    //        }

    //        contentItem: Item {
    //            implicitWidth: 200
    //            implicitHeight: 28

    //            Rectangle {
    //                width: control.visualPosition * parent.width
    //                height: parent.height
    //                radius: height / 2
    //                gradient: Gradient {
    //                    GradientStop {position: 0.0 ;color: "#ffffff"}
    //                    GradientStop {position: 0.1 ;color: "white"}
    //                    GradientStop {position: 1.0 ;color: "green"}
    //                }
    //            }
    //        }
    //    }
}

