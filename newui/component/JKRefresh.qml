import QtQuick 2.0
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id:window
    flags: Qt.Dialog | Qt.FramelessWindowHint
    color: "transparent"
    modality: Qt.ApplicationModal // Qt.WindowModal
    width: 100
    height: 100
    property var dialog

    Item{
        anchors.fill: parent
        anchors.margins: 10
        visible: window.visible

        JKBusyIndicator{
            anchors.fill: parent
        }

    }

    function open(){
        show()
    }
}
