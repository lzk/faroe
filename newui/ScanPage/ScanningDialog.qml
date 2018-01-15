import QtQuick 2.0
import QtQuick.Window 2.2
import "../component"

Window {
    flags: Qt.Dialog | Qt.FramelessWindowHint
    color: "transparent"
    property alias text: text1.text
    modality: Qt.ApplicationModal // Qt.WindowModal
    id:root
    width: 850
    height: 560
    signal cancel
    property var dialog

    Rectangle{
        anchors.fill: parent
        color: "#ccffffff"
    }

    Rectangle {
        id: rectangle
        width: 154
        height: 70
        color: "#ffe5e5e5"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Item {
            id: item1
            x: 0
            y: 40
            height: 30
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left

            Rectangle {
                id: rectangle1
                color: "transparent"
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                anchors.bottomMargin: 15
                anchors.fill: parent
                border.color: "#44444444"
                clip: true
                Rectangle {
                    id: rectangle2
                    width: 50
                    color: "green"
                    anchors.bottomMargin: 1
                    anchors.topMargin: 1
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top

                    NumberAnimation on x{
                        from: -rectangle2.width
                        to: rectangle1.width
                        duration: 1500
                        loops: Animation.Infinite
                    }
                }

            }
        }

        Item {
            id: item2
            anchors.bottom: item1.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            JKText {
                id: text1
                x: 0
                y: 0
                text: qsTr("Scanning...")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 20
            }
        }
    }

    JKAbstractButton{
        width: 50
        height: 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rectangle.bottom
        anchors.topMargin: 20
        dashRectange.radius: 26
        Image{
            width: 32
            height: 32
            anchors.centerIn: parent
            source: parent.enabled ?parent.mouseArea.pressed
                                     ?"qrc:/Images/cancelIcon.png"
                                     :"qrc:/Images/cancelPress.png"
            :"qrc:/Images/cancelDisable.png"
        }
        onClicked: cancel()

    }
    function open(){
        show()
    }
}
