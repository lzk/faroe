import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

ApplicationWindow {
    id:window
    visible: true
    width: 850
    height: 638
    title: qsTr("Faroe VOP")
    flags: Qt.Window | Qt.FramelessWindowHint

    MainView{
        id:mainview
        anchors.fill: parent
    }
    Connections{
        target: mainview
        onClosed: close()
        onMinimized: showMinimized()//window.visibility = ApplicationWindow.Minimized
        onMove: {
            window.x += dx
            window.y += dy
        }
    }
}
