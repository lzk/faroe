import QtQuick 2.7
Rectangle{

    color: "white"
    border.color: "gray"
    property alias input: input
    property alias text: input.text
    clip: true
    TextInput {
        id:input
        anchors.fill: parent
        anchors.margins: 8
        width: parent.width
//        focus: true
        font.family: "Verdana"
        font.pixelSize: 14
        onFocusChanged: {
            if(focus){
            }
        }
    }
}
