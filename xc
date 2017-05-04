#!/usr/bin/env node

var args = process.argv.slice(2);
try {
    var params = args.join('');
    var res=eval(params.replace('[', '(').replace(']', ')'));
    console.log(res);
} catch (e) {
    console.log('长度:'+params.length);
}


