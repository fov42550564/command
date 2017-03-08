#!/usr/bin/env node

var args = process.argv.slice(2);
var res=eval(args.join('').replace('[', '(').replace(']', ')'));

console.log(res);

