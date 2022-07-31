#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *src = fopen(argv[1], "r");
  FILE *dst = fopen(argv[2], "wb");
  while (1) {
    if (feof(src)) {
      break;
    }
    char ch = fgetc(src);
    printf("%c", ch);
  }
  fclose(src);
  return 0;
}
