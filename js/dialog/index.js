var _ = require('lodash');
var colors = require('colors');
var printf = require('printf');
var fs = require('fs');
var readline = require('readline');
var program = require('commander');

colors.setTheme({
    BORDER_COLOR: 'green',
    TITLE_COLOR: 'red',
    INDEX_COLOR: 'magenta',
    STRING_COLOR: 'blue',
    INPUTBOX_COLOR: 'blue',
    ERROR_COLOR: 'red',
    WARN_COLOR: 'yellow',
    DEBUG_COLOR: 'green',
});

class Dialog {
    constructor (simple) {
        this.HTAG = simple ? '-' : '\u2500';
        this.VTAG = simple ? '|' : '\u2502';
        this.TLTAG = simple ? '+' : '\u250c';
        this.TRTAG = simple ? '+' : '\u2510';
        this.MLTAG = simple ? '|' : '\u251c';
        this.MRTAG = simple ? '|' : '\u2524';
        this.BLTAG = simple ? '+' : '\u2514';
        this.BRTAG = simple ? '+' : '\u2518';

        this.DIALOG_WIDTH = 80;
        this.MSGBOX_WIDTH = 30;
    }
    error (args) {
        var ret = printf.apply(this, arguments);
        console.log(ret.ERROR_COLOR);
    }
    warn (args) {
        var ret = printf.apply(this, arguments);
        console.log(ret.WARN_COLOR);
    }
    debug (args) {
        var ret = printf.apply(this, arguments);
        console.log(ret.DEBUG_COLOR);
    }
    pauseOff() {
        process.stdin.pause();
    }
    pause () {
        return new Promise(async (resolve) => {
            console.log('Press any key to continue...');
            process.stdin.setRawMode(true);
            process.stdin.resume();
            process.stdin.on('data', ()=>{
                resolve();
            });
        });
    }
    isAsc(code) {
        return code >= 0 && code <= 256;
    }
    isAscWord(code) {
        return (code >= 48 && code <= 57) || (code >= 65 && code <= 90) || (code >= 97 && code <= 122) || code === 95; //数字、字母、下划线
    }
    getRealLength(text) {
        let realLength = 0, len = text.length, charCode = -1;
        for (let i = 0; i < len; i++) {
            charCode = text.charCodeAt(i);
            if (this.isAsc(charCode)) {
                realLength += 1;
            } else {
                realLength += 2;
            }
        }
        return realLength;
    }
    getRealCutText(text, n) {
        let realLength = this.getRealLength(text);
        return text+_.repeat(' ', n-realLength);
    }
    cutLimitText (list, text, n) {
        let realLength = 0, len = text.length, preLen = -1, charCode = -1, needCut = false;
        for (let i = 0; i < len; i++) {
            charCode = text.charCodeAt(i);
            if (this.isAsc(charCode)) {
                realLength += 1;
            } else {
                realLength += 2;
            }
            if (preLen === -1 && realLength >= n) {
                preLen = i + 1;
            } else if (realLength > n) {
                needCut = true;
                break;
            }
        }
        if (needCut) {
            let cutText = text.substr(0, preLen);
            let lastCode = cutText.charCodeAt(cutText.length-1);
            let nextCode = text.charCodeAt(preLen);

            if (this.isAscWord(lastCode) && this.isAscWord(nextCode)) {
                for (var j = 0; j < cutText.length; j++) {
                    if (!this.isAscWord(cutText.charCodeAt(cutText.length-1-j))) {
                        break;
                    }
                }
                if (j < cutText.length) {
                    preLen -= j;
                }
            }
            list.push(this.getRealCutText(text.substr(0, preLen), n));
            text = text.substr(preLen);
            if (text) {
                this.cutLimitText(list, text, n);
            }
        } else {
            list.push(this.getRealCutText(text, n));
        }
    }
    getBoder (width, layer = 'top', hasStart = true, hasEnd = true) {
        var headChar = (hasStart) ? (layer==='top' ? this.TLTAG : layer==='middle' ? this.MLTAG : this.BLTAG ) : this.HTAG;
        var tailChar = (hasEnd) ? (layer==='top' ? this.TRTAG : layer==='middle' ? this.MRTAG : this.BRTAG ) : this.HTAG;
        var str = headChar;
        for (var i=1; i<width; i++) {
            str += (i === width - 1) ? tailChar : this.HTAG;
        }
        return str;
    }
    showTopLineBoder (width) {
        width = width || this.DIALOG_WIDTH;
        console.log(this.getBoder(width).BORDER_COLOR);
    }
    showMiddleLineBoder (width) {
        width = width || this.DIALOG_WIDTH;
        console.log(this.getBoder(width, 'middle').BORDER_COLOR);
    }
    showBottomLineBoder (width) {
        width = width || this.DIALOG_WIDTH;
        console.log(this.getBoder(width, 'bottom').BORDER_COLOR);
    }
    showBoxTitle (title, width) {
        width = width || this.DIALOG_WIDTH;

        var len = width - this.getRealLength(title) - 2;
        var flen = parseInt(len / 2);
        var flen1 = flen + (len & 1);

        console.log(this.getBoder(flen, 'top', true, false).BORDER_COLOR +' '+ title.TITLE_COLOR +' '+ this.getBoder(flen1, 'top', false, true).BORDER_COLOR);
    }
    showBoxDiscription (disp, width) {
        width = width || this.DIALOG_WIDTH;
        this.showBoxString({head: '[Usage]:', text: disp}, width);
    }
    showBoxString (item, width) {
        var {head = '', text = ''} = item;
        text = ' ' + text;
        width = width || this.DIALOG_WIDTH;
        var in_width = width - 3;
        var list = [];
        this.cutLimitText(list, head+text, in_width);
        for (var i in list) {
            if (i == 0) {
                console.log(this.VTAG.BORDER_COLOR + ' ' + head.INDEX_COLOR + list[i].replace(head, '').STRING_COLOR + this.VTAG.BORDER_COLOR);
            } else {
                console.log(this.VTAG.BORDER_COLOR + ' ' + list[i].STRING_COLOR + this.VTAG.BORDER_COLOR);
            }
        }
    }
    msgbox (title, list, width) {
        width = width || this.DIALOG_WIDTH;
        var len = list.length;
        this.showBoxTitle(title, width);
        for (var i = 0; i < len; i++) {
            this.showBoxString(list[i], width);
        }
        this.showBottomLineBoder(width);
    }
    inputbox (info, defaultValue) {
        return new Promise(async (resolve) => {
            info = info.INPUTBOX_COLOR;
            if (defaultValue) {
                info += ('[default:' + defaultValue + ']:').INDEX_COLOR;
            }
            var rl = readline.createInterface({
                input: process.stdin,
                output: process.stdout,
            });
            rl.question(info, (ret) => {
                ret = ret.trim() || defaultValue;
                rl.close();
                resolve(ret);
            });
        });
    }
    listbox (title, disp, list, width) {
        width = width || this.DIALOG_WIDTH;
        var len = list.length;
        this.showBoxTitle(title, width);
        this.showBoxDiscription(disp, width);
        this.showMiddleLineBoder(width);
        for (var i = 0; i < len; i++) {
            this.showBoxString({head: i + ':', text: list[i]}, width);
        }
        this.showBottomLineBoder(width);
    }
    async radiobox (title, disp, list, callback, isUpdate, width) {
        width = width || this.DIALOG_WIDTH;
        var len = list.length;
        while (true) {
            if (len == 0) {
                this.msgbox('complete', [{text: 'have done all'}], this.MSGBOX_WIDTH);
                return;
            }
            this.listbox(title, disp, list, width);
            var ret = await this.inputbox('please input need ' + title.TITLE_COLOR + ' index,' + 'exit(q)'.INDEX_COLOR);
            var index = ret.trim();
            if (!index.length) {
                this.error('null is not allowed');
                continue;
            }
            if (index == 'q') {
                break;
            }
            if (isNaN(index)) {
                this.error('must input a number');
                continue;
            }
            if (index < 0 || index >= len) {
                this.error('the select number is out of range');
                continue;
            }
            callback(list[index]);
            if (isUpdate) {
                list.splice(index, 1);
                len--;
            }
            await this.pause();
        }
    }
    async checkbox (title, disp, list, callback, singleCallback, isUpdate, width) {
        width = width || this.DIALOG_WIDTH;
        var len = list.length;
        var isAll = false;

        if (len == 0) {
            this.error("list is null, can't get list in this case");
            return;
        }
        this.listbox(title, disp, list, width);
        var ret = await this.inputbox('please input need ' + title.TITLE_COLOR + ' index' + '([-]1 2 3|1-3 5-7|all)'.INDEX_COLOR);
        var input = ret.trim();
        if (!input.length) {
            this.error('null is not allowed');
            return;
        }
        if (input == 'q') {
            return;
        }
        var isFilter = input.charCodeAt(0) == 45;
        if (isFilter) {
            input = input.substring(1).trim();
        }
        var target_indexs = [];
        var all = [];
        var id = 0;
        for (var i = 0; i < len; i++) {
            all[i] = i;
        }
        if (input == 'all') {
            isAll = true;
            target_indexs = all;
        } else {
            var indexs = input.split(/\s+/);
            var indexs_len = indexs.length;
            for (var i = 0; i < indexs_len; i++) {
                var index = indexs[i];
                var isRange = index.match(/^[^-]*-[^-]*$/);
                if (isRange) {
                    var ranges = index.split(/-/);
                    if (isNaN(ranges[0]) || isNaN(ranges[1])) {
                        this.error('must input a number');
                        return;
                    }
                    for (var j = parseInt(ranges[0]), m = parseInt(ranges[1]); j <= m; j++) {
                        target_indexs[id++] = j;
                    }
                } else {
                    if (isNaN(index)) {
                        this.error('must input a number');
                        return;
                    }
                    target_indexs[id++] = parseInt(index);
                }
            }
            target_indexs.sort();
            target_indexs = target_indexs.filter((value, index, self) => {
                return self.indexOf(value) === index;
            });
            var target_len = target_indexs.length;
            for (var i = 0; i < target_len; i++) {
                if (target_indexs[i] < 0 || target_indexs[i] > len) {
                    this.error('invalid input out range');
                    return;
                }
            }
            if (isFilter) {
                var tmp = [];
                var id = 0;
                var isIn = (arr, value) => {
                    var arrLen = arr.length;
                    for (var i = 0; i < arrLen; i++) {
                        if (arr[i] == value) {
                            return true;
                        }
                    }
                    return false;
                };
                for (var i = 0; i < len; i++) {
                    if (!isIn(target_indexs, all[i])) {
                        tmp[id++] = all[i];
                    }
                }
                target_indexs = tmp;
            }
        }
        target_len = target_indexs.length;
        if (singleCallback) {
            for (var i = 0; i < target_len; i++) {
                singleCallback(list[target_indexs[i]]);
            }
        }
        if (callback) {
            var targets = [];
            for (var i = 0; i < target_len; i++) {
                targets[i] = list[target_indexs[i]];
            }
            callback(targets);
        }
        if (isUpdate) {
            for (var i = target_len - 1; i >= 0; i--) {
                var del_id = target_indexs[i];
                list.splice(del_id, 1);
            }
        }
    }
    menuitembox (disp, items, width) {
        width = width || this.DIALOG_WIDTH;
        this.showBoxTitle('menu', width);
        this.showBoxString({text: disp}, width);
        this.showBoxString({text: 'please input follows to do your work'}, width);
        this.showMiddleLineBoder(width);

        for (var tag in items) {
            this.showBoxString({head: '[' + tag + ']: ', text: items[tag].disp}, width);
        }
        this.showBoxString({head: '[q]:', text: 'exit'}, width);
        this.showBottomLineBoder(width);
    }
    async menubox (disp, items, width) {
        width = width || this.DIALOG_WIDTH;
        while (true) {
            this.menuitembox(disp, items, width);
            while (true) {
                var ret = await this.inputbox('please input oper type:');
                ret = ret.trim();
                if (ret == 'q') {
                    return;
                }
                if (items[ret]) {
                    break;
                }
                this.error('you select oper not exist!');
            }
            var item = items[ret];
            item.callback(item);
            if (item.disp) {
                disp = item.disp;
            }
        }
    }
    commander () {
        var _self = this;
        program.optionHelp = function () {
            var ret = [];
            ret[0] = {head: 'Usage:', text: this.usage()};
            ret[1] = {};
            for (var i = 0; i < this.options.length; i++) {
                ret[i + 2] = {head: '   '+this.options[i].flags + ': ',  text: this.options[i].description};
            }
            ret[i + 2] = {head: '   -h, --help:',  text:'show help'};
            return ret;
        };
        program.commandHelp = function () {
            var ret = [];
            if (!this.commands.length) return ret;

            ret[0] = {head: 'Commands:'};
            ret[1] = {};
            for (var i = 0; i < this.commands.length; i++) {
                var cmd = this.commands[i];
                var args = cmd._args.map((arg) => arg.required ? '<' + arg.name + '>' : '[' + arg.name + ']').join(' ');
                ret[i + 2] = {head: '   '+cmd._name + (cmd.options.length ? '[options]' : '') + ' ' + args + ': ',  text: (cmd.description() ? cmd.description() : '')};
            }
            return ret;
        };
        program.helpInformation = function () {
            var name = this._name;
            var commandHelp = this.commandHelp();
            var optionHelp = this.optionHelp();
            var options = optionHelp;
            if (commandHelp.length) {
                options = options.concat({}, commandHelp);
            }
            return { name, options };
        };
        program.outputHelp = function () {
            var args = this.helpInformation();
            _self.msgbox(args.name, args.options);
        };
        return program;
    }
};

module.exports = new Dialog();
