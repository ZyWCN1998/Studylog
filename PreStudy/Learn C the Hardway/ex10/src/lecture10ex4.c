#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("Error: You need another argument. \n");
    // This is how you abort a program
    return 1;
  }
  int i = 0;
  int j = 0;
  char letter;
  for (i = 1; i < argc; i++) {
    for (j = 0; letter = argv[i][j], letter != '\0'; j++) {
      // char letter = argv[1][i];

      // In ASICII A stands for 65 and Z for 90.
      if (letter >= 65 && letter <= 90) {
        letter = letter + 32;
      }
      if (letter == 'a') {
        printf("%d: 'A'\n", j);
      } else if (letter == 'e') {
        printf("%d: 'E'\n", j);
      } else if (letter == 'i') {
        printf("%d: 'I'\n", j);
      } else if (letter == 'o') {
        printf("%d: 'O'\n", j);
      } else if (letter == 'u') {
        printf("%d: 'U'\n", j);
      } else {
        printf("%d: %c is not a vowel \n", j, letter);
      }
    }
  }
  return 0;
}
