#include <ctype.h>
#include <stdio.h>
#include <string.h>
// forward declarations
// int can_print_it(char ch);
void print_letters(char arg[], int lenth);
void print_arguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {

  print_arguments(argc, argv);

  return 0;
}

void print_arguments(int argc, char *argv[]) {
  int i = 0;
  int lenth = 0;

  for (i = 0; i < argc; i++) {
    lenth = strlen(argv[i]);
    print_letters(argv[i], lenth);
  }
}

void print_letters(char arg[], int lenth) {
  int i = 0;

  for (i = 0; i < lenth; i++) {
    char ch = arg[i];

    if ((isalpha(ch) || isblank(ch))) {
      printf("'%c' == %d ", ch, ch);
    }
  }
  printf("\n");
}

// int can_print_it(char ch) { return isalpha(ch) || isblank(ch); }
