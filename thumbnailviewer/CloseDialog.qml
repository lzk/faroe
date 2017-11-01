import QtQuick 2.0
import QtQuick.Controls 2.2
import "../component" as JKComponent

JKComponent.JKDialog {
    id: root
    Rectangle {
        id: rectangle
        parent: container
        color: "#ffffff"
        anchors.fill: parent
//        anchors.topMargin: 30


        Text {
            anchors.centerIn: parent
            text: qsTr("Delete selected image?")
        }
        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            Button{
                text: qsTr("OK")
                onClicked:accept()
            }
            Button{
                text:qsTr("Cancel")
                onClicked:reject()
            }
        }

    }
}
