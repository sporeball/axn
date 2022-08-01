#include <stdio.h>

int line = 1;
int col = 1;

static int s_eq(char *a, char *b) {
  int i = 0;
  while (1) {
    if (a[i] != b[i]) {
      return 0;
    }
    if (a[i] == '\0') {
      return 1;
    }
    i++;
  }
}

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
  char token[64] = "";
  char ch = fgetc(f);
  if (is_nl(ch)) {
    token[0] = '\n';
    line++;
    col = 1;
  } else if (is_ws(ch)) {
    token[0] = ' ';
    while (is_ws(peekc(f))) {
      fgetc(f);
    }
  } else {
    int len = 0;
    ungetc(ch, f);
    while (1) {
      char next = peekc(f);
      if (is_nl(next) || is_ws(next)) {
        break;
      }
      token[len] = fgetc(f);
      len++;
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
      fgetc(src);
      continue;
    } else {
      maketoken(src);
    }
  }
  fclose(src);
  return 0;
}
