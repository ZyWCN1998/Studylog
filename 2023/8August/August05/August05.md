# 20230805 Learn C the Hardway15 02

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
      for (i = count - 1; i > 0; i--) {
        printf("%s is %d years old. \n", *(cur_name + i), *(cur_age + i));
      }
    
      printf("---\n");
    
      // third way, pointers are just arrays
      for (i = count - 1; i > 0; i--) {
        printf("%s is %d years old again. \n", cur_name[i], cur_age[i]);
      }
    
      printf("---\n");
    
      // fourth way with pointers in a stupid complex way
      for (cur_name = names + count - 1, cur_age = ages + count - 1;
           (cur_age - ages) > 0; cur_name--, cur_age--) {
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