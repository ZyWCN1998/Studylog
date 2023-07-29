# 20230729 Missing Semester 04

# Missing Semester 03 Data ****Wrangling****

Data Wrangling: 一种格式的数据转换为另一种数据

Data Source: system log

## Grep

查找特定的字符串，以以下命令为例，通过管道将前一个命令执行的结果传输至下一个命令

```bash
ssh tsp journalctl | grep ssh | grep "Disconnected from"
```

less：将一段长内容塞入你的命令行窗口

## Sed

字符串编辑器，本质上是一种编程语言。可以查找替换字符串的内容。以下命令为例，sed接受两个参数，第一个是查找的字符串，第二个是替换的字符串（本例中为空）

Sed逐行处理数据，即逐行匹配每个模式

```bash
cat ssh. log | sed 's / .*Disconnected from/ / '
```

<aside>
💡 正则表达式

</aside>

正则表达式主要用来match字符串的内容：

- .指所有字符，
- *指有0个或更多这种字符，
- +指有1个或更多这种字符
- [ ]匹配有[ ]中的任意一个内容的模式
- ( )匹配有( )所有内容的模式
- -E使Sed能匹配扩展的正则表达式
- ^匹配行首
- $匹配行位
- [0..9] 匹配0~9的数字
- ( )或[ ]内的内容为一个capture group，可以通过\ number的方式访问第number个capture group

在sed中，正则表达式采用的是懒惰模式（lazy），即只匹配到一次成功的模式就停止匹配，可以使用g参数使其匹配尽可能多的次数（贪婪模式，greedy）

以下命令表示只匹配还有（ab）的模式，只有a或只有b都不会被匹配

```bash
echo 'abcaba '| sed 's/(ab)*//g'
```

正则表达式中同样可以使用逻辑运算符来匹配内容，以下内容可以替换ab或bc，注意这里的 | 同样为短路操作

```bash
echo 'absababc' | sed -E 's/(ab | bc)*//g'
```

使用regular expression debugger来减轻生活的压力

以下例子展示了通过使用第二个capture group替换匹配到的字符串得到的结果，即显示所有用户名

```bash
cat ssh.log | sed -E 's/^.*Disconnected from (invalid | authenticating )?user (.*)[0-9.]+ port [0-9]·(\[preauth\])?$/\2/'head -n100
```

<aside>
💡 Regular expression can be really hard!

</aside>

![HH0XP05U]PK61{B64P_GM59.png](IMAGE/HH0XP05UPK61B64P_GM59.png)

## Sort

Sort可以排序结果，以上面的命令为例，可以通过sort来排序结果，比如通过字母顺序

-n：数值排序（numerical sort）

-k：

1,1: start at 1, stop at 1

```bash
cat ssh.log | sed -E 's/^.*Disconnected from (invalid |authenticating
?user (.*)[0-9.]+ port [0-9]+(\[preauth\] ) ?$/\2/'| sort
```

## Uniq

uniq -c 计数重复的项目

比如想查看什么用户名被使用了最多：以上面的命令再进行一次sort

tail -n10：只取最后10行

```bash
cat ssh.log | sed -E 's/^.*Disconnected from (invalid |authenticating
?user (.*)[0-9.]+ port [0-9]+(\[preauth\] ) ?$/\2/'| sort | uniq -c | sort -nk1,1 | tail -n10
```

## Awk

awk是一个column based stream processor. 更注重列数据，同样也是可以编程并进行流程控制的

print $2：打印第二列（在本例中为用户名）

awk同样可以使用正则表达式来查找匹配特定模式的字符串，比如以下命令可以从之前检索的信息中检索出用户名为c开头，e结尾，且只出现过1次的用户名

BEGIN: 只在开头match

END: 只在结尾match

```bash
cat ssh.log | sed -E 's/^.*Disconnected from (invalid lauthenticating )?user (.*)[0-9.]+ port [0-9]'( \ [preauth\])?$/\2/'  sort | uniq -c | awk '$1 == 1 && $2 ~ /Ac.*e$/ {print $0}'
```

## Paste

paste将多行信息重新粘贴为一行

## How do I know

Software: learn from class. 

Flags: man 

## bc

command line calculator，或许最初发源于Berkeley calculator，是一个命令行计算器

```bash
echo "1 +2" bc -l
3
```

以下命令展示了之前例子中，所有没有重复的用户名的数量，如果想把他们相加，可以首先用paste把他们打印成粘在一起的形式，之后通过管道工具传送给bc求得结果

```bash
cat ssh.log | sed -E 's/^.*Disconnected from (invalid | authenticating)?user (.*)[0-9.]+ port [0-9]·( \[preauthl])?$/\2/' | sort | uniq -c | awk '$1 != 1 { print $1 }' | paste -sd+ | bc -l
```

命令行同样可以和其他脚本语言结合起来进行数据处理

## Xargs

xargs接收各种各样的输入，并把他们变为参数，比如你想一次性删除多个文件，不需要挨个复制粘贴他们的名字给uninstall作为参数，可以直接使用|xargs uninstall将一堆文件名作为参数传给uninstall

## ffmpeg

一个非常好用的格式转换器