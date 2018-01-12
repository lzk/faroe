.pragma library

function deepCopy(p, a) {
    var c = a || {};
    for (var i in p) {
        if (typeof p[i] === 'object') {
          c[i] = (p[i].constructor === Array) ? [] : {};
          deepCopy(p[i], c[i]);
        } else {
           c[i] = p[i];
        }
      }
    return c;
}

function copy(target ,source){
    for(var i in source){
        target[i] = source[i]
    }
}
