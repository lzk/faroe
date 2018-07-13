import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 2.2
import "../component"
import com.liteon.JKInterface 1.0

DelegateModel{
    id: visualModel
    property int currentIndex //for delete dialog
    property bool empty: visualModel.items.count === 0
    property bool selectedAll:!empty && visualModel.items.count === selectionGroup.count
    property var selectList:[]
    property var lastSelectList:[]
    function getThumbnailFilename(url){
        return jkImageModel.getThumbnailFilename(url)
    }
    function selectAll(selectOrDeselect){
        if(selectOrDeselect){
            for(var i = visualModel.items.count ;i > 0 ;i--){
                visualModel.items.addGroups(i-1 ,1 ,"selected")
            }
//            visualModel.items.addGroups(0 ,visualModel.items.count ,"selected")
        }else{
            if(!empty && selectionGroup.count === visualModel.items.count)
                selectionGroup.remove(0 ,selectionGroup.count)
        }
    }

    function deleteImage(index){
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
        visualModel.model.remove(index ,1);
    }

    groups: [
        DelegateModelGroup {
            id:selectionGroup
            name: "selected"
            includeByDefault :false
            onChanged:{
//                    root.selectedAll = count === visualModel.items.count
                var i,j
                var itemIndex
                var obj
                var value
                //remove removed value
                for(j = 0 ;j < removed.length ;j++){
                    obj = removed[j]
                    for(i = 0 ;i < obj.count ;i++){
                        if(obj.index + i < lastSelectList.length){
                            value = lastSelectList[obj.index + i]
                            itemIndex = selectList.indexOf(value)
                            if(itemIndex !== -1){
                                selectList.splice(itemIndex ,1)
                            }
                        }
                    }
                }
                //add inserted value
                for(j = 0 ;j < inserted.length ;j++){
                    obj = inserted[j]
                    for(i = 0 ;i < obj.count ;i++){
                        value = get(obj.index + i).itemsIndex
                        itemIndex = selectList.indexOf(value)
                        if(itemIndex !== -1){
                            selectList.splice(itemIndex ,1)
                        }
                        selectList.push(value)
                    }
                }
                //update sn
                for(i = 0 ;i <visualModel.model.count ;i++){
//                    visualModel.model.get(i).sn = selectList.indexOf(i) + 1
                    visualModel.model.setData(visualModel.modelIndex(i) ,selectList.indexOf(i) + 1 ,ImageModel.SnRole)
//                    console.log("index:" ,i ,"sn" ,selectList.indexOf(i) + 1)
                }
                //update last select list
                if(lastSelectList.length > 0)
                    lastSelectList.splice(0 ,lastSelectList.length)
                for(i = 0 ;i < count ;i++){
                    lastSelectList.push(get(i).itemsIndex)
                }

//                console.log("selectList:" ,selectList)
//                console.log("lastSelectList:" ,lastSelectList)
            }
        }
    ]
    model: jkImageModel
    delegate: ImageDelegate{
        width: GridView.view.cellWidth
        height: GridView.view.cellHeight
        text: sn//selected ?sn :""
        selected: DelegateModel.inSelected
//            image.source: "image://async/" + getThumbnailFilename(url)
        image.source: "file:///" +  getThumbnailFilename(url)
        onClose:{
            visualModel.currentIndex = index
            information(qsTr("ResStr_Are_you_sure_to_delete_the_selected_picture") ,deleteDialogClose)
        }
        onDoubleClicked: {
            if(image.status === Image.Ready){
                console.log("double")
                DelegateModel.inSelected = !DelegateModel.inSelected
                openPreviewDialog(function(dialog){
//                    dialog.image.source = "file:///" + url
//                    dialog.sourceSize = sourceSize
//                    dialog.model = jkImageModel
                    dialog.index = index
                    dialog.requestImage(0 ,0)
                })
            }
        }
        onToggled: {
            console.log("single")
            DelegateModel.inSelected = !DelegateModel.inSelected
        }

    }
    function deleteDialogClose(){
        deleteImage(visualModel.currentIndex)
    }
    property var dialog
//    function openPreviewDialog(init){
//        dialog = openDialog("ImageViewer/ImagePreviewDialog.qml" ,{} ,init)
//    }

    function removeAllImages(){
        jkImageModel.removeAll()
    }
    Component.onCompleted: {
        selectAll(true)
    }
}
