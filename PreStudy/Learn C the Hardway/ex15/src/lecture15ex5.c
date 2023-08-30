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
