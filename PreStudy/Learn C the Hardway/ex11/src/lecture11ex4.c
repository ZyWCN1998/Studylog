#include <stdio.h>

int main(int argc, char *argv[]) {
  int number = 0;
  char name[4] = {0, 0, 0, 0};

  int i = 0;
  for (i = 0; i < 4; i++) {
    name[i] = i;
  }

  number = (name[0] << 24) + (name[1] << 16) + (name[2] << 8) + name[0];
  printf("%d", number);

  return 0;
}
