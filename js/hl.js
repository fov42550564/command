const hljs = require("highlight.js")
const h2j = require('html2json');
const css2json = require('css2json');
const chalk = require("chalk")
const fs = require('fs');
const path = require('path');

const readStylesheet = function(theme) {
    const style = fs.readFileSync(path.join(__dirname, '..', 'node_modules', 'highlight.js', 'styles', theme + '.css'));
    return css2json(style.toString());
};

const getTextWithColor = function(text, color, isBg) {
    if (color) {
        if (color.startsWith('#')) {
            if (color.length === 4) {
                color = `#${color[1]}${color[1]}${color[2]}${color[2]}${color[3]}${color[3]}`;
            }
            text = chalk[!isBg ? 'hex' : 'bgHex'](color)(text);
        } else {
            text = chalk[!isBg ? 'keyword' : 'bgKeyword'](color)(text);
        }
    }
    return text;
};

const stylize = function(name, text, styles, hasBg) {
    const style = styles['.'+name];
    if (style) {
        text = getTextWithColor(text, style['color']);
        if (hasBg) {
            text = getTextWithColor(text, style['background-color'] || style['background']);
        }
        if (style["text-decoration"] && style["text-decoration"].toLowerCase() === "underline") {
            text = chalk.underline(text);
        }
        if (style["font-weight"] && style["font-weight"].toLowerCase() === "bold") {
            text = chalk.bold(text);
        }
        if (style["font-style"] && style["font-style"].toLowerCase() === "italics") {
            text = chalk.italics(text);
        }
    }
    return text;
};

const deentitize = function(str) {
    return str.replace(/&gt;/g, '>')
    .replace(/&lt;/g, '<')
    .replace(/&quot;/g, '"')
    .replace(/&apos;/g, "'")
    .replace(/&amp;/g, '&');
};
const replaceSpan = function(obj, styles, hasBg) {
    if (obj.child) {
        for (let i = 0; i < obj.child.length; i++) {
            obj.child[i] = replaceSpan(obj.child[i], styles, hasBg);
        }
    }
    if (obj.node === "element") {
        return stylize(obj.attr.class, obj.child.join(''), styles, hasBg);
    } else if (obj.node === "text") {
        return obj.text;
    } else if (obj.node === "root") {
        return obj.child.join('');
    } else {
        console.error("Found a node type of " + obj.node + " that I can't handle!");
    }
};

module.exports = function(text, theme, lang, hasBg) {
    const styles = readStylesheet(theme);
    const hlText = lang &&  hljs.getLanguage(lang) ? hljs.highlight(lang, text) : hljs.highlightAuto(text);
    const json = h2j.html2json(hlText.value);
    text = replaceSpan(json, styles, hasBg);
    text = stylize('hljs', text, styles, hasBg);
    text = deentitize(text);
    return text;
}
