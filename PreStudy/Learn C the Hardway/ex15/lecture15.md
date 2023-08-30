# 习题15. 指针，可怕的指针

按照教材要求录入程序，使用make命令编译，执行

```c

#include <stdio.h>

int main(int argc, char *argv[]) {
  // create two arrays we care about
  int ages[] = {23, 43, 12, 89, 2};
  char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};

  // safely get the size of ages
  int count = sizeof(ages) / sizeof(int);
  int i = 0;

  // first way using indexing
  for (i = 0; i < count; i++) {
    printf("%s has %d years alive. \n", names[i], ages[i]);
  }

  printf("---\n");

  // setup the pointers to the start of the arrays
  int *cur_age = ages;
  char **cur_name = names;

  // second way using pointers
  for (i = 0; i < count; i++) {
    printf("%s is %d years old. \n", *(cur_name + i), *(cur_age + i));
  }

  printf("---\n");

  // third way, pointers are just arrays
  for (i = 0; i < count; i++) {
    printf("%s is %d years old again. \n", cur_name[i], cur_age[i]);
  }

  printf("---\n");

  // fourth way with pointers in a stupid complex way
  for (cur_name = names, cur_age = ages; (cur_age - ages) < count;
       cur_name++, cur_age++) {
    printf("%s lived %d years so far. \n", *cur_name, *cur_age);
  }

  return 0;
}
```

```bash
❯ make           
cc -Wall -g    lecture15.c   -o lecture15
```

```bash
❯ ./lecture15                
Alan has 23 years alive. 
Frank has 43 years alive. 
Mary has 12 years alive. 
John has 89 years alive. 
Lisa has 2 years alive. 
---
Alan is 23 years old. 
Frank is 43 years old. 
Mary is 12 years old. 
John is 89 years old. 
Lisa is 2 years old. 
---
Alan is 23 years old again. 
Frank is 43 years old again. 
Mary is 12 years old again. 
John is 89 years old again. 
Lisa is 2 years old again. 
---
Alan lived 23 years so far. 
Frank lived 43 years so far. 
Mary lived 12 years so far. 
John lived 89 years so far. 
Lisa lived 2 years so far.
```

# 破坏程序

只需要将指针指向错误的地方，就能破坏这个程序

- 尝试将cur_age指向names。你需要用C语言中的类型转换（cast）来强制执行，研究一下具体需要怎么做
    
    其中最后一个循环正确的原因是因为在for的初始化中被重新指向了age，C中的强制类型转换
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
      // create two arrays we care about
      int ages[] = {23, 43, 12, 89, 2};
      char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};
    
      // safely get the size of ages
      int count = sizeof(ages) / sizeof(int);
      int i = 0;
    
      // first way using indexing
      for (i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", names[i], ages[i]);
      }
    
      printf("---\n");
    
      // setup the pointers to the start of the arrays
      // int *cur_age = ages;
      int *cur_age = (int *)names;
      char **cur_name = names;
    
      // second way using pointers
      for (i = 0; i < count; i++) {
        printf("%s is %d years old. \n", *(cur_name + i), *(cur_age + i));
      }
    
      printf("---\n");
    
      // third way, pointers are just arrays
      for (i = 0; i < count; i++) {
        printf("%s is %d years old again. \n", cur_name[i], cur_age[i]);
      }
    
      printf("---\n");
    
      // fourth way with pointers in a stupid complex way
      for (cur_name = names, cur_age = ages; (cur_age - ages) < count;
           cur_name++, cur_age++) {
        printf("%s lived %d years so far. \n", *cur_name, *cur_age);
      }
    
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture15
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ---
    Alan is 1616273412 years old.
    Frank is 21930 years old.
    Mary is 1616273417 years old.
    John is 21930 years old.
    Lisa is 1616273423 years old.
    ---
    Alan is 1616273412 years old again.
    Frank is 21930 years old again.
    Mary is 1616273417 years old again.
    John is 21930 years old again.
    Lisa is 1616273423 years old again.
    ---
    Alan lived 23 years so far.
    Frank lived 43 years so far.
    Mary lived 12 years so far.
    John lived 89 years so far.
    Lisa lived 2 years so far.
    ```
    
- 在最后一个for循环中，尝试用各种方法让计算出错
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
      // create two arrays we care about
      int ages[] = {23, 43, 12, 89, 2};
      char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};
    
      // safely get the size of ages
      int count = sizeof(ages) / sizeof(int);
      int i = 0;
    
      // first way using indexing
      for (i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", names[i], ages[i]);
      }
    
      printf("---\n");
    
      // setup the pointers to the start of the arrays
      // int *cur_age = ages;
      int *cur_age = (int *)names;
      char **cur_name = names;
    
      // second way using pointers
      for (i = 0; i < count; i++) {
        printf("%s is %d years old. \n", *(cur_name + i), *(cur_age + i));
      }
    
      printf("---\n");
    
      // third way, pointers are just arrays
      for (i = 0; i < count; i++) {
        printf("%s is %d years old again. \n", cur_name[i], cur_age[i]);
      }
    
      printf("---\n");
    
      // fourth way with pointers in a stupid complex way
      for (cur_name = names, cur_age = (int *)names; (cur_age - ages) < count;
           cur_name++, cur_age++) {
        printf("%s lived %d years so far. \n", *cur_name, *cur_age);
      }
    
      return 0;
    }
    ```
    
    最后一个循环也把cur_age重新指向name，就会出错
    
    ```bash
    ❯ ./lecture15
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ---
    Alan is -103370748 years old.
    Frank is 22053 years old.
    Mary is -103370743 years old.
    John is 22053 years old.
    Lisa is -103370737 years old.
    ---
    Alan is -103370748 years old again.
    Frank is 22053 years old again.
    Mary is -103370743 years old again.
    John is 22053 years old again.
    Lisa is -103370737 years old again.
    ---
    ```
    
