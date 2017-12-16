import QtQuick 2.0

Item {
    id:root
    property color color: "#FF0E5686"
    property string lineCap: "square"
    property real lineWidth: 1
    property alias radius: canvas.radius

    Canvas{
        id:canvas
        anchors.fill: parent
        antialiasing: true
        property real radius: 5
        property var dashlist: [2,6]

        onPaint: {
            var ctx = canvas.getContext('2d')
            ctx.save()
            ctx.lineCap = root.lineCap
            ctx.strokeStyle = root.color
            ctx.lineWidth = root.lineWidth

            var pi = Math.PI
            var du = pi / 8
            var start=dashlist[0]
            var end=dashlist[1]
            var count=start+end
            var origin
            ctx.beginPath()
//            if(radius > 0){

//            }

//            ctx.moveTo(radius,0)
//            for(var i=0 ;i < (width-radius)/count ;i++){
//                origin=i*count + radius
//                ctx.moveTo(origin+start ,0)
//                ctx.lineTo(origin + end ,0)
//            }
//    //        ctx.lineTo(width -radius ,0)
//            ctx.moveTo(width - radius + Math.sin(du)*radius ,radius - Math.cos(du)*radius)
//            ctx.arc(width - radius ,radius ,radius ,1.75*pi-du ,pi*1.75+du)
//    //            ctx.arcTo(width ,0 ,width ,radius ,radius)
//            ctx.moveTo(width ,radius)
//            ctx.lineTo(width ,height -radius)
//            ctx.arcTo(width ,height ,width -radius ,height ,radius)
//    //            ctx.moveTo(width -radius ,height)
//            ctx.lineTo(radius ,height)
//            ctx.arcTo(0, height ,0 ,height -radius ,radius)
//    //            ctx.moveTo(0 ,height -radius )
//            ctx.lineTo(0 ,radius)
//            ctx.arcTo(0 ,0 ,radius ,0 ,radius)
//            for(var i ;i < 4 ;i++){

//            }
            drawDashLine(ctx ,[0,0] ,[width,0] ,dashlist)
            drawDashLine(ctx ,[0,0] ,[0,height] ,dashlist)
            drawDashLine(ctx ,[width,0] ,[width,height] ,dashlist)
            drawDashLine(ctx ,[0,height] ,[width,height] ,dashlist)
//            drawDashRound(ctx ,[width/2 ,height/2] ,100 ,dashlist)
            ctx.stroke()
            ctx.closePath()

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

        function drawDashArc(ctx ,point1 ,point2 ,radius ,dashList){
            var x1 = point1[0]
            var y1 = point1[1]
            var x2 = point2[0]
            var y2 = point2[1]

        }

        function drawDashRound (ctx ,point, radius ,dashList){
            var x = point[0]
            var y = point[1]
            var step = dashList[0] + dashList[1]
            var count = Math.floor(360 / step);
            step = 5 / 180 * Math.PI * 2;
            for (var b = 0, e = step / 2; e <= 360; b += step, e += step) {
              ctx.arc(x, y, radius, b, e);
            }
        }

    }
}
