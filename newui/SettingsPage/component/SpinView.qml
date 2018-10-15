import QtQuick 2.7
import QtQuick.Controls 2.2
import "../../component"

Item {
    id:root
    width: 190
    height: 65
    property alias slider: slider
    property alias input: input
    property alias text: input.text
    property alias value: slider.value
    property alias from: doubleValidator.bottom
    property alias to: doubleValidator.top
    property alias stepSize: slider.stepSize
    property alias decimals: doubleValidator.decimals
    property alias maximumLength: input.maximumLength
    property alias tooltip: tooltip

    DoubleValidator {
        id:doubleValidator
        bottom: 0
        top:  100
        notation:DoubleValidator.StandardNotation
        decimals: 0
    }

    Column {
        id: column
        anchors.fill: parent
        anchors.topMargin: 5
        spacing: 10

        Slider {
            id: slider
            width: 154
            height: 15
            from: root.from
            to: root.to
            stepSize: 1
            anchors.horizontalCenter: parent.horizontalCenter
            value: 50
//            onValueChanged: {
//                console.log("value changed")
//                root.value = value
//                value = Qt.binding(function(){return root.value})
//            }
        }

        Item {
            id: item1
            height: 40
            anchors.right: parent.right
            anchors.left: parent.left
            visible: decimals !== 0

            Text {
                id: text1
                color: "#707070"
                text: Math.floor(value) / 10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
            }

        }

        Item{
            height: 40
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            visible: !item1.visible
            JKTextButton{
                text: "—"
                fontSize: 15
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                autoRepeat: true
                onClicked: slider.decrease()
                enabled: root.value !== root.from
            }
            JKTextButton{
                text: "+"
                fontSize: 22
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                autoRepeat: true
                onClicked: slider.increase()
                enabled: root.value !== root.to
            }

            Rectangle{
                anchors.fill: input
                anchors.topMargin: -5
                anchors.bottomMargin: -5
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                color: "white"
                border.color: tooltip.visible ?"red":"transparent"
                border.width: 2
            }

            TextInput{
                id:input
                width: parent.width - 60
                anchors.centerIn: parent
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                font.pixelSize: 30
                color: "#707070"
                selectByMouse: true
                selectionColor: "lightblue"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                focus: true
                validator: IntValidator{
                    bottom: root.from
                    top:root.to
                }

                text: root.value
//                onCursorVisibleChanged: {
//                    console.log("cursor visible" ,cursorVisible)
//                    root.value = Number.fromLocaleString(Qt.locale(), text)
//                    text = Qt.binding(function(){return root.value})
//                }

//                onFocusChanged: {
//                    console.log("focus changed" ,focus)
//                    root.value = Number.fromLocaleString(Qt.locale(), text)
//                    text = Qt.binding(function(){return root.value})
//                }
                onActiveFocusChanged: {
                    root.value = Number.fromLocaleString(Qt.locale(), text)
                    text = Qt.binding(function(){return root.value})
                }

                onEditingFinished: {
                    console.log("editing finished:",text)
                    root.value = Number.fromLocaleString(Qt.locale(), text)
                    text = Qt.binding(function(){return root.value})

                }
                onTextChanged: {
                    console.log("text changed:" ,text)
                    root.value = Number.fromLocaleString(Qt.locale(), text)
                    console.log("value changed:" ,root.value)
                }
            }

            ToolTip{
                id:tooltip
                visible: input.activeFocus && text !== ""
                background: Rectangle{
                    color: "#C7EECE"
                }
                closePolicy:Popup.NoAutoClose
            }
        }

        //Qt5.9 SpinBox with DoubleValidator has issue:can input invalid text
        //use default IntValidator only
//        SpinBox{
//            id:control
//            visible: decimals === 0
//            height: 40
//            anchors.right: parent.right
//            anchors.left: parent.left
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10
//            editable: true
//            font.pixelSize: 30
//            from: root.from
//            to: root.to
//            stepSize: root.stepSize
//            value: root.value
//            onValueChanged:{
//                root.value = value
////                value = Qt.binding(function(){return root.value})
//            }

////            validator: doubleValidator

////            textFromValue: function(value, locale) {
////                if(decimals === 1)
////                    return Number(value / 10).toLocaleString(locale, 'f', decimals)
////                else
////                    return Number(value).toLocaleString(locale, 'f', decimals)
////            }
////            valueFromText: function(text, locale) {
////                if(decimals === 1)
////                    return Number.fromLocaleString(locale, text) * 10
////                else
////                    return Number.fromLocaleString(locale, text)
////            }

//            contentItem: TextInput {
//                id:input1
//                z: 2
//                anchors.centerIn: parent
//                text: control.textFromValue(control.value, control.locale)
//                font: control.font
//                color: "#707070"
//                selectByMouse: true
//                selectionColor: "lightblue"
//                horizontalAlignment: Qt.AlignHCenter
//                verticalAlignment: Qt.AlignVCenter

//                readOnly: !control.editable
//                validator: control.validator
//                inputMethodHints: Qt.ImhFormattedNumbersOnly
//                focus: true
//                onFocusChanged: {
//                    console.log("focus changed" ,focus)
//                }

//                onTextChanged: {
//                    console.log("text changed:" ,text)
////                    if(text.length > 0)
////                        root.value = control.valueFromText(text ,control.locale)
////                    text = text
////                    root.value = control.valueFromText(text ,control.locale)
//                }
//                onEditingFinished: {
//                    console.log("editing finished")
////                    text = Qt.binding(function(){return control.textFromValue(control.value ,control.locale)})
//                }
//            }

//            up.indicator:
//                JKTextButton{
//                    text: "+"
//                    x: control.mirrored ? 0 : parent.width - width
//                    anchors.verticalCenter: parent.verticalCenter
//                    width: 30
//                    height: 30
//                    onClicked: control.increase()
//                    autoRepeat: true
//                }

//            down.indicator:
//                JKTextButton{
//                x: control.mirrored ? parent.width - width : 0
//                anchors.verticalCenter: parent.verticalCenter
//                text: "-"
//                width: 30
//                height: 30
//                onClicked: control.decrease()
//                autoRepeat: true
//            }

//            background: Item{}
//        }
    }

}
