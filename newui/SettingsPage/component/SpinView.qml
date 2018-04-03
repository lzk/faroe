import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    width: 190
    height: 65
    property alias value: slider.value
    property alias slider: slider
    property alias text: text1.text

    Column {
        id: column
        anchors.fill: parent
        anchors.topMargin: 5
        spacing: 5

        Slider {
            id: slider
            width: 154
            height: 15
            to: 100
            stepSize: 1
            anchors.horizontalCenter: parent.horizontalCenter
            value: 50
        }

        Item {
            id: item1
            height: 40
            anchors.right: parent.right
            anchors.left: parent.left

            Text {
                id: text1
                color: "#707070"
                text: Math.floor(slider.value)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
            }

            Button {
                id: button
                width: 30
                height: 30
                text: qsTr("-")
                autoRepeat: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                id: button1
                width: 30
                height: 30
                text: qsTr("+")
                autoRepeat: true
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    Connections {
        target: button
        onClicked: slider.decrease()
    }
    Connections {
        target: button1
        onClicked: slider.increase()
    }
}
