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
  for (i = 0; i < argc; i++) {

    for (j = 0; letter = argv[i][j], letter != '\0'; j++) {
      // char letter = argv[1][i];

      // In ASICII A stands for 65 and Z for 90.
      if (letter >= 65 && letter <= 90) {
        letter = letter + 32;
      }

      switch (letter) {
      case 'a':
        //    case 'A':
        printf("%d: 'A'\n", j);
        break;

      case 'e':
        //    case 'E':
        printf("%d: 'E'\n", j);
        break;

      case 'i':
        //    case 'I':
        printf("%d: 'I'\n", j);
        break;

      case 'o':
        //    case 'O':
        printf("%d: 'O'\n", j);
        break;

      case 'u':
        //    case 'U':
        printf("%d: 'U'\n", j);
        break;

      case 'y':
        //    case 'Y':
        if (j > 2) {
          // it's only sometimes y
          printf("%d: 'Y'\n", j);
          break;
        }
        //        break;

      default:
        printf("%d: %c is not a vowel \n", j, letter);
      }
    }
  }
  return 0;
}
