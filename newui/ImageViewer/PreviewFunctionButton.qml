import QtQuick 2.0
import QtQuick.Controls 2.2
Button {
    id: button
    spacing: -3
    implicitHeight: 35
    implicitWidth: 60
    property alias source: image.source
    background: BorderImage {
        id: bg
        height: button.height
        width: button.width
        source:
            button.hovered?"qrc:/Images/Btn_Gray.png":"qrc:/Images/Btn_Gray.png"
        opacity: button.down ?0.5 :1.0

        border.left: 17; border.top: 17
        border.right: 17; border.bottom: 17
    }
    contentItem: Item{
        Image{
            id:image
            anchors.centerIn: parent
        }
    }

}
