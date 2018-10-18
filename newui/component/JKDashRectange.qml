import QtQuick 2.0

Item {
    id:root
    property color color: "black"//"#FF0E5686"
    property string lineCap: "square"
    property real lineWidth: 1
    property real radius: 5
    property var dashlist: [1,2]

    Canvas{
        id:canvas
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = canvas.getContext('2d')
            ctx.save()
            ctx.lineCap = root.lineCap
            ctx.strokeStyle = root.color
            ctx.lineWidth = root.lineWidth

            var pi = Math.PI
            var du = pi / 180
            var start=dashlist[0]
            var end=dashlist[1]
            var count=start+end
            var origin


            ctx.beginPath()
            drawDashLine(ctx ,[radius,0] ,[width - radius,0] ,dashlist)
            ctx.stroke()
            ctx.beginPath()
            drawDashLine(ctx ,[width ,radius] ,[width ,height - radius] ,dashlist)
            ctx.stroke()
            ctx.beginPath()
            drawDashLine(ctx ,[width - radius,height] ,[radius,height] ,dashlist)
            ctx.stroke()
            ctx.beginPath()
            drawDashLine(ctx ,[0,radius] ,[0,height - radius] ,dashlist)

            ctx.stroke()
//            ctx.closePath()
            if(radius > 0){
                ctx.beginPath()
                ctx.arc(radius ,radius ,radius ,210 * du,220 * du)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(width - radius ,radius ,radius ,320 * du ,330 * du)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(width - radius ,height - radius ,radius ,40 * du ,50 * du)
                ctx.stroke()
                ctx.beginPath()
                ctx.arc(radius ,height - radius ,radius ,130 * du ,140 * du)
                ctx.stroke()
            }

    //            ctx.fillRect(0 ,0 ,canvas.width ,1)
    //            ctx.fillRect(0 ,0 ,1 ,canvas.height)
    //            ctx.fillRect(canvas.width - 1 ,0 ,canvas.width ,canvas.height)
    //            ctx.fillRect(0 ,canvas.height - 1 ,canvas.width ,canvas.height)

            ctx.restore()
        }

        function drawDashLine(ctx ,point1 ,point2 ,dashList){
            var x1 = point1[0]
            var y1 = point1[1]
            var x2 = point2[0]
            var y2 = point2[1]
            var step = dashList[0] + dashList[1]

            var x = x2 - x1
            var y = y2 - y1
            var count = Math.floor(Math.sqrt(x * x + y * y) / step);
            var xv = x / count;
            var yv = y / count;
            for (var i = 0; i < count; i ++) {
                if (i % 2 === 0) {
                    ctx.moveTo(x1, y1);
                } else {
                    ctx.lineTo(x1, y1);
                }
              x1 += xv;
              y1 += yv;
            }
            ctx.lineTo(x2, y2);
            }
    }
}
