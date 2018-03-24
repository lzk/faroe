import QtQuick 2.7

Item{
    id: root
    signal clicked
    signal doubleClicked
    property alias mouseArea: mouseArea
    property alias dashRectange: dashRectange
    activeFocusOnTab: true

    JKDashRectange{
        id: dashRectange
        color: "black"   //"Transparent"
        anchors.fill: parent
        anchors.margins: -1
        visible: false //root.activeFocus
    }

    MouseArea {
        id:mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: {
            root.clicked()
            }
        onDoubleClicked: {
            root.doubleClicked()
        }
    }
}
