import QtQuick 2.0

Image {
    source: "Images/busyRefresh.tif";
    NumberAnimation on rotation { running: true; from: 0; to: 360; loops: Animation.Infinite; duration: 1200 }
}
