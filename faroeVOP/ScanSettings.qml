import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1

Item{
    id: root
    anchors.fill: parent
    signal ok

    Rectangle{
        anchors.fill:  parent

        Button {
            id: button
            text: qsTr("Default")
            anchors.bottomMargin: 10
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: -100
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Button {
            id: button1
            text: qsTr("OK")
            anchors.bottomMargin: 10
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: 100
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }


    Rectangle{
        id:rect_adfmode
        height: 80
        color: "white"
        border.color: "black"
        anchors.leftMargin: 20
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: parent.top

        Text {
            id: text1
            text: qsTr("ADF Mode")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }

        RowLayout {
            id: row
            width: 300
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: text1.bottom
            anchors.topMargin: 10

            RadioButton {
                id: radioButton
                text: qsTr("Two Side")
                onToggled: jkInterface.ADFMode = checked
                checked: jkInterface.ADFMode
            }

            RadioButton {
                id: radioButton1
                text: qsTr("One Side")
                anchors.right: parent.right
                onToggled: jkInterface.ADFMode = !checked
            }
        }
    }

    Rectangle{
        id: rect_dpi
        border.color: "black"
        height: 80
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: rect_adfmode.bottom

        Text {
            id: text2
            text: qsTr("DPI")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }

        ComboBox {
            id: comboBox
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: text2.bottom
            model: ["200*200" ,"300*300" ,"600*600"]
            currentIndex: jkInterface.dpi
            onActivated: jkInterface.dpi = index
        }
    }

    Rectangle{
        id: rect_colormode
        border.color: "black"
        height: 80
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: rect_dpi.bottom

        Text {
            id: text3
            text: qsTr("Color Mode")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }

        Rectangle{
            id: rect_color
            width: 60
            height: 40
            radius:2
            color: jkInterface.colorMode===1 ?"white":"gray"
            anchors.horizontalCenterOffset: -50
            anchors.top: text3.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            Image{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "Images/Color.png"
            }
            MouseArea{
                anchors.fill: parent
                onClicked: jkInterface.colorMode=0
            }
        }
        Rectangle{
            id: rect_gray
            width: 60
            height: 40
            radius: 2
            color: jkInterface.colorMode===0 ?"white":"gray"
            anchors.horizontalCenterOffset: 50
            anchors.top: text3.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            Image{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: "Images/Grayscale.png"
            }
            MouseArea{
                anchors.fill: parent
                onClicked: jkInterface.colorMode=1
            }
        }
    }

    Rectangle{
        id: rect_scanarea
        border.color: "black"
        height: 80
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: rect_colormode.bottom

        Text {
            id: text4
            text: qsTr("Scan Area")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }

        ComboBox {
            id: comboBox1
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: text4.bottom
            model: ["Auto" ,"A4(210*297mm)" ,"Letter(8.5*11)"]
            currentIndex: jkInterface.scanAreaSize
            onActivated: jkInterface.dpi = index
        }
    }

    Rectangle{
        id:rect_multifeed
        height: 80
        color: "white"
        border.color: "black"
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: rect_scanarea.bottom

        Text {
            id: text5
            text: qsTr("MultiFeed")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }

        RowLayout {
            id: row1
            width: 160
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: text5.bottom
            anchors.topMargin: 10

            RadioButton {
                id: radioButton2
                text: qsTr("On")
                onToggled: jkInterface.MultiFeed = checked
                checked: jkInterface.MultiFeed
            }

            RadioButton {
                id: radioButton3
                text: qsTr("Off")
                anchors.right: parent.right
                onToggled: jkInterface.MultiFeed = !checked
            }
        }
    }

    Rectangle{
        id:rect_autoCrop
        height: 80
        color: "white"
        border.color: "black"
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: rect_scanarea.bottom

        Text {
            id: text6
            text: qsTr("Auto Crop&Deskew")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }

        RowLayout {
            id: row2
            width: 160
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: text6.bottom
            anchors.topMargin: 10

            RadioButton {
                id: radioButton4
                text: qsTr("On")
                onToggled: jkInterface.AutoCropAndDeskew = checked
                checked: jkInterface.AutoCropAndDeskew
            }

            RadioButton {
                id: radioButton5
                text: qsTr("Off")
                anchors.right: parent.right
                onToggled: jkInterface.AutoCropAndDeskew = !checked
            }
        }
    }

    Rectangle{
        id: rect_constract
        border.color: "black"
        height: 125
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: rect_adfmode.bottom

        Text {
            id: text7
            text: qsTr("Constrct")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }
    }

    Rectangle{
        id: rect_brightness
        border.color: "black"
        height: 125
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: rect_constract.bottom

        Text {
            id: text8
            text: qsTr("Brightness")
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 16
        }
    }

    Connections {
        target: button
        onClicked: jkInterface.setDefault()
    }

    Connections {
        target: button1
        onClicked: {
            jkInterface.setOK()
            ok()
        }
    }

}
