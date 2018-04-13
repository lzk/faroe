import QtQuick 2.0
import QtQuick.Controls 2.2

CheckBox {
    id:control
    indicator: Image{
        anchors.fill: control
//        source: checked ?enabled ?"qrc:/Images/CheckBox_Open.png"
//                                 :"qrc:/Images/checkbox_ disable.png"
//                        :enabled ?"qrc:/Images/CheckBox_Close.png"
//                                :"qrc:/Images/CheckBox_Disable.png"
        source: checked ?"qrc:/Images/CheckBox_Open.png"
                        :"qrc:/Images/CheckBox_Close.png"
        opacity:  enabled ?1
                          :0.3
    }
}
