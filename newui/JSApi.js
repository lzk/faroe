.pragma library

function deepCopy(source, target) {

    if (typeof target !== 'object') {
        target = {}
    }

    var c = target;
    for (var i in source) {
        if (typeof source[i] === 'object') {
          c[i] = (source[i].constructor === Array) ? [] : {};
          deepCopy(source[i], c[i]);
        } else {
           c[i] = source[i];
        }
    }
    return c;
}
