import QtQuick 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id:window
    property alias container: container
    flags: Qt.Dialog | Qt.FramelessWindowHint
    color: "transparent"
    modality: Qt.ApplicationModal // Qt.WindowModal

    Item{
        id:container
        anchors.fill: parent
        anchors.margins: 10
        visible: window.visible

        RectangularGlow{
            anchors.fill: parent
            glowRadius: 10
            spread: 0.2
            color: "#FF858484"
            cornerRadius: glowRadius
        }

    }

    function open(){
        show()
    }

}
