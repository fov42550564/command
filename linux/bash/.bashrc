########################################
#author:fangyunjiang
#date:2012-08-26
#filename:.bashrc
########################################

# If not running interactively, don't do anything
[[ "$-" != *i* ]] && return

#source ~/.profile

if [ $(uname) = "Darwin" ];then
    export _OS_=mac
else
    export _OS_=cygwin
fi
shopt -s  expand_aliases

source ~/command/common

# Set a default prompt of: user@time and current_directory
PS1='\n\[\e[32m\]\u\[\e[34m\]@\[\e[32m\]\t \[\e[33m\]\w\[\e[0m\]\n\$'

# Uncomment to use the terminal colours set in DIR_COLORS
if __cygwin;then
eval "$(dircolors -b /etc/DIR_COLORS)"
# ignore return \r\d
set -o igncr
export SHELLOPTS
#open
alias open='cygstart'
fi

#set options
shopt -s checkwinsize
#shopt -s cdspell
shopt -s nocaseglob

#confirm to do command
# alias rm='rm -i'
# alias cp='cp -i'
# alias mv='mv -i'
# alias rm='xxrm'
alias rm='/Users/fang/command/linux/rm-trash/rm.rb'

#path
alias path='echo -e ${PATH//:/\\n}'
alias libpath='echo -e ${LD_LIBRARY_PATH//:/\\n}'  

#du df
alias du='du -sh'
alias df='df -kTh'

#show ls color
if __mac ;then
alias ls='ls -G'
else
alias ls='ls --color'
fi
alias dir='ls -l'
alias ll='ls -l'
alias la='ls -A'
alias l='ls -AlF'
alias lx='ls -lXB' #sort by file extract name
alias lk='ls -lSr' #sort by file size 
alias lc='ls -lcr' #sort by midify time
alias lu='ls -lur' #sort by access time
alias lt='ls -ltr' #sort by datetime
alias lr='ls -lR'  
alias lm='ls -al |more'
alias tree='tree -Csu'

#history
#use !4 to repeat 4th command
#use !$ get last command last param; !^ get last command first param ;can use !4$
#use !:1 get last command 1st param; !:* get all params
#use ctrl+r to search history
alias h='history'
alias hh='history 20'
export HISTCONTROL=ignoredups 
#export HISTCONTROL=erasedups
#HISTSIZE=450
#HISTFILESIZE=450
#HISTFILE=~/.bash_history


#modify cd command
alias cd='__extend_cd'
alias cdf='__folder_cd'
alias ..='cd ..'
alias ...='cd ../..'
alias ....='cd ../../..'
alias .....='cd ../../../..'
alias ......='cd ../../../../..'
alias -- +='pushd .'
alias -- -='popd'

#read command
alias r='__read_file'

#modify file operator
alias rd=rmdir
alias md='mkdir -p'


#show grep color
alias grep='grep --color' 
alias egrep='egrep --color=auto'
alias fgrep='fgrep --color=auto'    

alias dt='__set_dest_path'
alias pcs='xpcs'
alias cls='__clear'
function pb(){ __show_progressbar 0.1 "$@" 2>/dev/null; }


#make macosx lib file
function mk() { gcc -dynamiclib -undefined suppress -flat_namespace $1.c -o lib$1.dylib -m32; }

#log
function lg() { if [ -z "$1" ];then xlog -r; else xlog -r -f "$1"; fi }
function lga() { if [ -z "$1" ];then xlog -r -p; else xlog -r -p -f "$1"; fi }
function lgv() { if [ -z "$1" ];then xlog -v -r; else xlog -v -r -f "$1"; fi }
function lgm() { if [ -z "$1" ];then xlog -v -m -r; else xlog -v -m -r -f "$1"; fi }
function log() { if [ -z "$1" ];then xlog -a; else xlog -a -f "$1"; fi }
function logv() { if [ -z "$1" ];then xlog -v -a; else xlog -v -a -f "$1"; fi }
function logm() { if [ -z "$1" ];then xlog -v -m -a; else xlog -v -m -a -f "$1"; fi }
function xcat() { cat "$1"|pbcopy; }
function xlg() { pbpaste|pbcopy; }

#cd which
function wh() { cd $(which "$1"); }

#diffmerge
if [ $_OS_ = "mac" ];then
    alias di='diffmerge.sh'
else
    alias di='bcompare'
