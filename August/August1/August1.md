# 20230801 Missing Semester 04

# 1. Last exercise from lecture 4

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