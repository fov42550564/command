#!/usr/bin/env node

var args = process.argv.slice(2);
try {
    var params = args.join('');
    params = params.replace(/\[/g, '(').replace(/]/g, ')');
    params = params.replace(/{/g, '(').replace(/}/g, ')');
    params = params.replace('m.', 'Math.')
    console.log(eval(params));
} catch (e) {
    console.log('长度:'+params.length);
}


