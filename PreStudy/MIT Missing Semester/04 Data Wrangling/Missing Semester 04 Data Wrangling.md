# Missing Semester 04 Data Wrangling

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

---

# **Exercises**

1. Take this [short interactive regex tutorial](https://regexone.com/).

做就好了，很快（大概20min）就能做完，贴一个比较重要的总结在这

![Untitled](IMAGE/Untitled.png)

![Untitled](IMAGE/Untitled%201.png)

1.  Find the number of words (in `/usr/share/dict/words`) that contain at least three `a`s and don’t have a `'s` ending. What are the three most common last two letters of those words? `sed`’s `y` command, or the `tr` program, may help you with case insensitivity. How many of those two-letter combinations are there? And for a challenge: which combinations do not occur?

```bash
❯ cat /usr/share/dict/words | tr "[:upper:]" "[:lower:]" | grep -E "(.*a.*){3,}$"|grep -v "'s"|wc -l
854
```

```bash
❯ cat /usr/share/dict/words | tr "[:upper:]" "[:lower:]" | grep -E "(.*a.*){3,}" | grep -v "'s$" | sed -E "s/^.*([a-z]{2})$/\1/" | sort | uniq -c | sort -n | tail -n3
54 as
63 ns
101 an
```

```bash
❯ cat /usr/share/dict/words | tr "[:upper:]" "[:lower:]" | grep -E "(.*a.*){3}" | grep -v "'s$" | sed -E "s/^.*([a-z]{2})$/\1/" | sort | uniq -c | sort -n | wc -l   
112
```

编写一个脚本打印出所有可能的结果

```bash
#! /bin/bash
for i in {a..z};do
  for j in {a..z};do
    echo "$i$j"
  done
done
```

```bash
./all.sh > all.txt
```

```bash
cat /usr/share/dict/words | tr "[:upper:]" "[:lower:]" | grep -E "(.*a.*){3}" | grep -v "'s$" | sed -E "s/^.*([a-z]{2})$/\1/" | sort | uniq > ~/Desktop/appearance.txt
```

通过diff比较这两个文件的不同，并通过grep搜索出来

```bash
diff --unified appearance.txt all.txt | grep -E "^\+(.*)$" | wc -l
```

1. To do in-place substitution it is quite tempting to do something like `sed s/REGEX/SUBSTITUTION/ input.txt > input.txt`. However, this is a bad idea, why? Is this particular to `sed`? Use `man sed` to find out how to accomplish this.

`sed s/REGEX/SUBSTITUTION/ input.txt > input.txt` 表达式中后一个 `input.txt`会首先被清空，而且是发生在前的。所以前面一个`input.txt`在还没有被 `sed` 处理时已经为空了。在使用正则处理文件前最好是首先备份文件。

```bash
sed -i.bak s/REGEX/SUBSTITUTION/ input.txt
```

可以自动创建一个后缀为 `.bak` 的备份文件。

1. Find your average, median, and max system boot time over the last ten boots. Use `journalctl` on Linux and look for log timestamps near the beginning and end of each boot. On Linux, they may look something like:
    
    `Logs begin at ...`
    
    and
    
    `systemd[577]: Startup finished in ...`
    

先tldr一下

```bash
❯ tldr journalctl
journalctl

  Query the systemd journal.
  More information: https://manned.org/journalctl.

  - Show all messages with priority level 3 (errors) from this [b]oot:
    journalctl -b --priority=3

  - Show all messages from last [b]oot:
    journalctl -b -1

  - Delete journal logs which are older than 2 days:
    journalctl --vacuum-time=2d

  - [f]ollow new messages (like tail -f for traditional syslog):
    journalctl -f

  - Show all messages by a specific [u]nit:
    journalctl -u unit

  - Filter messages within a time range (either timestamp or placeholders like "yesterday"):
    journalctl --since now|today|yesterday|tomorrow --until YYYY-MM-DD HH:MM:SS

  - Show all messages by a specific process:
    journalctl _PID=pid

  - Show all messages by a specific executable:
    journalctl path/to/executable
```

-b是启动信息，结合grep，查找关键字

```bash
❯ cat log.txt | sed -E "s/.*=(.*)s.$/\1/"
 9.277
 8.740
 8.830
 9.075
 9.224
 9.534
 8.945
 8.700
 10.710
 9.243
```

```bash
#最长时间
❯ cat log.txt | sed -E "s/.*=(.*)s.$/\1/" | sort -n | tail -1
 10.710
#最短时间
❯ cat log.txt | sed -E "s/.*=(.*)s.$/\1/" | sort -n | head -1
 8.700
#平均数
❯ cat log.txt | sed -E "s/.*=(.*)s.$/\1/" | (paste -sd+) | bc -l | awk '{print $1/10}'
9.2278
#中位数
❯ cat log.txt | sed -E "s/.*=(.*)s.$/\1/" | (paste -sd+) | sort -n | awk '{print ($5+$6)/2}'
9.379
```

1.  Look for boot messages that are *not* shared between your past three reboots (see `journalctl`’s `-b` flag). Break this task down into multiple steps. First, find a way to get just the logs from the past three boots. There may be an applicable flag on the tool you use to extract the boot logs, or you can use `sed '0,/STRING/d'` to remove all lines previous to one that matches `STRING`. Next, remove any parts of the line that *always* varies (like the timestamp). Then, de-duplicate the input lines and keep a count of each one (`uniq` is your friend). And finally, eliminate any line whose count is 3 (since it *was* shared among all the boots).

<aside>
💡 把出现的所有信息排序一下然后把不等于3的提取出来就好了

</aside>

```bash
❯ cat log.txt | sed -E "s/.*zhiyuanwu-virtual-machine (.*)/\1/" | sort | uniq -c | sort | awk '$1!=3  { print }'
```

修改一下记录log的log.sh

```bash
#! /bin/bash
for i in {1..3};do
  journalctl -b -$i
done
```

1. Find an online data set like [this one](https://stats.wikimedia.org/EN/TablesWikipediaZZ.htm), [this one](https://ucr.fbi.gov/crime-in-the-u.s/2016/crime-in-the-u.s.-2016/topic-pages/tables/table-1), or maybe one [from here](https://www.springboard.com/blog/data-science/free-public-data-sets-data-science-project/). Fetch it using `curl` and extract out just two columns of numerical data. If you’re fetching HTML data, `[pup](https://github.com/EricChiang/pup)` might be helpful. For JSON data, try `[jq](https://stedolan.github.io/jq/)`. Find the min and max of one column in a single command, and the difference of the sum of each column in another.

使用第一个链接

```bash
curl 'https://stats.wikimedia.org/EN/TablesWikipediaZZ.htm#wikipedians'|sed -n "/table1/,/<\/table>/p"|grep "<tr" | sed "1,12d"|head -n -3|sed -E 's/(<[^>]*>)+/ /g'|sed 's/ &nbsp;/ -/g'|sed 's/&nbsp;//g' > data
```

需要先仔细观察数据：

数据之间通过<tag>分割，可以直接使用sed的查找替换功能替换为空格。网站中使用的数据空行使用了&nbsp，可以将&nbsp替换为-来避免串行

之后使用sort和awk即

注意这里的sort默认直接查找的话会按读到的第一个数来排序，比如21,123,145会按照

123

145

21

的顺序排序

```bash
~$ awk '{print $1,$4,$5}' data | sort --key=2n | head -n 1
Jan2001 9 -
# 从data中读取第一列（时间，用来定位后续结果）及第三、四列，并以第二行的数据以数字大小进行排序，然后显示最大值的结果；下一个命令显示最小值的结果

~$ awk '{print $1,$4,$5}' data | sort --key=2n | tail -n 1
Mar2007 91388 11506

~$ awk '{print $1,$4,$5}' data | awk '{print $2-$3}' | awk '{s+=$1} END {print s}'
10153001
# 使用第二列的数据减去第三列的数据后，将结果加总
```