#!/bin/bash

str="abc方运江123"


function getRealLength {
    len=${#1}
    realLength=0
    charCode=-1
    for ((i=0;i<len;i++));do
        charCode=$(printf "%d" "'${str:i:1}")
        if ((charCode >= 0 && charCode <= 256));then
            ((realLength+=1))
        else
            ((realLength+=2))
        fi
    done
    echo $realLength;
}
function getRealCutText {
    realLength=$(getRealLength "$1");
    n=$(($2-realLength))
    _str=$(printf "%${n}s")
    echo "$1$_str"
}
function cutLimitText() {
    list="$1"
    text="$2"
    n="$3"
    realLength=0
    len=${#text}
    preLen=-1
    charCode=-1
    needCut=0

    for ((i=0;i<len;i++));do
        charCode=$(printf "%d" "'${text:i:1}")
        if ((charCode >= 0 && charCode <= 256));then
            ((realLength+=1))
        else
            ((realLength+=2))
        fi
        if ((preLen == -1 && realLength >= n));then
            preLen=$((i+1))
        elif ((realLength > n));then
            needCut=1
            break
        fi
    done

    if ((needCut == 1));then
        cutText=${text:0:preLen}
        lastCode=$(printf "%d" "'${cutText:${#cutText}-1:1}")
        nextCode=$(printf "%d" "'=${text:preLen:1}")
        if ((lastCode >= 0 && lastCode <= 256 && nextCode >= 0 && nextCode <= 256));then
            cutTextLen=${#cutText}
            for ((j=0;j<cutTextLen;j++));do
                cutTextCode=$(printf "%d" "'${cutText:${#cutText}-1-j:1}")
                if ! ((charCode >= 0 && charCode <= 256));then
                    break;
                fi
            done
            if ((j < cutTextLen));then
                ((preLen-=j));
            fi
        fi
        list.push(this.getRealCutText(text.substr(0, preLen), n));
        text = text.substr(preLen);
        if (text) {
            this.cutLimitText(list, text, n);
        }
    else
        list.push(this.getRealCutText(text, n));
    fi
}

getRealCutText $str 15

#
# str=$(printf "%40s")
# echo ${str// /x}

# len=${#str}
#
# for ((i=0;i<len;i++));do
#     code=$(printf "%d" "'${str:i:1}")
#     if ((code>0));then
#         echo $code
#     fi
# done
