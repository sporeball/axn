#include <stdio.h>

int line = 1;
int col = 1;

static char peekc(FILE *f) {
  char next = fgetc(f);
  ungetc(next, f);
  return next;
}

static int is_ws(char ch) {
  return (ch == ' ' || ch == '\t');
}

static int is_nl(char ch) {
  return (ch == '\n');
}

static char* maketoken(FILE *f) {
  int len = 0;
  char token[64] = "";
  while (1) {
    token[len] = fgetc(f);
    len++;
    char next = peekc(f);
    if (is_ws(next) || is_nl(next)) {
      break;
    }
  }
  printf("%s\n", token);
  return token;
}

int main(int argc, char *argv[]) {
  FILE *src = fopen(argv[1], "r");
  FILE *dst = fopen(argv[2], "wb");
  while (1) {
    char ch = peekc(src);
    if (feof(src)) {
      break;
    }
    if (ch == '\r') {
      continue;
    } else if (ch == '\n') {
      fgetc(src);
      line++;
      col = 1;
      continue;
    } else if (is_ws(ch)) {
      while (is_ws(peekc(src))) {
        fgetc(src);
      }
      continue;
    } else {
      maketoken(src);
    }
  }
  fclose(src);
  return 0;
}
