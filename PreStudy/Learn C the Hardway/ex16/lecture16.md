# 习题16. 结构体和指向结构体的指针

按照教材要求录入代码

```c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
  char *name;
  int age;
  int height;
  int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight) {
  struct Person *who = malloc(sizeof(struct Person));
  assert(who != NULL);

  who->name = strdup(name);
  who->age = age;
  who->height = height;
  who->weight = weight;

  return who;
}

void Person_destory(struct Person *who) {
  assert(who != NULL);

  free(who->name);
  free(who);
}

void Person_print(struct Person *who) {
  printf("Name: %s\n", who->name);
  printf("\tAge: %d\n", who->age);
  printf("\tHeight: %d\n", who->height);
  printf("\tWeight: %d\n", who->weight);
}
int main(int argc, char *argv[]) {

  // make two people structures
  struct Person *joe = Person_create("Joe Alex", 32, 64, 140);

  struct Person *frank = Person_create("Frank Blank", 20, 72, 180);

  // print them out and where they are in memory
  printf("Joe is at memory location %p:\n", joe);
  Person_print(joe);

  printf("Frank is at memory location %p\n", frank);
  Person_print(frank);

  // make everyone age 20 years and print them out
  joe->age += 20;
  joe->height -= 2;
  joe->weight += 40;
  Person_print(joe);

  frank->age += 20;
  frank->weight += 20;
  Person_print(frank);

  // destory them both so we clean up
  Person_destory(joe);
  Person_destory(frank);

  return 0;
}
```

```bash
❯ ./lecture16
Joe is at memory location 0x55644180c2a0:
Name: Joe Alex
        Age: 32
        Height: 64
        Weight: 140
Frank is at memory location 0x55644180c2e0
Name: Frank Blank
        Age: 20
        Height: 72
        Weight: 180
Name: Joe Alex
        Age: 52
        Height: 62
        Weight: 180
Name: Frank Blank
        Age: 40
        Height: 72
        Weight: 200
```

# 破坏程序

破坏这个程序的方法与你如何使用指针以及malloc系统有关

