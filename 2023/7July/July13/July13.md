# 20230713 GNU Debugger & Make

# GDB: GNU Debugger

GDB是*nix系统默认自带的调试器，可以用来调试C与C++

RTFM：GDB文档

[GDB, The GNU Debugger, By Example - Cprogramming.com](https://www.cprogramming.com/gdb.html)

官方文档给出了一个快速入门教程，以下内容为对该入门教程的学习整理

使用gcc/g++编译一个C程序

```bash
g++ main.cpp -o main
```

即可输出一个名为main的可执行文件，可以使用./main来运行这个文件

但如果想调试这个文件还需要加入调试指令（不加入的话打包的可执行文件体积更小）

```bash
g++ main.cpp -g -Wall -Werror -o main
```

打开调试功能与所有warning，之后可以使用GDB调试器来进行调试

### GDB调试与断点设置

```bash
gdb <filename> #使用gdb调试特定文件名
break <sourcefile line number> #在调试文件特定位置放置断点
run #使程序开始运行到断点位置
list #程序停止时显示断点附近的10行内容（显示停在了什么位置）
```

当程序运行到断点之后可以进行单步调试并查看变量值等一系列操作

### 单步调试与查看变量值

next和step两个命令都可以进行单步执行，其区别是next不会进入被调用函数内部。step会进入被调用函数内部

- Next: Next keeps the control strictly in the current scope
- Step: Step follows the execution through function calls.

```bash
next  #下一行
step #进入函数内部
print <variable name> #打印查看变量值
set <varname> = <value> #设置变量值为value，直接改变变量值
```

通过set watchpoint设置观测点观察变量的值

```bash
watch <var> #设置某个变量为watch point
```