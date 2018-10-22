import QtQuick 2.7
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2

Button{
//JKAbstractButton{
    id: root
    property alias container: container

    Keys.onEnterPressed: clicked()
    Keys.onReturnPressed: clicked()
    activeFocusOnTab: true
    focusPolicy:Qt.TabFocus

    background:Item{}

    contentItem:
    Item{
        id:container
        anchors.fill: parent
        visible: false
    }

    DropShadow{
        id:shadow
        anchors.fill: container
        source: container
        visible: false
        color: "#FF9C9797"
    }

    FastBlur{
        id:blur
        source: shadow
        anchors.fill: shadow
    }

    states:[
        State {
            name: "normal"
//            when: root.enabled && !mouseArea.containsMouse
            when: root.enabled && !root.hovered
            PropertyChanges {
                target: blur
                scale: 1.0
                radius: 0
            }
            PropertyChanges {
                target: shadow
                samples: 0
                horizontalOffset:0
                verticalOffset: 0
            }
        },
        State {
            name: "pressed"
//            when: root.enabled && mouseArea.pressed
            when: root.enabled && root.pressed
            PropertyChanges {
                target: shadow
                samples: 11
                horizontalOffset:2
                verticalOffset: 2
            }
            PropertyChanges {
                target: blur
                scale: 1.02
            }
        },
        State {
            name: "disable"
            when: !root.enabled
            PropertyChanges {
                target: shadow
                samples: 0
                horizontalOffset:0
                verticalOffset: 0
            }
            PropertyChanges {
                target: blur
//                visible: false
                scale: 1.0
                radius: 0
            }
        },
        State {
            name: "enter"
//            when: root.enabled && mouseArea.containsMouse
            when: root.enabled && root.hovered
            PropertyChanges {
                target: shadow
                samples: 11
                horizontalOffset:1
                verticalOffset: 1
            }
            PropertyChanges {
                target: blur
//                visible: true
                scale: 1.04
                radius: 0.01
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "pressed"
            ParallelAnimation{
                NumberAnimation{
                    target: blur
                    duration: 100
                    properties: "scale ,radius"
                }
                NumberAnimation{
                    target: shadow
                    duration: 100
                    properties: "samples,horizontalOffset,verticalOffset"
                }
            }

        },
        Transition {
            from: "pressed"
            to: "enter"
            ParallelAnimation{
                NumberAnimation{
                    target: blur
                    duration: 100
                    properties: "scale ,radius"
                }
                NumberAnimation{
                    target: shadow
                    duration: 100
                    properties: "samples,horizontalOffset,verticalOffset"
                }
            }

        },
        Transition {
            from: "normal"
            to: "enter"
            ParallelAnimation{
                NumberAnimation{
                    target: blur
                    duration: 200
                    properties: "scale ,radius"
                }
                NumberAnimation{
                    target: shadow
                    duration: 200
                    properties: "samples,horizontalOffset,verticalOffset"
                }
            }

        },
        Transition {
            from: "enter"
            to: "normal"
            ParallelAnimation{
                NumberAnimation{
                    target: blur
                    duration: 200
                    properties: "scale ,radius"
                }
                NumberAnimation{
                    target: shadow
                    duration: 200
                    properties: "samples,horizontalOffset,verticalOffset"
                }
            }

        }
    ]
}

