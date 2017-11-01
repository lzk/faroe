import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQml.Models 2.2
import "../component" as JKComponent
import com.liteon.JKInterface 1.0
JKComponent.JKDialog {
    id: root

    property int index:-1
    ListView{
        parent: container
        anchors.fill: parent
        clip: true
        currentIndex:root.index
        interactive: false
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        transitions: [
            Transition {
            }
        ]
        model: jkInterface.model
        delegate: ScrollView{
            id:scrollView
            clip: true
            ScrollBar.vertical.policy: contentHeight > availableHeight ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: contentWidth > availableWidth ?ScrollBar.AlwaysOn :ScrollBar.AlwaysOff
            width: ListView.view.width
            height: ListView.view.height
            Item{
                anchors.centerIn: parent
                implicitWidth:{image.width > scrollView.availableWidth ?image.width :scrollView.availableWidth}
                implicitHeight: {image.height > scrollView.availableHeight ?image.height :scrollView.availableHeight}
                Image{
                    id:image
                    anchors.centerIn: parent
                    source: "file:///" + url
                }
            }
        }
    }

}
