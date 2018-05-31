import QtQuick 2.0
import "component"

JKDialog {
    width: 400 + 20
    height: 200 + 20

    property int pixelSize: 13
    property color textColor: "#222222"

    toolbar{
        text.text: qsTr("About")
        text.color: "White"
        color: "#FF203144"
    }

    Rectangle{
        parent: container
        anchors.fill: parent

        Column{
            anchors.fill: parent
            anchors.margins: 30
            spacing: 5

            JKText{
                font.pixelSize: pixelSize
                text: qsTr("Faroe Virtual Panel 0.0.14")
                color: textColor
            }

            Item {
               width: parent.width
               height: 10
            }

            JKText{
                font.pixelSize: pixelSize
                text: qsTr("Copyright (C) (2018)")
                color: textColor
            }
            JKText{
                font.pixelSize: pixelSize
                text: qsTr("Liteon Limited.")
                color: textColor
            }
            JKText{
                font.pixelSize: pixelSize
                text: qsTr("All Rights Reserved")
                color: textColor
            }
        }
    }
}
