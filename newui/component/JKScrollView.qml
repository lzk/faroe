import QtQuick 2.0
import QtQuick.Controls 2.2

ScrollView {
    id:control
    focus: true
    contentItem.clip: true
    property alias image: image
    topPadding: 10
    bottomPadding: 15
    leftPadding: 15
    rightPadding: 30

    ScrollBar.vertical.policy: image.height > availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
    ScrollBar.horizontal.policy: image.width > availableWidth ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff

    contentWidth: image.width > availableWidth ?image.width :availableWidth
    contentHeight: image.height > availableHeight ?image.height :availableHeight

    Image{
       id:image
       asynchronous : true
       cache: false
       fillMode: Image.PreserveAspectFit
       width: control.availableWidth
       height: control.availableHeight
       anchors.centerIn: parent
    }


}
