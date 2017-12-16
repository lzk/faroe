import QtQuick 2.0
import ".."

JKAbstractButton{
    property color color: mouseArea.containsPress ? "#FF497C1F":mouseArea.containsMouse ? "#FF6BDE0A":"#FF4BA400"
    Canvas{
        id:canvas
        anchors.fill: parent
        antialiasing: true
        property color fillColor: parent.color

        onPaint: {
            var ctx = getContext('2d')
            ctx.save()
            ctx.clearRect(0 ,0 ,width ,height)

            ctx.fillStyle = fillColor
            var x1 = 0
            var y1 = 0.5*height
            var x2 = width
            var y2 = 0
            var x3 = width
            var y3 = height
            ctx.beginPath()
            ctx.moveTo(x1 ,y1)
            ctx.lineTo(x2 ,y2)
            ctx.lineTo(x3 ,y3)
            ctx.fill()
            ctx.closePath()
            ctx.restore()
        }
    }
    Connections{
        target: mouseArea
        onContainsMouseChanged: canvas.requestPaint()
        onContainsPressChanged: canvas.requestPaint()
    }

}
