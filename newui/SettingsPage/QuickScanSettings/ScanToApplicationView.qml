import QtQuick 2.0
import QtQuick.Controls 2.2
import "../../component"
Item {
    width: 477
    height: 309

    Column{
        anchors.fill: parent
        Item {
            id: item1
            width: parent.width
            height: 60

            JKText {
                id: text1
                text: qsTr("Programs:")
                font.bold: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
            }

            JKComboBox {
                id: comboBox
                width: 250
                height: 35
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                model: ["Paint" ,"Windows Photo Viewer" ,"Other Application"]
            }
        }
        Item {
            id: item2
            width: parent.width
            height: 60


            JKTextInput {
                id: textInput2
                width: 250
                height: 30
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                visible: comboBox.currentIndex === 2
            }
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
            if(comboBox.textAt(i) === setting.fileName){
                index = i
            }
        }
        comboBox.currentIndex = index
        textInput2.text = setting.filePath
//            textInput3.text = setting.filePath
    }
    function ok(){
        setting.fileName = comboBox.currentText
        setting.filePath = textInput2.text
//        setting.filePath = textInput3.text
    }
}
