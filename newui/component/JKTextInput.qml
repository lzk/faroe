import QtQuick 2.7
Rectangle{
    color: "white"
    border.color: "gray"
    property alias input: input
    property alias text: input.text
    property alias maximumLength: input.maximumLength
    property alias validator: input.validator
    property alias echoMode: input.echoMode
    clip: true
    TextInput {
        id:input
        anchors.fill: parent
        anchors.margins: 8
        width: parent.width
        focus: true
        selectByMouse: true
        font.family: "Verdana"
        font.pixelSize: 14
        horizontalAlignment:TextInput.AlignLeft
        onFocusChanged: {
            if(!focus){
                cursorPosition = 0
            }
        }
    }
}
