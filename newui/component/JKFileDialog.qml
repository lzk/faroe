import QtQuick 2.0
import QtQuick.Dialogs 1.2
import "../ScanData.js" as JSData

FileDialog {
    id: fileDialog
    title: qsTr("Save As")
    folder: shortcuts.pictures
    nameFilters: JSData.constFileDialogSaveFileType()
    selectExisting:false
    property var dialog
}