- 试着传NULL到Person_destory中，看会发生什么事，如果程序不终止，那么你一定是在Makefile中的CFLAGS选项中缺少了-g选项。
    
    ```c
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    struct Person {
      char *name;
      int age;
      int height;
      int weight;
    };
    
    struct Person *Person_create(char *name, int age, int height, int weight) {
      struct Person *who = malloc(sizeof(struct Person));
      assert(who != NULL);
    
      who->name = strdup(name);
      who->age = age;
      who->height = height;
      who->weight = weight;
    
      return who;
    }
    
    void Person_destory(struct Person *who) {
      assert(who != NULL);
    
      free(who->name);
      free(who);
    }
    
    void Person_print(struct Person *who) {
      printf("Name: %s\n", who->name);
      printf("\tAge: %d\n", who->age);
      printf("\tHeight: %d\n", who->height);
      printf("\tWeight: %d\n", who->weight);
    }
    int main(int argc, char *argv[]) {
    
      // make two people structures
      struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    
      struct Person *frank = Person_create("Frank Blank", 20, 72, 180);
    
      // print them out and where they are in memory
      printf("Joe is at memory location %p:\n", joe);
      Person_print(joe);
    
      printf("Frank is at memory location %p\n", frank);
      Person_print(frank);
    
      // make everyone age 20 years and print them out
      joe->age += 20;
      joe->height -= 2;
      joe->weight += 40;
      Person_print(joe);
    
      frank->age += 20;
      frank->weight += 20;
      Person_print(frank);
    
      // destory them both so we clean up
      Person_destory(NULL);
      Person_destory(joe);
      Person_destory(frank);
    
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture16
    Joe is at memory location 0x560d85d542a0:
    Name: Joe Alex
            Age: 32
            Height: 64
            Weight: 140
    Frank is at memory location 0x560d85d542e0
    Name: Frank Blank
            Age: 20
            Height: 72
            Weight: 180
    Name: Joe Alex
            Age: 52
            Height: 62
            Weight: 180
    Name: Frank Blank
            Age: 40
            Height: 72
            Weight: 200
    lecture16: lecture16.c:26: Person_destory: Assertion `who != NULL' failed.
    [1]    236405 IOT instruction (core dumped)  ./lecture16
    ```
    
- 忘记在程序结尾调用Person_destory，然后在调试器中运行，看它怎样报告说你忘记了释放内存。研究一下看你需要给调试器什么选项，让它打印出内存泄漏的相关细节。
    
    使用valgrind 来检查内存泄漏
    
    ```c
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    struct Person {
      char *name;
      int age;
      int height;
      int weight;
    };
    
    struct Person *Person_create(char *name, int age, int height, int weight) {
      struct Person *who = malloc(sizeof(struct Person));
      assert(who != NULL);
    
      who->name = strdup(name);
      who->age = age;
      who->height = height;
      who->weight = weight;
    
      return who;
    }
    
    void Person_destory(struct Person *who) {
      assert(who != NULL);
    
      free(who->name);
      free(who);
    }
    
    void Person_print(struct Person *who) {
      printf("Name: %s\n", who->name);
      printf("\tAge: %d\n", who->age);
      printf("\tHeight: %d\n", who->height);
      printf("\tWeight: %d\n", who->weight);
    }
    int main(int argc, char *argv[]) {
    
      // make two people structures
      struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    
      struct Person *frank = Person_create("Frank Blank", 20, 72, 180);
    
      // print them out and where they are in memory
      printf("Joe is at memory location %p:\n", joe);
      Person_print(joe);
    
      printf("Frank is at memory location %p\n", frank);
      Person_print(frank);
    
      // make everyone age 20 years and print them out
      joe->age += 20;
      joe->height -= 2;
      joe->weight += 40;
      Person_print(joe);
    
      frank->age += 20;
      frank->weight += 20;
      Person_print(frank);
    
      // destory them both so we clean up
      // Person_destory(joe);
      // Person_destory(frank);
    
      return 0;
    }
    ```
    
    ```bash
    ❯ valgrind --leak-check=full ./lecture16
    ==256664== Memcheck, a memory error detector
    ==256664== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==256664== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==256664== Command: ./lecture16
    ==256664==
    Joe is at memory location 0x4a96040:
    Name: Joe Alex
            Age: 32
            Height: 64
            Weight: 140
    Frank is at memory location 0x4a960f0
    Name: Frank Blank
            Age: 20
            Height: 72
            Weight: 180
    Name: Joe Alex
            Age: 52
            Height: 62
            Weight: 180
    Name: Frank Blank
            Age: 40
            Height: 72
            Weight: 200
    ==256664==
    ==256664== HEAP SUMMARY:
    ==256664==     in use at exit: 69 bytes in 4 blocks
    ==256664==   total heap usage: 5 allocs, 1 frees, 1,093 bytes allocated
    ==256664==
    ==256664== 33 (24 direct, 9 indirect) bytes in 1 blocks are definitely lost in loss record 3 of 4
    ==256664==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==256664==    by 0x1091EB: Person_create (lecture16.c:14)
    ==256664==    by 0x10936E: main (lecture16.c:41)
    ==256664==
    ==256664== 36 (24 direct, 12 indirect) bytes in 1 blocks are definitely lost in loss record 4 of 4
    ==256664==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==256664==    by 0x1091EB: Person_create (lecture16.c:14)
    ==256664==    by 0x109390: main (lecture16.c:43)
    ==256664==
    ==256664== LEAK SUMMARY:
    ==256664==    definitely lost: 48 bytes in 2 blocks
    ==256664==    indirectly lost: 21 bytes in 2 blocks
    ==256664==      possibly lost: 0 bytes in 0 blocks
    ==256664==    still reachable: 0 bytes in 0 blocks
    ==256664==         suppressed: 0 bytes in 0 blocks
    ==256664==
    ==256664== For lists of detected and suppressed errors, rerun with: -s
    ==256664== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
    ```
    
- 忘记释放Person_destory中的who->name，然后比较输出。再提醒一遍，使用正确的选项，让调试器告诉你程序中具体在哪里出错了。
    
    ```c
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    struct Person {
      char *name;
      int age;
      int height;
      int weight;
    };
    
    struct Person *Person_create(char *name, int age, int height, int weight) {
      struct Person *who = malloc(sizeof(struct Person));
      assert(who != NULL);
    
      who->name = strdup(name);
      who->age = age;
      who->height = height;
      who->weight = weight;
    
      return who;
    }
    
    void Person_destory(struct Person *who) {
      assert(who != NULL);
    
      // free(who->name);
      free(who);
    }
    
    void Person_print(struct Person *who) {
      printf("Name: %s\n", who->name);
      printf("\tAge: %d\n", who->age);
      printf("\tHeight: %d\n", who->height);
      printf("\tWeight: %d\n", who->weight);
    }
    int main(int argc, char *argv[]) {
    
      // make two people structures
      struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    
      struct Person *frank = Person_create("Frank Blank", 20, 72, 180);
    
      // print them out and where they are in memory
      printf("Joe is at memory location %p:\n", joe);
      Person_print(joe);
    
      printf("Frank is at memory location %p\n", frank);
      Person_print(frank);
    
      // make everyone age 20 years and print them out
      joe->age += 20;
      joe->height -= 2;
      joe->weight += 40;
      Person_print(joe);
    
      frank->age += 20;
      frank->weight += 20;
      Person_print(frank);
    
      // destory them both so we clean up
      Person_destory(joe);
      Person_destory(frank);
    
      return 0;
    }
    ```
    
    ```bash
    ❯ valgrind --leak-check=full ./lecture16
    ==262799== Memcheck, a memory error detector
    ==262799== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==262799== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==262799== Command: ./lecture16
    ==262799==
    Joe is at memory location 0x4a96040:
    Name: Joe Alex
            Age: 32
            Height: 64
            Weight: 140
    Frank is at memory location 0x4a960f0
    Name: Frank Blank
            Age: 20
            Height: 72
            Weight: 180
    Name: Joe Alex
            Age: 52
            Height: 62
            Weight: 180
    Name: Frank Blank
            Age: 40
            Height: 72
            Weight: 200
    ==262799==
    ==262799== HEAP SUMMARY:
    ==262799==     in use at exit: 21 bytes in 2 blocks
    ==262799==   total heap usage: 5 allocs, 3 frees, 1,093 bytes allocated
    ==262799==
    ==262799== 9 bytes in 1 blocks are definitely lost in loss record 1 of 2
    ==262799==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==262799==    by 0x491360E: strdup (strdup.c:42)
    ==262799==    by 0x10922A: Person_create (lecture16.c:17)
    ==262799==    by 0x10935F: main (lecture16.c:41)
    ==262799==
    ==262799== 12 bytes in 1 blocks are definitely lost in loss record 2 of 2
    ==262799==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==262799==    by 0x491360E: strdup (strdup.c:42)
    ==262799==    by 0x10922A: Person_create (lecture16.c:17)
    ==262799==    by 0x109381: main (lecture16.c:43)
    ==262799==
    ==262799== LEAK SUMMARY:
    ==262799==    definitely lost: 21 bytes in 2 blocks
    ==262799==    indirectly lost: 0 bytes in 0 blocks
    ==262799==      possibly lost: 0 bytes in 0 blocks
    ==262799==    still reachable: 0 bytes in 0 blocks
    ==262799==         suppressed: 0 bytes in 0 blocks
    ==262799==
    ==262799== For lists of detected and suppressed errors, rerun with: -s
    ==262799== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
    ```
    
- 这一次将NULL传给Person_print，看看调试器会有什么反应。你会发现NULL是一个让程序崩溃的快捷方法。
    
    ```c
    #include <assert.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    struct Person {
      char *name;
      int age;
      int height;
      int weight;
    };
    
    struct Person *Person_create(char *name, int age, int height, int weight) {
      struct Person *who = malloc(sizeof(struct Person));
      assert(who != NULL);
    
      who->name = strdup(name);
      who->age = age;
      who->height = height;
      who->weight = weight;
    
      return who;
    }
    
    void Person_destory(struct Person *who) {
      assert(who != NULL);
    
      free(who->name);
      free(who);
    }
    
    void Person_print(struct Person *who) {
      printf("Name: %s\n", who->name);
      printf("\tAge: %d\n", who->age);
      printf("\tHeight: %d\n", who->height);
      printf("\tWeight: %d\n", who->weight);
    }
    int main(int argc, char *argv[]) {
    
      // make two people structures
      struct Person *joe = Person_create("Joe Alex", 32, 64, 140);
    
      struct Person *frank = Person_create("Frank Blank", 20, 72, 180);
    
      // print them out and where they are in memory
      printf("Joe is at memory location %p:\n", joe);
      Person_print(joe);
    
      printf("Frank is at memory location %p\n", frank);
      Person_print(frank);
    
      // make everyone age 20 years and print them out
      joe->age += 20;
      joe->height -= 2;
      joe->weight += 40;
      Person_print(joe);
    
      frank->age += 20;
      frank->weight += 20;
      Person_print(frank);
    
      Person_print(NULL);
      // destory them both so we clean up
      Person_destory(joe);
      Person_destory(frank);
    
      return 0;
    }
    ```
    
    ```bash
    ❯ ./lecture16
    Joe is at memory location 0x5637a3d922a0:
    Name: Joe Alex
            Age: 32
            Height: 64
            Weight: 140
    Frank is at memory location 0x5637a3d922e0
    Name: Frank Blank
            Age: 20
            Height: 72
            Weight: 180
    Name: Joe Alex
            Age: 52
            Height: 62
            Weight: 180
    Name: Frank Blank
            Age: 40
            Height: 72
            Weight: 200
    [1]    269751 segmentation fault (core dumped)  ./lecture16
    ```
    

# 附加任务

在这部分中，我需要你试着做一点有难度的事情：将这个程序转换成不使用指针和malloc的版本。这个难度比较高，需要你先研究一下下面这些课题。

- 如何在栈（stack）上创建struct，就和你创建别的变量一样。
- 如何使用x.y（句点）而非x->y语法来初始化struct。
- 如何不使用指针将结构体传给别的函数。

貌似大一学的是这种在栈上创建结构体的方式，而不是直接在内存里创建。

```c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *name;
  int age;
  int height;
  int weight;
} Person;

Person Person_create(char *name, int age, int height, int weight) {
  Person who = {who.name = name, who.age = age, who.height = height,
                who.weight = weight};

  return who;
}

void Person_print(Person who) {
  printf("Name: %s\n", who.name);
  printf("\tAge: %d\n", who.age);
  printf("\tHeight: %d\n", who.height);
  printf("\tWeight: %d\n", who.weight);
}
int main(int argc, char *argv[]) {
  Person joe = Person_create("Joe Alex", 32, 64, 140);
  Person frank = Person_create("Frank Blank", 20, 72, 180);

  Person_print(joe);
  Person_print(frank);

  joe.age += 20;
  joe.height -= 2;
  joe.weight += 40;
  Person_print(joe);

  frank.age += 20;
  frank.weight += 20;
  Person_print(frank);

  return 0;
}
```

```c
❯ ./lecture16ex1
Name: Joe Alex
        Age: 32
        Height: 64
        Weight: 140
Name: Frank Blank
        Age: 20
        Height: 72
        Weight: 180
Name: Joe Alex
        Age: 52
        Height: 62
        Weight: 180
Name: Frank Blank
        Age: 40
        Height: 72
        Weight: 200
```