# 20230718 Learn C the Hardway 09

While loop in C

按照课本内容编写while循环内容，便携Makefile文件，并使用gdb调试器进行调试，查看程序行为：

![Untitled](IMAGE/Untitled.png)

注：gdb几个常见的tricks：

- 使用命令 layout asm 即可进入gdb的TUI界面（terminal user interface）
- 使用命令layout next可以切换不同的TUI界面（源代码，源代码+汇编，源代码+汇编+寄存器，汇编+寄存器）
- 不输入命令直接摁Enter默认执行上一次的指令
- 可以使用缩写，诸如b为break等

## Break the program of Lecture 9

![Untitled](IMAGE/Untitled%201.png)

特别的，对于忘记初始化的情况，可以在gdb调试的时候看出来：

i如果忘记初始化赋值，里面的garbage可能是任何数：

![Untitled](IMAGE/Untitled%202.png)

## **Extra Credit**

- Make the loop count backward by using i-- to start at 25 and go to 0.

![Untitled](IMAGE/Untitled%203.png)

- Write a few more complex while-loops using what you know so far

输出乘法表

![Untitled](IMAGE/Untitled%204.png)