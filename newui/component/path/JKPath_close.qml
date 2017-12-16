import QtQuick 2.0

Item {

    property alias color: canvas.strokeColor
    property alias lineWidth: canvas.lineWidth
    Canvas{
        id:canvas
        anchors.fill: parent
        anchors.margins: 0.25*parent.width
        antialiasing: true
        property color strokeColor: "#FF0E5686"
        property string lineCap: "round"
        property real lineWidth: 3

        onPaint: {
            var ctx = canvas.getContext('2d')
            ctx.save()
            ctx.clearRect(0 ,0 ,canvas.width ,canvas.height)

            ctx.lineCap = canvas.lineCap
            ctx.strokeStyle = canvas.strokeColor
            ctx.lineWidth = canvas.lineWidth
            var lw = 0
            var lh = 0
            var hw = canvas.width
            var hh = canvas.height
            ctx.beginPath()
            ctx.moveTo(lw ,lh)
            ctx.lineTo(hw ,hh)
            ctx.moveTo(hw ,lh)
            ctx.lineTo(lw ,hh)
            ctx.stroke()
            ctx.closePath()
            ctx.restore()
        }
    }
}
