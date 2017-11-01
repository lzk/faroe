import QtQuick 2.4
import QtQuick.Controls 2.2
import "../component"
import "../thumbnailviewer"

Item {
    id: root
    property alias gridView: gridView

    JKImageButton {
        id: button_back
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        pressedSource: "Images/back_press.png"
        hoverSource: "Images/back_over.png"
        normalSource: "Images/back_normal.png"
        source: "Images/back_normal.png"
    }

    CheckBox {
        id: checkBox
        x: 246
        y: 20
        text: qsTr("Select All")
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.bottom: button_back.bottom
        checked: thumbnailViewer.selectedAll
        onCheckedChanged: thumbnailViewer.selectAll(checked)
    }

    Image {
        id: image
        anchors.topMargin: 20
        anchors.bottomMargin: 50
        source: "Images/ScanView.png"

        anchors.rightMargin: 80
        anchors.leftMargin: 80
        anchors.top: button_back.bottom
        anchors.right: parent.right
        anchors.bottom: gridView.top
        anchors.left: parent.left

    }

    ThumbnailViewer {
        id: thumbnailViewer
        anchors.fill: image
    }

    ImageButtonView {
        id: gridView
        y: 380
        height: 100
        anchors.rightMargin: 30
        anchors.leftMargin: 30
        anchors.right: image.right
        anchors.left: image.left
        anchors.bottom: parent.bottom
    }

    JKDialog{
        id:backDialog
        Rectangle{
            parent: backDialog.container
            anchors.fill:parent
            Text{
                anchors.centerIn: parent
                text: qsTr("delete all?")
            }
            Button{
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("OK")
                onClicked: backDialog.accept()
            }
        }


        onAccepted: {

            jkInterface.model.removeAll()
            root.StackView.view.pop()
        }
    }

    Connections {
        target: button_back
        onClicked:{
            backDialog.open()
            backDialog.moveToCenter()
        }
    }
}
