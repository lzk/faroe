import QtQuick 2.0
import QtQuick.Controls 2.2
import com.liteon.JKInterface 1.0

Item {
    id:root
    property alias allSelected: model.selectedAll

    ScrollView{
        anchors.fill: parent
        clip: true
        ScrollBar.vertical.policy: contentHeight > availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
//        ScrollBar.vertical.x:width-10
        GridView{
            id: gridView
            anchors.fill: parent
            property int columns: 5
            signal clicked(int index)
//            clip:true
            cellWidth: width / columns
            cellHeight: cellWidth * 4 / 3
            model: ImageDelegateModel{
                id:model
            }
        }
        background:
            Rectangle{
                color: "white"
                border.color: "#44000000"
                radius: 5
            }
    }

    function selectAll(checked){
        model.selectAll(checked)
    }

    function removeAllImages(){
        jkImageModel.removeAll()
    }
}
