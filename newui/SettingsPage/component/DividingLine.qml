import QtQuick 2.0

Item {
    height: 6
    Canvas{
        anchors.fill: parent
        antialiasing: true
        property color strokeColor: "lightgray"
        property real lineWidth: 2

        onPaint: {
            var ctx = getContext('2d')
            ctx.save()
            ctx.clearRect(0 ,0 ,width ,height)

            ctx.strokeStyle = strokeColor
            ctx.lineWidth = lineWidth
            ctx.beginPath()
            ctx.moveTo(0 ,4)
            ctx.lineTo(width ,2)
            ctx.stroke()
            ctx.closePath()
            ctx.restore()
        }

    }
}
