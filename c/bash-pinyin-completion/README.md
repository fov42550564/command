## 特性

  * 支持拼音首字母匹配和完全匹配
  * 支持多音字匹配
  * 仅支持UTF-8编码环境

## 匹配规则
方运江 方运江1 fang f123
f： 可以匹配所有
fang： 可以匹配： 方运江 方运江1 fang
fyj: 可以匹配： 方运江 方运江1
fj: 可以匹配： 方运江 方运江1

全字连续匹配 > 拼音首字母连续匹配 > 拼音首字母跳跃匹配 > 全字跳跃匹配
如果有数字，先使用数字匹配，如果没有找到，使用分数字匹配后过滤

## 安装
### mac
* 首先需要安装 bash_complete

``` sh
brew install bash_complete
```
* 然后进入工程，编译，安装

``` sh
make
make install
```

## 规则

```
 实验目录如下：
 ls
 SVN培训  全球眼  浙江建行  浙江农信

 使用: (输完后按 TAB 键自动补全)
    cd S     <tab>             进入[SVN培训]
    cd q     <tab>             进入[全球眼]
    cd z     <tab>             自动补全[浙江]
    cd zj    <tab><tab>        提示[浙江建行 浙江农信]备选
    cd zjj   <tab>             进入[浙江建行]
    cd zj1   <tab>             进入[浙江建行]
    cd zj2   <tab>             进入[浙江农信]
```
