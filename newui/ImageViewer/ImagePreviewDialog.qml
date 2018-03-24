import QtQuick 2.0
import QtQuick.Controls 2.2
import com.liteon.JKInterface 1.0
import "../component"
import "../component/path"
JKDialog{
    id: root
    property alias image: scrollView.image
    property bool save: true
    property bool showExtra: false
    property int weight: 0
    property int angle: 0
    property size sourceSize :index<0?"0x0":model.get(index ,ImageModel.SourceSizeRole)
    property var model:jkImageModel
    property int index:-1
    property string title: qsTr("Scan To Preview")
    property string name: ""
    width: 648+20
    height: 700+20
    toolbar{
        text.text:title
        text.font.pixelSize: 15
        text.color: "black"
        text.font.bold: true
    }

    ImagePreivewLayout{
        id:ip
        parent: container
        anchors.fill: parent
        item_extra.visible: showExtra
        ImageScrollView {
            id: scrollView
            parent:  ip.item_view
            anchors.fill:parent
            image.source: index < 0?"":"file:///" + model.get(index ,ImageModel.UrlRole)
        }

        JKFunctionButton{
            id:button_fit
            parent: ip.item_fb1
            anchors.fill: parent
    //        jktext.text: qsTr("Fit")
    //        jktext.font.pixelSize: 12
            source_normal: "qrc:/Images/fit.png"
            source_press: "qrc:/Images/fit.png"
            source_disable: "qrc:/Images/fit.png"
            onClicked: {
                requestImage(0 ,angle)
            }
            enabled: weight !== 0
        }
        JKFunctionButton{
            id:button_zoomin
            parent: ip.item_fb2
            anchors.fill: parent
    //        jktext.text: qsTr("Zoom In")
    //        jktext.font.pixelSize: 12
            source_normal: "qrc:/Images/zoomin.png"
            source_press: "qrc:/Images/zoomin.png"
            source_disable: "qrc:/Images/zoomin.png"
            onClicked: requestImage(++weight ,angle)
            enabled: (image.width * (1.0 + weight / 10.0) < sourceSize.width)
            && (image.height * (1.0 + weight / 10.0) < sourceSize.height)
        }
        JKFunctionButton{
            id:button_zoomout
            parent: ip.item_fb3
            anchors.fill: parent
    //        jktext.text: qsTr("Zoom Out")
    //        jktext.font.pixelSize: 12
            source_normal: "qrc:/Images/zoomout.png"
            source_press: "qrc:/Images/zoomout.png"
            source_disable: "qrc:/Images/zoomout.png"
            onClicked: requestImage(--weight ,angle)
            enabled: weight >= -4
        }
        JKFunctionButton{
            id:button_turn
            parent: ip.item_fb4
            anchors.fill: parent
    //        jktext.text: qsTr("Turn")
    //        jktext.font.pixelSize: 12
            source_normal: "qrc:/Images/turn.png"
            source_press: "qrc:/Images/turn.png"
            source_disable: "qrc:/Images/turn.png"
            onClicked: requestImage(weight ,++angle)
        }

        JKTextButton{
            parent: ip.item_return
            anchors.fill: parent
            text.text: qsTr("Return")
            onClicked: {
                toolbar.close()
            }
        }

        JKPath_pre{
            visible: index !== 0
            parent: ip.item_pre
            anchors.fill: parent
            onClicked: {
                if(index > 0)
                    index --
                requestImage(0 ,0)
            }

        }

        JKPath_next{
            visible: index !== model.rowCount() - 1
            parent: ip.item_next
            anchors.fill: parent
            onClicked: {
                if(index < model.rowCount() - 1)
                    index ++
                requestImage(0 ,0)
            }
        }

        JKText{
            parent: ip.item_viewname
            anchors.centerIn: parent
            text:name
        }
    }

    defaultClose:false
    Connections{
        target: toolbar
        onClose:{
            if(save && angle % 4 !== 0){
                information(qsTr("The scanned image has been changed.Do you want to save changes?")
                                        ,returnSave ,root.close)
            }else{
                close()
            }
        }
    }

    function returnSave(){
        model.saveRotatoedImage(index ,angle)
        close()
    }

    function requestImage(weight ,angle){
        var factor = (1.0 + weight / 10.0)
        var width = scrollView.availableWidth * factor
        var height = scrollView.availableHeight * factor
//        if(angle % 2 === 0){
            image.width  = width > sourceSize.width ?sourceSize.width :width
            image.height = height > sourceSize.height ?sourceSize.height :height
//        }else{
//            image.width  = height > sourceSize.width ?sourceSize.width :height
//            image.height = width > sourceSize.height ?sourceSize.height :width
//        }
        image.rotation = angle % 4 * 90
        root.weight = weight
        root.angle = angle % 4
    }
}
