import QtQuick 2.4

GridView {
    id: gridView

    signal clicked(int index)

    interactive: false
    cellWidth: width / 4
    cellHeight: height
    property color textcolor: "#75d7f9"

    model: ListModel {
        ListElement {
            name: qsTr("QR Code")
            imageSource: "Images/QRCode.png"
        }
        ListElement {
            name: qsTr("Bar Code")
            imageSource: "Images/BarCode.png"
        }
        ListElement {
            name: qsTr("Scan To")
            imageSource: "Images/ScanTo.png"
        }
        ListElement {
            name: qsTr("Settings")
            imageSource: "Images/ScanSettings.png"
        }
    }
    delegate: ButtonViewDelegate {
        id: button
        width: gridView.cellWidth
        height: gridView.cellHeight
        text.text: name
        text.color: textcolor
        image.source: imageSource
        Connections {
            target: button
            onClicked: {
                gridView.clicked(index)
            }
        }
    }
}
