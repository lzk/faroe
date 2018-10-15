import QtQuick 2.7
import QtQuick.Controls 2.2

ComboBox {
    id: control
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            id:text
            text: modelData
//            color: "lightgray"
            font: control.font
            elide: Text.ElideMiddle
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
        ToolTip{
            id:tooltip
            visible: parent.hovered
            text:modelData
            contentItem: Text {
                text:tooltip.text
                font: tooltip.font
                wrapMode: Text.WrapAnywhere
            }

            background: Rectangle{
                color: "lightblue"//"#9adbc8"
            }
            closePolicy:Popup.NoAutoClose
        }
    }

    indicator: Rectangle{
        x: control.width - width// - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 20
        height: parent.height
        color:"transparent"
        Canvas {
            id: canvas
            width: 12
            height: 8
            anchors.centerIn: parent
            contextType: "2d"

            Connections {
                target: control
                onPressedChanged: canvas.requestPaint()
            }

            onPaint: {
                context.reset();
                context.moveTo(0, 0);
                context.lineTo(width, 0);
                context.lineTo(width / 2, height);
                context.closePath();
                context.fillStyle = control.pressed ? "darkgray" : "gray";
                context.fill();
            }
        }
    }

    contentItem:
//        Text {
//        leftPadding: 0
//        rightPadding: control.indicator.width + control.spacing

//        text: control.displayText
//        font: control.font
//        color: "lightgray"
//        verticalAlignment: Text.AlignVCenter
//        elide: Text.ElideRight
//    }
    Rectangle{
        z:2
//            y:parent.height
        width: /*hovered ?parent.width :*/parent.width - parent.indicator.width
        height:  parent.height//text2.height > parent.height ?text2.height :parent.height
        color: "transparent"//hovered ?"lightblue" :"transparent"
        Text{
            id:text2
            x:5
            anchors.verticalCenter: parent.verticalCenter
            text:control.displayText
            font: control.font
            width: parent.width - x
            elide: Text.ElideMiddle
//            wrapMode: hovered ?Text.WrapAnywhere :Text.NoWrap
        }
    }
    ToolTip{
        id:tooltip2
        visible: hovered
        text:control.displayText
        contentItem: Text {
            text:tooltip2.text
            font: tooltip2.font
            wrapMode: Text.WrapAnywhere
        }

        background: Rectangle{
            color: "lightblue"//"#9adbc8"
        }
        closePolicy:Popup.NoAutoClose
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        border.color: control.pressed ? "darkgray" : "lightgray"
        border.width: control.visualFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1
//        clip: true
        contentItem:
            ScrollView{
                clip: true
                ScrollBar.vertical.policy: height + parent.padding * 2 < listview.contentHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
                ListView {
                id:listview
//                height: contentHeight
                model: control.popup.visible ? control.delegateModel : null
    //            currentIndex: control.highlightedIndex

    //            ScrollIndicator.vertical: ScrollIndicator {
    //            }

                }
            }


        background: Rectangle {
            border.color: "darkgray"
            radius: 2
        }
    }
}