fi

#copy
alias mhon='xsmb -m'
alias mhoff='xsmb -u'
alias mhls='ls /Volumes/home/方运江'
alias mhrm='rm /Volumes/home/方运江/*'
alias mhcd='cd /Volumes/home/方运江'
alias mhopen='open /Volumes/home/方运江'
alias mhmv='cp /Volumes/home/方运江/*'


#set_path
export PATH=$PATH:~/command:/usr/local/lib/node_modules/cordova/bin

#ndk
alias ndk='ndk-build'
if [ "$_OS_" = "cygwin" ];then
export ANDROID_NDK_HOME=~/f/android/android-ndk-r9d
export ANDROID_SDK_HOME=~/f/android/android-sdk
else
export ANDROID_NDK_HOME=~/tools/android-ndk-r10e
export ANDROID_SDK_HOME=/usr/local/opt/android-sdk
fi
export ANDROID_NDK_PREFIX=$ANDROID_NDK_HOME/toolchains/arm-linux-androideabi-4.6/prebuilt/darwin-x86/bin/arm-linux-androideabi
export ANDROID_SDK_PLATFORM_TOOLS=$ANDROID_SDK_HOME/platform-tools
export ANDROID_SDK_TOOLS=$ANDROID_SDK_HOME/tools
export PATH="$PATH:$ANDROID_NDK_HOME:$ANDROID_SDK_TOOLS:$ANDROID_SDK_PLATFORM_TOOLS"
#export NDK_MODULE_PATH=~/Desktop/cocos2d-x-2.2.2/cocos2dx/platform/third_party/android/prebuilt:~/Desktop/cocos2d-x-2.2.2
#
export ANDROID_HOME=/usr/local/opt/android-sdk
#vim
alias vi='vim'
alias clearvim='rm -fr ~/.data/swap'
alias viclear='rm -f ~/.data/swap/*.swp'

#go
alias xgo='go run'
#node.js

if __mac ;then
alias nodex='iojs --es_staging --use-strict'
alias noded='iojs --debug-brk'
alias nodedd='node-inspector &'
alias nodedc='xnw -l "http://127.0.0.1:8080/debug?port=5858"'
export NODE_PATH="/usr/local/lib/node_modules/"
else
export NODE_PATH="/cygdrive/c/Documents and Settings/fang/Application Data/npm/node_modules"
fi
#atom-shell
#alias atom='/Users/apple/node/atom-shell/Atom.app/Contents/MacOS/Atom'
#node-webkit
alias nw='/usr/local/lib/nwjs/nwjs.app/Contents/MacOS/nwjs'
alias nw-build='nw-gyp build'
alias nw-rebuild='nw-gyp rebuild --target=0.7.5'
alias nw-read='~/node/git/nw-read/nw-read'
alias node-build='node-gyp build'
alias node-rebuild='node-gyp rebuild'
alias ffpcm='ffplay -f s16le -ar 41000 -ac 2 -loop 0'
alias ffrgba='ffplay -f rawvideo -pixel_format rgba -video_size 128x128'
alias ffyuv='ffplay -f rawvideo -loop 0 -video_size 568x320'
alias copytext='lg __1__ && cat __1__|pbcopy && rm -f __1__'
alias cpwd='echo -n `pwd`|pbcopy'

alias gyp='~/node/gyp/gyp-read-only/gyp --depth=.'

alias youku='nw ~/node/youku'
alias qiubai='nw ~/node/git/node-qiubai/node_modules/node-qiubai/qiubai'
alias net='__skip_sudo;sudo ifconfig en0 ether c8:2a:14:5a:f9:bb'

#function xphone(){ cp "~/Desktop/cocos2d-x-2.2.2/samples/Cpp/HelloCpp/proj.android/bin/HelloCpp.apk" "/Volumes/NO NAME 1/a-qiubai/"; }
function xphone(){ cp "~/android/workspace/TestGlRender/bin/TestGlRender.apk" "/Volumes/NO NAME 1/a-qiubai/"; echo "done"; }

alias tsvn='telnet tj.myrsky.com.cn 3690'
alias st='svn st|grep -v node_modules'

#pod
alias pinstall='pod install --no-repo-update'
alias xandroid='react-native run-android'

#mongodb
alias mongostart='mongod —config /usr/local/etc/mongod.conf'

export NVM_DIR="/Users/apple/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm
