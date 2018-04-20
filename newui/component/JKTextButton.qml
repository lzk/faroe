import QtQuick 2.7
//import QtGraphicalEffects 1.0
//import QtQuick.Controls 2.2

JKButton{
//Button{
    id:root
//    property alias text: text1.text
    property int fontSize: 16
    Text{
        id: text1
        parent:rectangle
        text:root.text
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Verdana"
        font.pixelSize: fontSize
        color: root.state === "disable" ?"lightgray" :"black"
    }
//    background:
    Rectangle {
        id:rectangle
        parent: container
        anchors.fill: parent
        radius: 5
        opacity: 0.8
        border.color: "lightgray"
        color: root.state === "pressed" ?"lightgray" :"transparent"
    }

}
