# 20230731 Learn C the Hardway15 01

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