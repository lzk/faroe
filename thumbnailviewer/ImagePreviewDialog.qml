import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQml.Models 2.2
import "../component" as JKComponent
import com.liteon.JKInterface 1.0
JKComponent.JKDialog {
    id: root
    property alias image: image

    property int weight: 0
    property int angle: 0
    property size sourceSize
//    property var model:jkInterface.model
    property int index:-1
    toolbar.text: qsTr("Preview")
    Rectangle {
        id: rectangle
        parent: container
        color: "#ffffff"
        anchors.fill: parent

        ScrollView {
            id: scrollView
            anchors.margins: 10
            anchors.bottom: row.top
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            focus: true
            clip: true
            ScrollBar.vertical.policy: contentHeight > availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: contentWidth > availableWidth ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
//            ScrollBar.horizontal.policy:image.width > scrollView.availableWidth ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
//            ScrollBar.horizontal.y:height-10
//            ScrollBar.vertical.policy: image.height > scrollView.availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
//            ScrollBar.vertical.x:width-10

            contentWidth: image.width > scrollView.width ?image.width :scrollView.width
            contentHeight: image.height > scrollView.height ?image.height :scrollView.height
            Item{
//                implicitWidth:{image.width > scrollView.availableWidth ?image.width :scrollView.availableWidth}
//                implicitHeight: {image.height > scrollView.availableHeight ?image.height :scrollView.availableHeight}
                anchors.fill: parent
                Image{
                   id:image
                   asynchronous : true
                   cache: false
                   fillMode: Image.PreserveAspectFit
                   width:scrollView.availableWidth
                   height:scrollView.height
//                   source:index >= 0 ?"file:///" + model.data(model.index(index ,0) ,ImageModel.UrlRole) :""
                   anchors.centerIn: parent
                }
            }
        }

        Row {
            id: row
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: button.top
            anchors.bottomMargin: 10

            JKButton {
//                contentItem: Item{
//                    Image{
//                        source: "Images/Fit.tif"
//                        anchors.centerIn: parent
//                    }
//                }
                onClicked: {
                    requestImage(0 ,angle)
                }
                enabled: weight !== 0
            }

            JKButton {
                contentItem: Item{
                    Image{
                        source: "Images/ZoomIn.tif"
                        anchors.centerIn: parent
                    }
                }
                onClicked: requestImage(++weight ,angle)
                enabled: (image.width * (1.0 + weight / 10.0) < sourceSize.width)
                && (image.height * (1.0 + weight / 10.0) < sourceSize.height)
            }

            JKButton {
                contentItem: Item{
                    Image{
                        source: "Images/ZoomOut.tif"
                        anchors.centerIn: parent
                    }
                }
                onClicked: requestImage(--weight ,angle)
                enabled: weight >= -4
            }

            JKButton {
                contentItem: Item{
                    Image{
                        source: "Images/Turn.tif"
                        anchors.centerIn: parent
                    }
                }
                onClicked: requestImage(weight ,++angle)
            }
        }
        Button {
            id: button
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            background: BorderImage {
                id: bg
                source:
                    button.down?"Images/Btn_Gray_Pressed.png":"Images/Btn_Gray.png"
                opacity: button.down ?0.5 :1.0

                border.left: 17; border.top: 17
                border.right: 17; border.bottom: 17
            }
            contentItem: Item{
                Text{
                    text: qsTr("Return")
                    color: button.hovered ?button.down ?"white" :"green" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.centerIn: parent
                }
            }
            onClicked: {
                if(angle % 4 !== 0){
                    jkInterface.model.saveRotatoedImage(index ,angle)
                }
                close()
            }
        }
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
