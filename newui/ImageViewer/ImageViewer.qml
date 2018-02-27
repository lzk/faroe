import QtQuick 2.0
import QtQuick.Controls 2.2
import com.liteon.JKInterface 1.0

Item {
    id:root
    property alias allSelected: model.selectedAll
//    property alias model: gridView.model
    property alias selectList: model.selectList

    Item{
//    ScrollView{
        anchors.fill: parent
        clip: true
//        ScrollBar.vertical.policy:ScrollBar.AlwaysOff
//        ScrollBar.horizontal.policy:ScrollBar.AlwaysOff
//        ScrollBar.horizontal.interactive: false
//        ScrollBar.vertical.interactive: false

//        ScrollBar.vertical.policy: contentHeight > availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
//        ScrollBar.vertical.x:width-10
//        background:
        Rectangle{
            anchors.fill: parent
            color: "white"
            border.color: "#44000000"
            radius: 5
        }
        GridView{
            id: gridView
            anchors.fill: parent
            property int columns: 4
            signal clicked(int index)
//            clip:true
//            interactive: false
            cellWidth: width / columns
            cellHeight:height / 2
//            cellHeight: cellWidth * 4 / 3
            model: ImageDelegateModel{
                id:model
            }
        }
    }

    function selectAll(checked){
        model.selectAll(checked)
    }

    function removeAllImages(){
        model.removeAllImages()
    }

    function moveDown(){
        gridView.moveCurrentIndexDown()
        gridView.moveCurrentIndexDown()
    }

    function moveup(){
        gridView.moveCurrentIndexUp()
        gridView.moveCurrentIndexUp()
    }
}
