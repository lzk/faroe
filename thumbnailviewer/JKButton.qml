import QtQuick 2.0
import QtQuick.Controls 2.2
Button {
    id: button
    implicitHeight: 35
    implicitWidth: 60
    background: BorderImage {
        id: bg
        height: 35
        width: 60
        source:
            button.hovered?"Images/Btn_Gray_Pressed.png":"Images/Btn_Gray.png"
        opacity: button.down ?0.5 :1.0

        border.left: 17; border.top: 17
        border.right: 17; border.bottom: 17
    }
    contentItem: Item{
        Image{
            id:image
            source: "Images/Fit.tif"
            anchors.centerIn: parent
        }
    }

}

