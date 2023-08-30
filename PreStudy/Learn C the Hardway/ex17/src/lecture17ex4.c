#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
  int id;
  int set;
  char sex;
  char name[MAX_DATA];
  char email[MAX_DATA];
};

int main(int argc, char *argv[]) {
  printf("Size of Address is %ld\n", sizeof(struct Address));
  return 0;
}
