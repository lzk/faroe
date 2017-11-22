import QtQuick 2.0
import QtQuick.Controls 2.2
import "../component"

Item {
    id:root

        JKImageButton {
            id: button_back
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top: parent.top
            pressedSource: "Images/back_press.png"
            hoverSource: "Images/back_over.png"
            normalSource: "Images/back_normal.png"
            source: "Images/back_normal.png"
            onClicked: {
                root.StackView.view.pop()
            }
        }
        JKImageButton{
            id:button_refresh
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.top: parent.top
            width: 60
            height: 60
            pressedSource: "Images/clockwise-arrow.png"
            hoverSource: "Images/clockwise-arrow.png"
            normalSource: "Images/clockwise-arrow.png"
            source: "Images/clockwise-arrow.png"
        }

        ScrollView{
            id:scrollView
            anchors.left: button_back.right
            anchors.leftMargin: 50
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            anchors.right: button_refresh.left
            anchors.rightMargin: 50

            background: Rectangle{}

            ListView{
                id:listview
                anchors.fill: parent
                model: jkInterface.deviceList
                clip: true

                delegate: Rectangle {
                    id:wrapper
                    height: 150
                    width: ListView.view.width
                    Text { text: modelData }
                    color: ListView.isCurrentItem ? "yellow" : "white"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: wrapper.ListView.view.currentIndex = index
                    }
                }
            }
        }


        Button{
            text: "Connect"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: scrollView.bottom
            anchors.topMargin: 20
            onClicked: {
                jkInterface.connectDevice(listview.currentIndex)
            }
        }




    Connections {
        target: button_refresh
        onClicked: {
            jkInterface.deviceList.clear()
            jkInterface.searchDeviceList()
        }
    }
}
