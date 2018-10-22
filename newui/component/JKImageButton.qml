import QtQuick 2.0

JKButton{
    id:root
    property url source_normal
    property url source_disable
    property url source_press
    Image {
        id:image
        parent: container
        anchors.fill: parent
        source: {
            switch(root.state){
            case "pressed":
                root.source_press
                break
            case "disable":
                root.source_disable
                break
            case "normal":
            case "enter":
                root.source_normal
                break
            }
        }
    }

    JKDashRectange{
        anchors.fill: parent
        anchors.margins: -2
        visible: root.activeFocus
    }
}

