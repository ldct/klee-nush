#include "klee/klee.h"

#define N 5

int main() {
  
  char arg_buf[N];
  unsigned char buffer_buf[100];

  klee_make_symbolic(arg_buf, sizeof(arg_buf), "arg");
  arg_buf[N-1] = '\0';

  char *arg = arg_buf;
  unsigned char *buffer = buffer_buf;

  int i, ac;

  while (*arg) {
        if (*arg == '\\') {
                arg++;
                i = ac = 0;
                if (*arg >= '0' && *arg <= '7') {
                        do {
                                ac = (ac << 3) + *arg++ - '0';
                                i++;
                        } while (i < 4 && *arg >= '0' && *arg <= '7');
                        *buffer++ = ac;
                } else if (*arg != '\0')
                        *buffer++ = *arg++;
        } else if (*arg == '[') {
                arg++;
                i = *arg++;
                if (*arg++ != '-') {
                        *buffer++ = '[';
                        arg -= 2;
                        continue;
                }
                ac = *arg++;
                while (i <= ac) *buffer++ = i++;
                arg++;          /* Skip ']' */
        } else
                *buffer++ = *arg++;
        klee_merge();
  }
}
