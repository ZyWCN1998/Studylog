# 习题11. 数组和字符串

按照课本中的内容输入代码，编写Makefile文件并运行

```c
#include <stdio.h>

int main(int argc, char *argv[]) {

  int numbers[4] = {0};
  char name[4] = {'a'};

  // first, print them out raw
  printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);
  printf("name: %s\n", name);

  // setup the numbers
  numbers[0] = 1;
  numbers[1] = 2;
  numbers[2] = 3;
  numbers[3] = 4;

  // setup the name
  name[0] = 'Z';
  name[1] = 'e';
  name[2] = 'd';
  name[3] = '\0';

  // then print them out initialized
  printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

  // print the name like a string
  printf("name: %s\n", name);

  // another way to use name
  char *another = "Zed";

  printf("another: %s\n", another);

  printf("another each: %c %c %c %c\n", another[0], another[1], another[2],
         another[3]);

  return 0;
}
```

可以看到命令行输出结果

![Untitled](IMAGE/Untitled.png)

现在来解释这个代码：

- ’\0’是一个特殊的字符串，是不会被打印的
- 如果初始化数组的时候只设置了一个数，那么剩下的元素会被0填充
- 当创建字符串字面量的时候，应该使用典型的 char *another = “Literal”语法

# 破坏程序

<aside>
💡 几乎所有的C程序中的bug都是由没有准备足够的空间（下标越界），或者没有在字符串结尾添加’\0’

</aside>

在这个程序中，破坏程序的关键方式是忘记在字符串结尾添加’\0’字符

- 删除设置name数组的初始化语句：可以看到程序仍然正常执行且没有warning，name仍然初始化了000
    
    ![Untitled](IMAGE/Untitled%201.png)
    
- 无意间设置了name[3] = ‘A’，这样就没有终止符了：同上
    
    ![Untitled](IMAGE/Untitled%202.png)
    
- 初始式设置为了{’a’,’a’,’a’,’a’} ，这样a太多了，就没有足够空间放’\0’终止符了：程序同样成功执行了
    
    ![Untitled](IMAGE/Untitled%203.png)
    

# 附加任务

- 将字符赋给numbers，然后使用printf打印出来，一次打印一个字符。你会得到什么样的编译器警告？
    
    并没有警告，打印出来的值是ASCII码
    
    ![Untitled](IMAGE/Untitled%204.png)
    
    ![Untitled](IMAGE/Untitled%205.png)
    
    而如果使用%c打印的话打印出来的就是Zed字符，虽然初始化是int
    
    ![Untitled](IMAGE/Untitled%206.png)
    
- 反过来处理name，将它作为一个int数组处理，让它每次打印一个int。调试器会给你一个什么样的警告
    
    同样没有警告，顺利打印出了数字
    
    ![Untitled](IMAGE/Untitled%207.png)
    
- 如果一个字符数组有4字节长，一个整数也是4字节长，那么你可以将整个name数组当成一个整数吗？
    
    当然可以，一个char是4 bit（000→111），左移各个位到对应位置就好了
    
    ![Untitled](IMAGE/Untitled%208.png)
    
- 将name转换成another风格，看看代码是否还能正常工作?
    
    可以正常初始化赋值，正常打印初始化的值，但不能修改其中的值
    
    <aside>
    💡 *name通过Read-Only Mode存储
    
    </aside>
    
    ![Untitled](IMAGE/Untitled%209.png)
    
    ![Untitled](IMAGE/Untitled%2010.png)