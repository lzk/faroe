import QtQuick 2.0

Item {
    id: container

    property alias label: labelText.text
    property bool selected: true
    property bool center: false
    signal clicked

    width: labelText.width + 70 ; height: 34

    BorderImage {
        anchors { fill: container;}
        source: {
            if(selected){
                mouseArea.pressed ?"Images/Btn_Green_Pressed.png":"Images/Btn_Green.png"
            }else{
                mouseArea.pressed ?"Images/Btn_Gray_Pressed.png":"Images/Btn_Gray.png"
            }
        }

        border.top: 17;border.left: 17;border.right: 17;border.bottom: 17;
    }

    Text {
        id: labelText;
        font.pixelSize: 15;
        color: {
            if(selected){
                mouseArea.containsMouse&&!mouseArea.pressed ?"black":"white"
            }else{
                mouseArea.containsMouse&&!mouseArea.pressed ?"green":mouseArea.pressed?"white":"black"
            }
        }
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: center ?parent.horizontalCenter:parent.left
        anchors.horizontalCenterOffset: center ?0 :17 + width / 2
    }

    MouseArea {
        id:mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: container.clicked()
    }
}
