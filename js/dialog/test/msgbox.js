var dialog = require('../dialog');

var msg = [
    {head: "Usage :", text: "xfind [OPTIONS]"},
    {head: "  -t [str] :", text: "the to find str, can be regex"},
    {head: "  -n [name] :", text: "set name as x.c "},
    {head: "  -d [dir] :", text: "set dir as ./ "},
    {head: "  -s [new_str]:", text: "repalce str with new_str"},
    {head: "  -e [command]:", text: "do command(_TARGET_) is the find file"},
    {head: "  -r [rename_str]:", text: "[c:cpp]|[]"},
    {head: "  -l [_list_style]:", text: "set list_style[0:not show list, 1:radiobox, 2:checkbox mult command, 3:checkbox single command] "},
    {head: "  -I [_TARGET_]:", text: "set _TARGET_, default is ~~ "},
    {head: "  -v [vim_command]:", text: "do vim command"},
    {head: "  -p [dest_dir]:", text: "copy find files to dest_dir and make dirs"},
    {head: "  -a :", text: "set auto indent by vim"},
    {head: "  -c :", text: "put the *first* found file to clipboard"},
    {head: "  -i :", text: "set ignorcase "},
    {head: "  -f :", text: "show file only "},
    {head: "  -w :", text: "show with window"},
    {head: "  -h :", text: "show help "}
];
dialog.msgbox("xfind", msg);
