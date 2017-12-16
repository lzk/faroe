import QtQuick 2.7

JKAbstractButton {
    id: root
    width: 35
    height: 35

    property alias pathStyle:canvas.pathStyle

    Canvas{
        id:canvas
        anchors.fill: parent
        antialiasing: true
        property color borderColor: "#FFEEBA70"
        property color strokeColor: "#FF0E5686"
        property string lineCap: "round"
        property real lineWidth: 3
        property string  pathStyle

        onPaint: {
            var ctx = canvas.getContext('2d')
            ctx.save()
            ctx.clearRect(0 ,0 ,canvas.width ,canvas.height)
            ctx.strokeStyle = canvas.borderColor
            ctx.fillStyle = canvas.borderColor
            ctx.lineWidth = 2
            if(mouseArea.containsPress)
                ctx.fillRect(0 ,0 ,canvas.width ,canvas.height)
            else if(mouseArea.pressed || mouseArea.containsMouse){
                ctx.strokeRect(0 ,0 ,canvas.width ,canvas.height)
            }

            ctx.lineCap = canvas.lineCap
            ctx.strokeStyle = canvas.strokeColor
            ctx.lineWidth = canvas.lineWidth
            var lw = 0.3*canvas.width
            var lh = 0.3*canvas.height
            var hw = 0.7*canvas.width
            var hh = 0.7*canvas.height
            switch(root.pathStyle){
            case "minimize":
                ctx.beginPath()
                ctx.moveTo(lw ,hh)
                ctx.lineTo(hw ,hh)
                ctx.stroke()
                ctx.closePath()
                break;
            case "close":
                ctx.beginPath()
                ctx.moveTo(lw ,lh)
                ctx.lineTo(hw ,hh)
                ctx.moveTo(hw ,lh)
                ctx.lineTo(lw ,hh)
                ctx.stroke()
                ctx.closePath()
            }
            ctx.restore()
        }
    }

    Connections{
        target: mouseArea
        onContainsMouseChanged: canvas.requestPaint()
        onContainsPressChanged: canvas.requestPaint()
    }
}
