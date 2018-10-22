import QtQuick 2.7
import "../component"
Item {
    id: root
    property alias source_disable: button.source_disable
    property alias source_normal: button.source_normal
    property alias source_press: button.source_press
    property alias jktext: jktext
    signal clicked

    JKImageButton{
        id:button
        width: parent.width
        height: parent.width
        onClicked: {
            root.clicked()
        }
    }
    JKText {
        id:jktext
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
    }
}

