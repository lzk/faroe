import QtQuick 2.0

Item {

    property color color: "#FF0E5686"
    property int lineWidth: 3

    Rectangle{
        anchors.fill: parent
        anchors.margins: 0.25*parent.width
        anchors.topMargin: parent.height - parent.lineWidth - 0.25*parent.width
        color: parent.color
    }

}
