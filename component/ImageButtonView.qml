import QtQuick 2.4

GridView {
    id: gridView
    property color textcolor: "#75d7f9"
    signal clicked(int index)
    interactive: false
    cellWidth: width / model.count
    cellHeight: height

    model: ListModel {
        ListElement {
            name: qsTr("To Printer")
            imageSource: "Images/ScanToPrint.png"
        }
        ListElement {
            name: qsTr("To Email")
            imageSource: "Images/ScanToEmail.png"
        }
        ListElement {
            name: qsTr("To File")
            imageSource: "Images/ScanToFile.png"
        }
        ListElement {
            name: qsTr("To FTP")
            imageSource: "Images/ScanToFtp.png"
        }
        ListElement {
            name: qsTr("To AP")
            imageSource: "Images/ScanToProgram.png"
        }
        ListElement {
            name: qsTr("To Cloud")
            imageSource: "Images/ScanToCloud.png"
        }
    }
    delegate:ImageButtonViewDelegate {
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
