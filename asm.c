#include <stdio.h>

int line = 1;
int col = 1;

char current_token[64] = "";

// print error
static int error(const char *type, const char *msg) {
  fprintf(stderr, "%s error: %s\n", type, msg);
  return 0;
}

// string equal
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

// peek char
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

static int is_comma(char ch) {
  return (ch == ',');
}

// @returns 0 for success / 1 for failure
static int maketoken(FILE *f) {
  // empty
  for (int i = 0; i < 64; i++) {
    current_token[i] = '\0';
  }
  char ch = fgetc(f);
  // newline
  if (is_nl(ch)) {
    current_token[0] = '\n';
    line++;
    col = 1;
  }
  // whitespace
  else if (is_ws(ch)) {
    current_token[0] = ' ';
    // greedy
    while (is_ws(peekc(f))) {
      fgetc(f);
    }
  }
  // comma
  else if (is_comma(ch)) {
    current_token[0] = ',';
  }
  // default
  else {
    int len = 0;
    ungetc(ch, f);
    // greedy
    while (1) {
      current_token[len] = fgetc(f);
      len++;
      char next = peekc(f);
      if (is_nl(next) || is_ws(next) || is_comma(next)) {
        break;
      }
    }
  }
  printf("%s\n", current_token); // debug
  return 0;
}

int main(int argc, char *argv[]) {
  FILE *src = fopen(argv[1], "r");
  FILE *dst = fopen(argv[2], "wb");
  while (1) {
    char ch = peekc(src);
    if (feof(src)) {
      break;
    }
    // don't care
    if (ch == '\r') {
      fgetc(src);
      continue;
    }
    // token
    else {
      maketoken(src);
    }
  }
  fclose(src);
  return 0;
}
