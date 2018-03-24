import QtQuick 2.0
import "../JSApi.js" as JSApi

JKDialog{
    id: root
    property var setting
    property var settingEffective

    function sourceTheSetting(){
        JSApi.deepCopy(setting ,settingEffective)
    }

    function update(){
    }

    function initWithSetting(setting){
        settingEffective = setting
        root.setting = JSApi.deepCopy(setting)
        update()
    }
}
