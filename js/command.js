const colors = require('colors/safe');
const readline = require('readline');
const fs = require('fs-extra');
const path = require('path');
const osHomedir = require('os-homedir');

module.exports = (cmds, options = {}) => {
    const historyPath = path.join(osHomedir(), `.xn_${options.prompt || 'common'}_history`);
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
    const history = fs.readFileSync(historyPath, 'utf8').toString().split('\n').slice(0, -1).reverse();
    const _addHistory = rl._addHistory;
    rl._addHistory = () => {
        const last = rl.history[0];
        const line = _addHistory.call(rl);
        if (line.length > 0 && line != last) {
            fs.appendFileSync(historyPath, line + '\n');
        }
        return line;
    };
    if (rl.history instanceof Array) {
        rl.history.push.apply(rl.history, history);
    }
    rl.on('SIGINT', () => {
        rl.clearLine();
        rl.prompt(true);
    });
    rl.on('close', () => {
        console.log('');
        console.log('Goodbye!');
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
        for (let key in cmds) {
            const func = cmds[key];
            if (/\|/.test(key)) {
                key = key.split('|');
                key = new RegExp(`^(${key.join('|')})($|\\s+)`);
            } else if (!(key instanceof RegExp)) {
                key = new RegExp(`^${key}($|\\s+)`);
            }
            if (key.test(line)) {
                return func.call(cmd, line.replace(key, ''), options);
            }
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
