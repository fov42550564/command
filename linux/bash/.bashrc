########################################
#author:fangyunjiang
#date:2012-08-26
#filename:.bashrc
########################################

# If not running interactively, don't do anything
[[ "$-" != *i* ]] && return

#source ~/.profile

shopt -s expand_aliases

source ~/command/common

if __mac;then
  [ -f /usr/local/etc/bash_completion ] && . /usr/local/etc/bash_completion
  echo "fangyunjiang"|pinyinmatch "fx"
elif __cygwin;then
  [ -f /usr/share/bash-completion/bash_completion ] && . /usr/share/bash-completion/bash_completion
fi

# Set a default prompt of: user@time and current_directory
PS1='\n\[\e[32m\]\u\[\e[34m\]@\[\e[32m\]\t \[\e[33m\]\w\[\e[0m\]\n\$'

# Uncomment to use the terminal colours set in DIR_COLORS
if __windows;then
    eval "$(dircolors -b ~/.dir_colors)"
    # ignore return \r\d
    set -o igncr
    export SHELLOPTS
    alias open='__open'
    alias atom='__atom'
    function new() {
      C:/cygwin/CygwinEx.bat "$PWD"
    }
fi

#set options
shopt -s checkwinsize
#shopt -s cdspell
shopt -s nocaseglob

#confirm to do command
alias cp='__cp'
alias tcp=`which cp`
alias mv='__mv'
alias tmv=`which mv`
alias rm='__rm'
alias trm=`which rm`
alias rmr='__recover'
alias rmf='__rm_files'
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
if __mac ;then
    function xcat() { cat "$1"|pbcopy; }
    function xlg() { pbpaste|pbcopy; }
    function xecho() { echo $(pbpaste)|pbcopy; }
    function cpwd() { echo -n `pwd`|pbcopy; }
else
    function xcat() { cat "$1"  > /dev/clipboard; }
    function xlg() {  cat /dev/clipboard > /dev/clipboard; }
    function xecho() { echo $(cat /dev/clipboard) > /dev/clipboard; }
    function cpwd() { echo -n `pwd` > /dev/clipboard; }
fi

#cd which
function wh() { cd $(which "$1"); }

#diffmerge
alias di='__diff'

#git
alias tgit=`which git`
alias git='xgit'

#vim
alias vi='vim'
alias clearvim='rm -fr ~/.data/swap'

#react
function react() { cp "${HOME}/command/linux/atom/snippets/react${1}.cson" "${HOME}/.atom/packages/react/snippets/JavaScript (JSX).cson"; }

#go
alias xgo='go run'

#pod
alias pinstall='pod install --no-repo-update'
alias xandroid='react-native run-android'
alias xandroidinstall='xserver android/app/build/outputs/apk/app-debug.apk'

#phantomjs
alias pt='~/tools/phantomjs-2.1.1-macosx/bin/phantomjs'

#mongodb
alias mongostart='mongod —config /usr/local/etc/mongod.conf'

export NVM_DIR="/Users/apple/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && . "$NVM_DIR/nvm.sh"  # This loads nvm

#npm
alias cnpm="npm --registry=https://registry.npm.taobao.org"

#set_path
export PATH=$PATH:~/command
if __mac;then
    export PATH=/usr/local/opt/gnu-sed/libexec/gnubin:$PATH
    export PATH=/usr/local/opt/findutils/libexec/gnubin:$PATH
    export PATH=$PATH:/usr/local/lib/node_modules/cordova/bin:~/tools/phantomjs-2.1.1-macosx/bin:~/tools/wasm:~/tools/wabt
elif __linux;then
    export PATH=$PATH:/root/tools/node/bin:/root/tools/node/lib/node_modules/npm/bin/node-gyp-bin:/root/tools/mongodb/bin
    export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.212.b04-0.el7_6.x86_64
    export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
    export PATH=$JAVA_HOME/bin:$PATH
fi

#only for mac
if __mac;then
    # iphone6
    function xphone(){ xcrun instruments -w "iPhone 6 (12.2)"; }
    #idea
    alias idea='/Applications/IntelliJ\ IDEA.app/Contents/MacOS/idea'
    #.DS
    alias clearmac='find -name .DS_* -exec rm {} \;'
    # pm2
    alias clearpm2='/bin/rm -f ~/.pm2/logs/*'
    #android
    alias ndk='ndk-build'
    export ANDROID_NDK_HOME=~/tools/android-ndk-r10e
    export ANDROID_SDK_HOME=~/Library/Android/sdk
    #export ANDROID_SDK_HOME=/usr/local/opt/android-sdk
    export ANDROID_NDK_PREFIX=$ANDROID_NDK_HOME/toolchains/arm-linux-androideabi-4.6/prebuilt/darwin-x86/bin/arm-linux-androideabi
    export ANDROID_SDK_PLATFORM_TOOLS=$ANDROID_SDK_HOME/platform-tools
    export ANDROID_SDK_TOOLS=$ANDROID_SDK_HOME/tools
    export PATH="$PATH:$ANDROID_NDK_HOME:$ANDROID_SDK_TOOLS:$ANDROID_SDK_PLATFORM_TOOLS"
    #export NDK_MODULE_PATH=~/Desktop/cocos2d-x-2.2.2/cocos2dx/platform/third_party/android/prebuilt:~/Desktop/cocos2d-x-2.2.2
    #
    export ANDROID_HOME=~/Library/Android/sdk

    # spring
    export M2_HOME=~/tools/apache-maven-3.6.0
    export PATH=$PATH:$M2_HOME/bin

    #hadoop
    export HADOOP_HOME=~/tools/hadoop-2.7.3
    export PATH=$PATH:$HADOOP_HOME/bin
    export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME/lib/native
    export HADOOP_OPTS="-Djava.library.path=$HADOOP_HOME/lib"


    export EMSDK=~/tools/emsdk-portable
    export PATH=$PATH:$EMSDK/emscripten/1.37.35
fi
