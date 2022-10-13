#include <stdio.h>

int line = 1;
int col = 1;

typedef struct {
  char *type;
  char value[64];
  int length;
} Token;

static const Token empty_token;
Token current_token;

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

static int tok_append(char ch) {
  if (current_token.length == 64) {
    error("token", "too long");
    return 1;
  }
  current_token.value[current_token.length] = ch;
  current_token.length++;
  return 0;
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

static int is_semi(char ch) {
  return (ch == ';');
}

static int is_sep(char ch) {
  return (is_ws(ch) || is_nl(ch) || is_comma(ch) || is_semi(ch));
}

static int is_digit(char ch) {
  return (ch >= '0' && ch <= '9');
}

static int is_letter(char ch) {
  return (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z');
}

// @returns 0 for success / 1 for failure
static int maketoken(FILE *f) {
  char ch = peekc(f);
  // comment
  if (is_semi(ch)) {
    // greedy drop
    while (!is_nl(peekc(f))) {
      fgetc(f);
    }
  }
  // go again
  ch = fgetc(f);
  current_token = empty_token;
  tok_append(ch);
  // newline
  if (is_nl(ch)) {
    current_token.type = "newline";
    line++;
    col = 1;
  }
  // whitespace
  else if (is_ws(ch)) {
    current_token.type = "whitespace";
    // greedy drop
    while (is_ws(peekc(f))) {
      fgetc(f);
    }
  }
  // comma
  else if (is_comma(ch)) {
    current_token.type = "comma";
  }
  // default
  else {
    // greedy keep
    while (1) {
      char next = peekc(f);
      if (is_sep(next)) {
        break;
      }
      fgetc(f);
      if (tok_append(next) == 1) {
        error("token", "could not create token");
        return 1;
      }
    }
  }
  printf("%s (%s)\n", current_token.type, current_token.value); // debug
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
      if (maketoken(src) == 1) {
        error("assembly", "failed to assemble ROM");
        return 1;
      }
    }
  }
  fclose(src);
  return 0;
}
