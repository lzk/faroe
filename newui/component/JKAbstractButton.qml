import QtQuick 2.7

Item{
    id: root
    signal clicked
    signal doubleClicked
    property alias mouseArea: mouseArea
    property alias dashRectange: dashRectange
    activeFocusOnTab: true
    Keys.onEnterPressed: clicked()
    Keys.onReturnPressed: clicked()

    JKDashRectange{
        id:dashRectange
        anchors.fill: parent
        anchors.margins: 2
        radius: 0
        visible: root.activeFocus
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
