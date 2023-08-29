#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 1;
  // int j = 1;
  while (i <= 9) {
    int j = 1;
    while (j <= i) {
      printf("%d x %d = %d\t", i, j, i * j);
      j++;
    }
    // printf("i = %d\n", i);
    i++;
    printf("\n");
  }
  return 0;
}
