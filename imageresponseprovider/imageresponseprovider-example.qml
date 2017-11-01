import QtQuick 2.7
import QtQuick.Controls 2.0
import "../thumbnailviewer"

ApplicationWindow {
    id:window
    width: 640
    height: 480
    visible: true
    title: qsTr("ScalesImage")

    StackView{
        id:stackView
        anchors.bottomMargin: 10
        anchors.bottom: button.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        initialItem: ThumbnailViewer{
            id:thumbnailViewer
        }
    }

    Button {
        id: button
        text: qsTr("Scan")
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: jkInterface.add()
    }

    CheckBox {
        id: checkBox
        x: 532
        y: 430
        text: qsTr("Select All")
        checked: thumbnailViewer.selectedAll
        onCheckedChanged: thumbnailViewer.selectAll(checked)
    }

}