- 尝试重写这些循环，从结尾向开头访问数组。这个任务看着简单，做起来确难
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
      // create two arrays we care about
      int ages[] = {23, 43, 12, 89, 2};
      char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};
    
      // safely get the size of ages
      int count = sizeof(ages) / sizeof(int);
      int i = 0;
    
      // first way using indexing
      for (i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", names[i], ages[i]);
      }
    
      printf("---\n");
    
      // setup the pointers to the start of the arrays
      int *cur_age = ages;
      char **cur_name = names;
    
      // second way using pointers
      for (i = count - 1; i >= 0; i--) {
        printf("%s is %d years old. \n", *(cur_name + i), *(cur_age + i));
      }
    
      printf("---\n");
    
      // third way, pointers are just arrays
      for (i = count - 1; i >= 0; i--) {
        printf("%s is %d years old again. \n", cur_name[i], cur_age[i]);
      }
    
      printf("---\n");
    
      // fourth way with pointers in a stupid complex way
      for (cur_name = names + count - 1, cur_age = ages + count - 1;
           (cur_age - ages) >= 0; cur_name--, cur_age--) {
        printf("%s lived %d years so far. \n", *cur_name, *cur_age);
      }
    
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture15
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ---
    Lisa is 2 years old.
    John is 89 years old.
    Mary is 12 years old.
    Frank is 43 years old.
    ---
    Lisa is 2 years old again.
    John is 89 years old again.
    Mary is 12 years old again.
    Frank is 43 years old again.
    ---
    Lisa lived 2 years so far.
    John lived 89 years so far.
    Mary lived 12 years so far.
    Frank lived 43 years so far.
    ```
    

# 附加任务

- 将这个程序中所有的数组重写为指针
    
    ```c
    for (i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", *(names + i), *(ages + i));
      }
    ```
    
- 将这个程序中所有的指针重写为数组
    
    这个题看的有点云里雾里的，源代码里已经这么做了啊
    
    ```c
    // second way using pointers
      for (i = count - 1; i >= 0; i--) {
        printf("%s is %d years old. \n", cur_name[i], cur_age[i]);
      }
    ```
    
- 回顾以往使用输入的程序，用指针替代之
    
    重写exercise11
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
    
      int numbers[4] = {0};
      char name[4] = {'a', 'a', 'a', 'a'};
      //  char *name = "aaaa";
      // first, print them out raw
      printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
             numbers[3]);
    
      printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);
      printf("name: %s\n", name);
    
      // setup the numbers
      numbers[0] = 'Z';  // 1;
      numbers[1] = 'e';  // 2;
      numbers[2] = 'd';  // 3;
      numbers[3] = '\0'; // 4;
    
      // setup the name
      name[0] = 'Z';
      name[1] = 'e';
      name[2] = 'd';
      name[3] = '\0';
    
      // then print them out initialized
      printf("numbers: %c %c %c %c\n", *(numbers), *(numbers + 1), *(numbers + 2),
             *(numbers + 3));
    
      printf("name each: %d %d %d %d\n", *(name), *(name + 1), *(name + 2),
             *(name + 3));
    
      // print the name like a string
      printf("name: %s\n", name);
    
      // another way to use name
      char *another = "Zed";
    
      printf("another: %s\n", another);
    
      printf("another each: %c %c %c %c\n", *(another), *(another + 1),
             *(another + 2), *(another + 3));
    
      return 0;
    }
    ```
    
