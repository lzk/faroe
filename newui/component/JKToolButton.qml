import QtQuick 2.0

JKAbstractButton {
    id: root
    width: 35
    height: 35
    property color borderColor: "#FFEEBA70"
    property color strokeColor: "#FF0E5686"

    Rectangle{
        anchors.fill: parent
        color:mouseArea.containsPress ?parent.borderColor :"transparent"
        border.color: !mouseArea.containsPress && (mouseArea.pressed || mouseArea.containsMouse) ?parent.borderColor :"transparent"
        border.width: 2
    }
}
