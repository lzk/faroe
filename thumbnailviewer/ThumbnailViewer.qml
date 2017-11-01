import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQml.Models 2.2
import "../component" as JKComponent
import com.liteon.JKInterface 1.0
Item{
    id:root

    property bool selectedAll:visualModel.items.count === selectionGroup.count
    property var selectList:[]
    property var lastSelectList:[]
    function selectAll(selectOrDeselect){
        if(selectOrDeselect){
            visualModel.items.addGroups(0 ,visualModel.items.count ,"selected")
        }else{
            if(selectionGroup.count === visualModel.items.count)
                selectionGroup.remove(0 ,selectionGroup.count)
        }

    }

    function close(index){
        var i
        for(i = 0 ; i < selectList.length ;i++){
            if(selectList[i] > index)
                selectList[i] --
        }
        for(i = lastSelectList.length - 1 ;i >= 0 ;i --){
            if(lastSelectList[i] > index)
                lastSelectList[i] --
            else
                break
        }

//        console.log("index:" ,index)
//        console.log("selectList:" ,selectList)
//        console.log("lastSelectList:" ,lastSelectList)

//        DelegateModel.model.model.remove(index ,1);
        visualModel.model.remove(index ,1);
        console.log("index:" ,index)
    }

    Rectangle{
        id:background
        color: "black"
        anchors.fill: parent
    }

    ListModel{
        id:imageModelforTest
        ListElement{url:"E:/tmp/pic/1.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/2.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/3.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/4.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/5.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/6.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/7.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/8.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/9.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/10.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/11.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/12.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/13.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/14.jpg" ;sn:0}
        ListElement{url:"E:/tmp/pic/15.jpg" ;sn:0}
    }

    DelegateModel{
        id: visualModel
        model: jkInterface.model

        groups: [
            DelegateModelGroup {
                id:selectionGroup
                name: "selected"
                includeByDefault :true
                onChanged:{
//                    root.selectedAll = count === visualModel.items.count
                    var i,j
                    var itemIndex
                    var obj
                    var value
//                    console.log("before remove value")
                    for(j = 0 ;j < removed.length ;j++){
                        obj = removed[j]
                        for(i = 0 ;i < obj.count ;i++){
                            if(obj.index + i < lastSelectList.length){
                                value = lastSelectList[obj.index + i]
                                itemIndex = selectList.indexOf(value)
                                if(itemIndex !== -1){
                                    selectList.splice(itemIndex ,1)
                                }
    //                            console.log("remove value:" ,value)
                            }
                        }
                    }
//                    console.log("before add value")
                    for(j = 0 ;j < inserted.length ;j++){
                        obj = inserted[j]
                        for(i = 0 ;i < obj.count ;i++){
                            value = get(obj.index + i).itemsIndex
                            itemIndex = selectList.indexOf(value)
                            if(itemIndex !== -1){
                                selectList.splice(itemIndex ,1)
                            }
                            selectList.push(value)
//                            console.log("add value to the end:" ,value)
                        }
                    }

//                    console.log("before set value")
                    for(i = 0 ;i <visualModel.model.count ;i++){
//                        visualModel.model.get(i).sn = selectList.indexOf(i) + 1
//                        console.log("before set value i" ,i)
                        visualModel.model.setData(visualModel.modelIndex(i) ,selectList.indexOf(i) + 1 ,ImageModel.SnRole)
//                        console.log("end set value i" ,i)
                    }

//                    console.log("before update value")
                    if(lastSelectList.length > 0)
                        lastSelectList.splice(0 ,lastSelectList.length)
                    for(i = 0 ;i < count ;i++){
                        lastSelectList.push(get(i).itemsIndex)
                    }

//                    console.log("end")
//                    console.log("selectList:" ,selectList)
//                    console.log("lastSelectList:" ,lastSelectList)
                }
            }
        ]
        delegate: ThumbnailDelegate{
            width: GridView.view.cellWidth
            height: GridView.view.cellHeight
            text.text: sn//selected ?sn :""
            selected: DelegateModel.inSelected
//            image.source: "image://async/" + jkInterface.getThumbnailFilename(url)
            image.source: "file:///" + jkInterface.model.getThumbnailFilename(url)
            onClose:{
                closeDialog.index = index
                closeDialog.open()
                closeDialog.moveToCenter()

            }
            onDoubleClick: {
                if(image.status == Image.Ready){
                    previewDialog.image.source = "file:///" + url
                    previewDialog.sourceSize = sourceSize
                    previewDialog.index = index
                    previewDialog.requestImage(0 ,0)
                    previewDialog.open()
                    previewDialog.moveToCenter()
                }
            }
            onClick: {
                DelegateModel.inSelected = !DelegateModel.inSelected
            }
        }
    }

    ScrollView{
        anchors.fill: parent
        focus: true
        clip: true
        ScrollBar.vertical.policy: contentHeight > availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
//        ScrollBar.vertical.x:width-10
        GridView{
            id: gridView
            property int columns: 5
            signal clicked(int index)
            clip:true
            cellWidth: width / columns
            cellHeight: cellWidth * 4 / 3
            model: visualModel
        }
    }

    CloseDialog{
        id:closeDialog
        property int index
        onAccepted: root.close(index)
    }

    ImagePreviewDialog{
        id:previewDialog
        width:Math.round(ApplicationWindow.window.width * 1  / 2)
        height: Math.round(ApplicationWindow.window.height * 4 / 5 )

    }

}