- 只用指针处理命令行参数，方法与本例中处理name相似
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
      char **arugments = argv;
      int i = 0;
      for (i = 0; i < argc; i++) {
        printf("the %dth argument is %s \n", i, *(arugments + i));
      }
      return 0;
    }
    ```
    
    处理结果
    
    ```bash
    ❯ ./lecture15ex4 learn c the hardway
    the 0th argument is ./lecture15ex4
    the 1th argument is learn
    the 2th argument is c
    the 3th argument is the
    the 4th argument is hardway
    ```
    
- 尝试将获取地址与获取值结合起来使用
    
    使用%p可以打印指针指向的地址，或者使用&
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
      char **arugments = argv;
      int i = 0;
      for (i = 0; i < argc; i++) {
        printf("the %dth argument is %s, the address is %p \n", i, *(arugments + i),
               (arugments + i));
      }
      for (i = 0; i < argc; i++) {
        printf("the %dth argument is %s, the address is %p \n", i, *(arugments + i),
               &argv[i]);
      }
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture15ex4 learn c the hardway
    the 0th argument is ./lecture15ex4, the address is 0x7ffe8f225cd8
    the 1th argument is learn, the address is 0x7ffe8f225cd8
    the 2th argument is c, the address is 0x7ffe8f225cd8
    the 3th argument is the, the address is 0x7ffe8f225cd8
    the 4th argument is hardway, the address is 0x7ffe8f225cd8
    
    ❯ ./lecture15ex4 learn c the hardway
    the 0th argument is ./lecture15ex4, the address is 0x7ffc9015a548
    the 1th argument is learn, the address is 0x7ffc9015a550
    the 2th argument is c, the address is 0x7ffc9015a558
    the 3th argument is the, the address is 0x7ffc9015a560
    the 4th argument is hardway, the address is 0x7ffc9015a568
    the 0th argument is ./lecture15ex4, the address is 0x7ffc9015a548
    the 1th argument is learn, the address is 0x7ffc9015a550
    the 2th argument is c, the address is 0x7ffc9015a558
    the 3th argument is the, the address is 0x7ffc9015a560
    the 4th argument is hardway, the address is 0x7ffc9015a568
    ```
    
