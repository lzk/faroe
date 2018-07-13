import QtQuick 2.0
import QtQuick.Controls 2.2
import "../../component"
Item {
    width: 477
    height: 309

    Item {
        id: item1
        anchors.bottom: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottomMargin: -50

        JKText {
            id: text1
            text: qsTr("ResStr_Printer_Name")
            font.bold: true
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 14
        }

        JKComboBox {
            id: comboBox
            width: 250
            height: 35
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
//            onActivated: setting.printerName = textAt(index)
            model: jkInterface.getPrinterName()
        }
    }

    property var setting
    Component.onCompleted:{
        init()
    }

    function init(){
        var index = -1
        if(comboBox.count > 0)
            index = 0
        for (var i=0; i<comboBox.count; i++){
            if(comboBox.textAt(i) === setting.printerName){
                index = i
            }
            comboBox.currentIndex = index
        }
    }
    function ok(){
        setting.printerName = comboBox.currentText
        return true
    }
}
