const chalk = require("chalk");
const readline = require('readline');
const fs = require('fs-extra');
const path = require('path');
const osHomedir = require('os-homedir');
const shell = require('shelljs');
const _ = require('lodash');
const hl = require('./hl');

function checkCommand(cmds, line) {
    for (let key in cmds) {
        const func = cmds[key];
        if (/\|/.test(key)) {
            key = key.split('|');
            key = new RegExp(`^(${key.join('|')})($|\\s+)`);
        } else if (!(key instanceof RegExp)) {
            key = new RegExp(`^${key}($|\\s+)`);
        }
        if (key.test(line)) {
            return { key, func };
        }
    }
}
function checkHistory(cmds, options, line) {
    return checkCommand(_.omit(cmds, options.history || []), line);
}
module.exports = (cmds, options = {}) => {
    let lastLine;
    const historyPath = path.join(osHomedir(), `.xn_${options.prompt || 'common'}_history`);
    const deleteLeft = readline.Interface.prototype._deleteLeft;
    const insertString = readline.Interface.prototype._insertString;
    const addHistory = readline.Interface.prototype._addHistory;
    const newLine = readline.Interface.prototype._line;
    const ttyWrite = readline.Interface.prototype._ttyWrite;

    readline.Interface.prototype._deleteLeft = function() {
        deleteLeft.call(this);
        lastLine = this.line;
        this.historyIndex = -1;
    };
    readline.Interface.prototype._insertString = function(c) {
        insertString.call(this, c);
        lastLine = this.line;
        this.historyIndex = -1;
    };
    readline.Interface.prototype._addHistory = function() {
        const line = this.line.trim();
        if (checkHistory(cmds, options, line)) {
            return line;
        }
        return addHistory.call(this);
    };
    readline.Interface.prototype._line = function() {
        lastLine = undefined;
        newLine.call(this);
    };
    readline.Interface.prototype._ttyWrite = function(s, key = {}) {
        if (!(key.ctrl || key.meta)) {
            if (key.name === 'up' || key.name === 'down') {
                const history = this.history;
                if (lastLine) {
                    const reg = new RegExp(`^${lastLine}`);
                    this.history = history.filter(o => reg.test(o));
                }
                if (key.name === 'up') {
                    this._historyPrev();
                } else if (key.name === 'down') {
                    this._historyNext();
                }
                this.history = history;
                return;
            }
        }
        ttyWrite.call(this, s, key);
    };
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
        completer: options.completer,
        removeHistoryDuplicates: true,
        historySize: 10000,
    });
    const _prompt = options.prompt;
    delete options.prompt;
    rl.setPrompt( _prompt ? `${_prompt} > ` : '> ');
    fs.createFileSync(historyPath);
    rl.history = fs.readFileSync(historyPath, 'utf8').toString().split('\n').filter(o => o && o.trim());
    rl.on('SIGINT', () => {
        rl.clearLine();
        rl.prompt(true);
    });
    rl.on('close', () => {
        fs.writeFileSync(historyPath, rl.history.join('\n'));
        console.log('');
        console.log('bye');
        process.exit(0);
    });
    const cmd = {
        print (msg = '', color, follow) {
            if (follow) {
                this.color = color;
            }
            if (color) {
                this.follow = follow;
            }
            if (!color && this.follow) {
                color = this.color;
            }
            process.stdout.clearLine();
            process.stdout.cursorTo(0);
            msg = chalk[color] && chalk[color](msg) || msg;
            console.log(msg);
        },
        showJson (obj, pretty) {
            const msg = pretty ? JSON.stringify(obj, null, 2) : JSON.stringify(obj);
            this.code(msg, 'json');
            rl.prompt(true);
        },
        code (text, lang, hsBg) {
            process.stdout.clearLine();
            process.stdout.cursorTo(0);
            console.log(hl(text, process.platform === 'win32' ? 'railscasts' : 'github', lang, hsBg));
            rl.prompt(true);
        },
        log (msg) {
            this.print(msg);
            rl.prompt(true);
        },
        prompt () {
            rl.prompt(true);
        },
        question (msg, callback) {
            rl.question(chalk['blue'](msg), callback);
        },
        verbose (msg) {
            this.print(msg, 'gray');
            rl.prompt(true);
        },
        success (msg) {
            this.print(msg, 'green');
            rl.prompt(true);
        },
        error (error) {
            this.print(error, 'red');
            rl.prompt(true);
        },
        fatal (error) {
            this.error(error);
            process.exit();
        },
        close() {
            rl.close();
        },
        clear() {
            shell.exec('printf "\\33c\\e[3J"');
            rl.prompt(true);
        },
        dropHistory() {
            rl.history = _.drop(rl.history);
        },
        showHistory(n = 10) {
            const _history = _.slice(rl.history, 0, n).map((o, i) => `${i+1}: ${o}`);
            this.print(_history.join('\n'), 'blue');
            rl.prompt(true);
        },
        useHistory(n = 1) {
            const line = rl.history[+n-1];
            if (line && line.length) {
                rl.line = line;
                rl.cursor = line.length;
                rl._refreshLine();
            }
        },
    };
    rl.on('line', line => {
        line = line.trim();
        const ret = checkCommand(cmds, line);
        if (ret) {
            return ret.func.call(cmd, line.replace(ret.key, '').trim()||undefined, options);
        }
        if (cmds['default']) {
            cmds['default'].call(cmd, line, options);
        } else {
            cmd.error('invalid command!');
        }
    });
    cmd.prompt();

    return cmd;
};
