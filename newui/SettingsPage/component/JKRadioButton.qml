import QtQuick 2.7
 import QtQuick.Controls 2.2

 RadioButton {
     id: control
     text: qsTr("RadioButton")
//     checked: true
     font.pixelSize: 14
     opacity: enabled ? 1.0 : 0.3

     indicator: Rectangle {
         implicitWidth: 24
         implicitHeight: 24
         x: control.leftPadding
         anchors.verticalCenter: parent.verticalCenter
         radius: 12
         border.color: "lightgray"

         Rectangle {
             width: 14
             height: 14
             radius: 7
             anchors.centerIn: parent
             color:  "#21be2b"
             visible: control.checked
         }
     }

     contentItem: Text {
         text: control.text
         font: control.font
//         horizontalAlignment: Text.AlignHCenter
         verticalAlignment: Text.AlignVCenter
         leftPadding: control.indicator.width + control.spacing
     }
 }
