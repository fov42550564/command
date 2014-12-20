########################################
#author:fangyunjiang
#date:2012-08-26
#filename:.bashrc
########################################

# If not running interactively, don't do anything
[[ "$-" != *i* ]] && return

#source ~/.profile

export _OS_=mac
#export _OS_=cygwin
shopt -s  expand_aliases

source ~/command/common

# Set a default prompt of: user@time and current_directory
PS1='\n\[\e[32m\]\u\[\e[34m\]@\[\e[32m\]\t \[\e[33m\]\w\[\e[0m\]\n\$'

# Uncomment to use the terminal colours set in DIR_COLORS
__cygwin && eval "$(dircolors -b /etc/DIR_COLORS)"

#set options
shopt -s checkwinsize
#shopt -s cdspell

#confirm to do command
# alias rm='rm -i'
# alias cp='cp -i'
# alias mv='mv -i'
# alias rm='xxrm'

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

#cd which
function wh() { cd $(which "$1"); }

#diffmerge
alias di='diffmerge'

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
#alias ndk='ndk-build'
#export ANDROID_NDK_ROOT=~/android/android-ndk-r8b
#export ANDROID_SDK_ROOT=~/android/android-sdk-mac_x86
#export ANDROID_NDK_PREFIX=$ANDROID_NDK_ROOT/toolchains/arm-linux-androideabi-4.6/prebuilt/darwin-x86/bin/arm-linux-androideabi
#export ANDROID_SDK_PLATFORM_TOOLS=$ANDROID_SDK_ROOT/platform-tools
#export ANDROID_SDK_TOOLS=$ANDROID_SDK_ROOT/tools
#export PATH="$PATH:$ANDROID_NDK_ROOT:$ANDROID_SDK_TOOLS:$ANDROID_SDK_PLATFORM_TOOLS"
#export NDK_MODULE_PATH=/Users/mac/Desktop/cocos2d-x-2.2.2/cocos2dx/platform/third_party/android/prebuilt:/Users/mac/Desktop/cocos2d-x-2.2.2
#
#vim
alias vi='vim'
alias clearvim='rm -fr /Users/mac/.data/swap'
alias viclear='rm -f /Users/mac/.data/swap/*.swp'

#go
alias xgo='go run'
#node.js
export NODE_PATH="/usr/local/lib/node_modules/"
#node-webkit
alias nw='~/node/node-webkit/node-webkit.app/Contents/MacOS/node-webkit'
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

alias gyp='/Users/mac/node/gyp/gyp-read-only/gyp --depth=.'

alias youku='nw ~/node/youku'
alias qiubai='nw ~/node/git/node-qiubai/node_modules/node-qiubai/qiubai'
alias net='__skip_sudo;sudo ifconfig en0 ether c8:2a:14:5a:f9:bb'

#function xphone(){ cp "/Users/mac/Desktop/cocos2d-x-2.2.2/samples/Cpp/HelloCpp/proj.android/bin/HelloCpp.apk" "/Volumes/NO NAME 1/a-qiubai/"; }
function xphone(){ cp "/Users/mac/android/workspace/TestGlRender/bin/TestGlRender.apk" "/Volumes/NO NAME 1/a-qiubai/"; echo "done"; }


