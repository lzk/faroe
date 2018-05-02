import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

ApplicationWindow {
    id:root
    visible: true
    width: 850
    height: 638
    title: qsTr("Faroe VOP")
    flags:
        Qt.Window | Qt.FramelessWindowHint

    MainView{
        id:mainview
        anchors.fill: parent
        centerx: window.x + window.width / 2
        centery: window.y + window.height / 2
        window: root
    }
    Connections{
        target: mainview
        onClosed: close()
        onMinimized: {
            if(jkInterface.macVersion() > 14){
                hide()
                flags = Qt.Window
                showNormal()
//                jkInterface.showMinimize(root)
                showMinimized()//root.visibility = Window.Minimized//
                flags = Qt.Window | Qt.FramelessWindowHint
            }else{
                showMinimized()
            }

        }
        onMove: {
            root.x += dx
            root.y += dy
        }
    }

    onClosing: {
        console.log("closing")
    }

//    Component.onCompleted: {
//        jkInterface.setWindowFrameless(root)
//    }

//    onVisibilityChanged: {
//        if(jkInterface.macVersion() > 14){
//            flags = visibility ===  Window.Minimized ?Qt.Window :Qt.Window | Qt.FramelessWindowHint
//        }
//    }
}
