import QtQuick 2.0

QtObject {
    property string sid
    property string title
    property bool canEdit: false
    property string name
    property var scanSetting:
    ScanSetting{
    }
}
