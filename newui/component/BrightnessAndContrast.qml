import QtQuick 2.0
import QtGraphicalEffects 1.0

Image {
    id:image
    signal completed(string filename)
    sourceSize: undefined
    property alias brightness: bc.brightness
    property alias contrast: bc.contrast
    layer.enabled: true
    layer.effect: BrightnessContrast{
        id:bc
        source: image
    }
    function save(filename ,brightness ,contrast){
        image.source = "file:///" + filename
        image.brightness = (brightness - 50) * 0.02
        image.contrast = (contrast - 50) * 0.02
        image.grabToImage(function(result){
            result.saveToFile(image.source)
            completed(image.source)
        }
        ,image.sourceSize)
    }
}
