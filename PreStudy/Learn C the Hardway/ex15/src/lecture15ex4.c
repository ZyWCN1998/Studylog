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
