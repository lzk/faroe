import QtQuick 2.0
import QtQml.Models 2.2
import QtQuick.Controls 2.2
import "../component"
import com.liteon.JKInterface 1.0

DelegateModel{
    id: visualModel
    property bool selectedAll:visualModel.items.count === selectionGroup.count
    property var selectList:[]
    property var lastSelectList:[]
    function getThumbnailFilename(url){
        return jkImageModel.getThumbnailFilename(url)
    }
    function selectAll(selectOrDeselect){
        if(selectOrDeselect){
            visualModel.items.addGroups(0 ,visualModel.items.count ,"selected")
        }else{
            if(selectionGroup.count === visualModel.items.count)
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

//        console.log("index:" ,index)
//        console.log("selectList:" ,selectList)
//        console.log("lastSelectList:" ,lastSelectList)

//        DelegateModel.model.model.remove(index ,1);
        visualModel.model.remove(index ,1);
        console.log("index:" ,index)
    }

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
                        visualModel.model.get(i).sn = selectList.indexOf(i) + 1
//                        console.log("before set value i" ,i)
//                    visualModel.model.setData(visualModel.modelIndex(i) ,selectList.indexOf(i) + 1 ,ImageModel.SnRole)
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
    model: jkImageModel
    delegate: ImageDelegate{
        width: GridView.view.cellWidth
        height: GridView.view.cellHeight
        text.text: sn//selected ?sn :""
        selected: DelegateModel.inSelected
//            image.source: "image://async/" + getThumbnailFilename(url)
        image.source: "file:///" +  url//getThumbnailFilename(url)
        onClose:{
//            var dialog = deleteDialog
            var dialog = getDialog("delete")
            dialogs.index = index
            dialog.open()

        }
        onDoubleClick: {
//            var dialog = previewDialog
            var dialog = getDialog("preview")
            if(image.status === Image.Ready){
                dialog.image.source = "file:///" + url
                dialog.sourceSize = sourceSize
                dialog.index = index
                dialog.requestImage(0 ,0)
                dialog.open()
//                dialog.x = dialogs.centerx - dialog.width / 2
//                dialog.y = dialogs.centery - dialog.height / 2
            }
        }
        onClick: {
            DelegateModel.inSelected = !DelegateModel.inSelected
        }

        Loader{
            id:dialogs
            property int index
//            property int centerx: ApplicationWindow.window.x + ApplicationWindow.window.width / 2
//            property int centery: ApplicationWindow.window.y + ApplicationWindow.window.height / 2
        }

        function deleteDialogClose(){
            deleteImage(dialogs.index)
            dialogs.source = ""
        }

        function getDialog(dialogname){
            var dialog = undefined
            dialogs.source = ""
            switch(dialogname){
            case "delete":
                dialogs.source = "../component/JKMessageBox_information.qml"
                dialog = dialogs.item
                dialog.message.text = qsTr("Do you want to delete the selected image?")
                dialog.accepted.connect(deleteDialogClose)
                break
            case "preview":
                dialogs.source = "ImagePreviewDialog.qml"
                dialog = dialogs.item
                break
            default:
                break
            }
            return dialog
        }
    }
}