- 在结尾加入另一个for循环，打印这些指针使用的地址。你需要在printf中使用%p格式
    
    好像跟上一道题没有区别。。。
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[]) {
      char **arugments = argv;
      int i = 0;
      for (i = 0; i < argc; i++) {
        printf("the %dth argument is %s, the address is %p \n", i, *(arugments + i),
               (arugments + i));
      }
      for (i = 0; i < argc; i++) {
        printf("the %dth argument is %s, the address is %p \n", i, *(arugments + i),
               &argv[i]);
      }
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture15ex4 learn c the hardway
    the 0th argument is ./lecture15ex4, the address is 0x7ffe8f225cd8
    the 1th argument is learn, the address is 0x7ffe8f225cd8
    the 2th argument is c, the address is 0x7ffe8f225cd8
    the 3th argument is the, the address is 0x7ffe8f225cd8
    the 4th argument is hardway, the address is 0x7ffe8f225cd8
    
    ❯ ./lecture15ex4 learn c the hardway
    the 0th argument is ./lecture15ex4, the address is 0x7ffc9015a548
    the 1th argument is learn, the address is 0x7ffc9015a550
    the 2th argument is c, the address is 0x7ffc9015a558
    the 3th argument is the, the address is 0x7ffc9015a560
    the 4th argument is hardway, the address is 0x7ffc9015a568
    the 0th argument is ./lecture15ex4, the address is 0x7ffc9015a548
    the 1th argument is learn, the address is 0x7ffc9015a550
    the 2th argument is c, the address is 0x7ffc9015a558
    the 3th argument is the, the address is 0x7ffc9015a560
    the 4th argument is hardway, the address is 0x7ffc9015a568
    ```
    
- 重写这个程序，使每种打印方式使用一个函数。尝试将指针传递给函数以便他们获得数据。记住，你可以声明一个函数接受指针作为参数，但这个参数也可以作为数组使用
    
    ```c
    #include <stdio.h>
    
    int print_name1_age(int ages[], char *name[], int count);
    int print_name2_age(int *ages, char **name, int count);
    int print_name3_age(int *ages, char **name, int count);
    int print_name4_age(int *ages, char **names, int *cur_ages, char **cur_names,
                        int count);
    
    int main(int argc, char *argv[]) {
      // create two arrays we care about
      int ages[] = {23, 43, 12, 89, 2};
      char *names[] = {"Alan", "Frank", "Mary", "John", "Lisa"};
    
      // safely get the size of ages
      int count = sizeof(ages) / sizeof(int);
    
      print_name1_age(ages, names, count);
      printf("---\n");
    
      print_name2_age(ages, names, count);
      printf("---\n");
    
      print_name3_age(ages, names, count);
      printf("---\n");
    
      int *cur_ages = ages;
      char **cur_names = names;
      print_name4_age(ages, names, cur_ages, cur_names, count);
      return 0;
    }
    int print_name1_age(int ages[], char *name[], int count) {
      for (int i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", name[i], ages[i]);
      }
      return 0;
    }
    
    int print_name2_age(int *ages, char **name, int count) {
      for (int i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", *(name + i), *(ages + i));
      }
      return 0;
    }
    
    int print_name3_age(int *ages, char **name, int count) {
      for (int i = 0; i < count; i++) {
        printf("%s has %d years alive. \n", name[i], ages[i]);
      }
      return 0;
    }
    
    int print_name4_age(int *ages, char **names, int *cur_ages, char **cur_names,
                        int count) {
      for (cur_ages = ages, cur_names = names; cur_ages - ages < count;
           cur_ages++, cur_names++) {
        printf("%s has %d years alive. \n", *cur_names, *cur_ages);
      }
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture15ex5
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ---
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ---
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ---
    Alan has 23 years alive.
    Frank has 43 years alive.
    Mary has 12 years alive.
    John has 89 years alive.
    Lisa has 2 years alive.
    ```
    
- 将for循环改写为while循环，看看对于不同种类指针的使用，哪种循环更为合适
    
    ```c
    #include <stdio.h>
    
    int main(int argc, char *argv[])
    {
        // create two arrays we care about
        int ages[] = { 23, 43, 12, 89, 2 };
        char *names[] = {
            "Alan", "Frank",
            "Mary", "John", "Lisa"
        };
        
        // safely get the size of ages
        int count = sizeof(ages) / sizeof(int);
        int i = 0;
        
        // first way using indexing
        while (i < count) {
            printf("%s has %d years alive.\n", names[i], ages[i]);
            i++;
        }
        
        printf("---\n");
        
        // setup the pointers to the start of the arrays
        int *cur_age = ages;
        char **cur_name = names;
        
        // second way using pointers
        i = 0;
        while (i < count) {
            printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
            i++;
        }
        
        printf("---\n");
        
        // third way, pointers are just arrays
        i = 0;
        while (i < count) {
            printf("%s is %d years old again.\n", cur_name[i], cur_age[i]);
            i++;
        }
        
        printf("---\n");
        
        // fourth way with pointers in a stuipd conplex way
        while (cur_age - ages < count) {
            printf("%s lived %d years so far.\n", *cur_name, *cur_age);
            cur_name++;
            cur_age++;
        }
        
        return 0;
    }
    ```