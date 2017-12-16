import QtQuick 2.0
import "component"
import QtQuick.Controls 2.2

Item {
    id: root
    width: 750
    height: 533

    ListModel{
        id:deviceList
        ListElement{name:"USBDEVICE0" ;status:false}
        ListElement{name:"USBDEVICE0" ;status:false}
        ListElement{name:"USBDEVICE0" ;status:true}
        ListElement{name:"123.456.789.123" ;status:false}
        ListElement{name:"USBDEVICE0" ;status:false}
        ListElement{name:"USBDEVICE0" ;status:false}
        ListElement{name:"123.456.789.123" ;status:false}
        ListElement{name:"USBDEVICE0" ;status:false}
    }

    JKImageButton {
        id: button_back
        width: 55
        height: 55
        x:5
        y:10
        source_disable: "qrc:/Images/back_disable.png"
        source_normal: "qrc:/Images/back_normal.png"
        source_press: "qrc:/Images/back_press.png"
        onClicked: root.StackView.view.pop()
    }

    Column {
        id: column
        x:100
        y:50
        width: root.width - 200
        height: root.height -50
        spacing: 20

        ScrollView{
            id:scrollView
            width: 500
            height:parent.height - 93
            anchors.horizontalCenter: parent.horizontalCenter
            background: Rectangle{
                color:"#FFF4F4F4"
                border.color: "black"
                opacity:0.3
            }
            activeFocusOnTab: false

            ListView{
                id:listview
                anchors.fill: parent
                anchors.margins: 2
                model: deviceList
                clip: true
                enabled: listview.model.count > 0
                spacing:2
                header:Rectangle{
                    height: 2
                    color: "transparent"
                }

                delegate: JKAbstractButton {
                    id:wrapper
                    height:76
                    width: ListView.view.width
                    activeFocusOnTab: ListView.isCurrentItem
                    dashRectange.z: 2
                    Rectangle{
                        anchors.fill: parent
                        anchors.margins: 1
                        z:1
                        opacity: wrapper.ListView.isCurrentItem || mouseArea.containsMouse ?0.5 :1
                        border.color: wrapper.ListView.isCurrentItem ? "green"
                                                             :"Transparent"
                        border.width: 2
                        color: mouseArea.containsMouse ? "LightSlateGray"
                                                       : "LightSteelBlue"

                        Text {
                            text: name
                            x:50
                            font.bold: true
                            font.pixelSize: 16
                            color:"DarkSlateGray"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: -2
                        }
                        Text {
                            text:qsTr("Connected")
                            x:200
                            visible: status
                            color:"green"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    MouseArea{
                        id:mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: wrapper.ListView.view.currentIndex = index
                    }
                    onFocusChanged:
                        activeFocusOnTab = focus
                }
            }
        }
        JKTextButton{
            id:button_connect
            width: 150
            height: 35
            anchors.horizontalCenter: parent.horizontalCenter
            text.text: qsTr("Connect")
            enabled: listview.model.count > 0
        }
    }

    JKImageButton {
        id: button_refresh
        x:parent.width - 100
        y:50
        width: 70
        height: 70
        source_normal: "qrc:/Images/clockwise-arrow.png"
        source_press: "qrc:/Images/clockwise-arrow_press.png"
    }

    Loader{
        id:dialogs
    }

    Connections{
        target: button_refresh
        onClicked:{
            openRefreshDialog()
        }
    }

    function openRefreshDialog(){
        var dialog
        dialogs.source = "component/JKMessageBox_refresh.qml"
//        dialogs.source = "component/JKMessageBox_warning.qml"
        dialog = dialogs.item
//        dialog.message.text = qsTr("haha")
//        dialog.showImage = true
        dialog.open()
    }

    function closeRefreshDialog(){
        dialogs.item.close()
        dialogs.source = ""
    }
    
}
