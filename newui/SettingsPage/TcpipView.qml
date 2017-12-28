import QtQuick 2.0

Item {
    id: item1
    width: 495
    height: 460
    property alias text1Text: text1.text

    Text {
        id: text1
        x: 142
        y: 119
        text: qsTr("Tcp ip")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 12
    }

}
