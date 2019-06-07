const colors = require('colors/safe');
const readline = require('readline');
const fs = require('fs-extra');
const path = require('path');
const osHomedir = require('os-homedir');

function checkCommand(cmds, line) {
    for (let key in cmds) {
        let reg = key;
        if (/\|/.test(reg)) {
            reg = key.split('|');
            reg = new RegExp(`^(${reg.join('|')})($|\\s+)`);
        } else if (!(key instanceof RegExp)) {
            reg = new RegExp(`^${reg}($|\\s+)`);
        }
        if (reg.test(line)) {
            return key;
        }
    }
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
        if (checkCommand(cmds, line)) {
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
        print (msg = '', color) {
            process.stdout.clearLine();
            process.stdout.cursorTo(0);
            if (color) {
                this.color = color;
            } else {
                color = this.color;
            }
            msg = colors[color] && colors[color](msg) || msg;
            console.log(msg);
        },
        showJson (obj, pretty) {
            const msg = pretty ? JSON.stringify(obj, null, 2) : JSON.stringify(obj);
            this.print(msg);
            this.prompt();
        },
        log (msg) {
            this.print(msg);
            this.prompt();
        },
        prompt () {
            rl.prompt(true);
        },
        question (msg, callback) {
            rl.question(colors['blue'](msg), callback);
        },
        success (msg) {
            this.print(msg, 'green');
            this.prompt();
        },
        error (error) {
            this.print(error, 'red');
            this.prompt();
        },
        fatal (error) {
            this.error(error);
            process.exit();
        },
        close() {
            rl.close();
        },
    };
    rl.on('line', line => {
        line = line.trim();
        const key = checkCommand(cmds, line);
        if (key) {
            return cmds[key].call(cmd, line.replace(key, ''), options);
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
