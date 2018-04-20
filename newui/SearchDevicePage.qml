import QtQuick 2.0
import "component"
import QtQuick.Controls 2.2

Item {
    id: root
    width: 750
    height: 533

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
                model: scanData.model_deviceList
                clip: true
                enabled: count > 0
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
                            text: modelData
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
                            visible: modelData === scanData.currentDevice
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
            text: qsTr("Connect")
            enabled: listview.count > 0
            onClicked: connectToDevice(listview.currentIndex)
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
        source_disable: "qrc:/Images/clockwise-arrow.png"
    }

    Connections{
        target: button_refresh
        onClicked:refresh()
    }

    Connections{
        target: jkInterface
        onSearchComplete:{
            closeRefreshDialog()
            if(listview.count >= 0)
                connectToDevice(0)
        }
        onDeviceConnectCompleted:{
            button_connect.enabled = true
        }
    }

    Component.onCompleted: refresh()

    property var dialog
    function openRefreshDialog(){
        dialog = openDialog("component/JKMessageBox_refresh.qml" ,{"showCancel":true} ,function(dialog){
            dialog.cancelClick.connect(jkInterface.cancelSearch)
        })
    }

    function closeRefreshDialog(){
        dialog.close()
        button_refresh.enabled = true
    }
    
    function refresh(){
        button_refresh.enabled = false
        scanData.model_deviceList = null
        jkInterface.searchDeviceList()
        openRefreshDialog()
    }

    function connectToDevice(index){
        if(index < 0)
            return
        jkInterface.connectDevice(index)
        button_connect.enabled = false
    }
}
