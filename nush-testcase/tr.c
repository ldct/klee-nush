/* tr - translate characters            Author: Michiel Huisjes */
/* Usage: tr [-cds] [string1 [string2]]
 *      c: take complement of string1
 *      d: delete input characters coded string1
 *      s: squeeze multiple output characters of string2 into one character
 */

#define BUFFER_SIZE     1024
#define USER_SIZE       4
#define ASCII           0377

typedef char BOOL;
#define TRUE    1
#define FALSE   0

#define NIL_PTR         ((char *) 0)

BOOL com_fl, del_fl, sq_fl;

unsigned char output[BUFFER_SIZE], input[BUFFER_SIZE];
unsigned char vector[ASCII + 1];
BOOL invec[ASCII + 1], outvec[ASCII + 1];
char user_input[USER_SIZE];

int my_read_called = 0;
short in_index, out_index;




#include <stdlib.h>

int main(int argc, char **argv);
void convert(void);
void map(unsigned char *string1, unsigned char *string2);
void expand(char *arg, unsigned char *buffer);
void complement(unsigned char *buffer);

int my_read() {
  if (my_read_called == 0) {
    strcpy(user_input, input);
    my_read_called++;
    return strlen(input);
  }
  else {
    return -1;
  }
}

int main(int argc, char *argv[])
{
  unsigned char *ptr;
  int index = 1;
  short i;
  
  klee_make_symbolic(user_input, sizeof(user_input), "user input");
  user_input[USER_SIZE - 1] = '\n';

  if (argc > 1 && argv[index][0] == '-') {
        for (ptr = (unsigned char *) &argv[index][1]; *ptr; ptr++) {
                switch (*ptr) {
                    case 'c':   com_fl = TRUE;  break;
                    case 'd':   del_fl = TRUE;  break;
                    case 's':   sq_fl = TRUE;   break;
                    default:
                        write(2,"Usage: tr [-cds] [string1 [string2]].\n", 38);
                        exit(1);
                }
        }
        index++;
  }
  for (i = 0; i <= ASCII; i++) {
        vector[i] = i;
        invec[i] = outvec[i] = FALSE;
  }

  if (argv[index] != NIL_PTR) {
        expand(argv[index++], input);
        if (com_fl) complement(input);
        if (argv[index] != NIL_PTR) expand(argv[index], output);
        if (argv[index] != NIL_PTR) map(input, output);
        for (ptr = input; *ptr; ptr++) invec[*ptr] = TRUE;
        for (ptr = output; *ptr; ptr++) outvec[*ptr] = TRUE;
  }
  convert();
  return(0);
}

void convert()
{
  short read_chars = 0;
  short c, coded;
  short last = -1;

  for (;;) {
         if (in_index == read_chars) {
                 if ((read_chars = my_read()) <= 0) {
                         if (write(1, (char *)output, out_index) != out_index)
                                 write(2, "Bad write\n", 10);
                         exit(0);
                 }
                 in_index = 0;
         }

        c = input[in_index++];
        coded = vector[c];
        if (del_fl && invec[c]) continue;
        if (sq_fl && last == coded && outvec[coded]) continue;
        output[out_index++] = last = coded;
        if (out_index == BUFFER_SIZE) {
                if (write(1, (char *)output, out_index) != out_index) {
                        write(2, "Bad write\n", 10);
                        exit(1);
                }
                out_index = 0;
        }
  }

  /* NOTREACHED */
}

void map(string1, string2)
unsigned char *string1, *string2;
{
  unsigned char last;

  while (*string1) {
        if (*string2 == '\0')
                vector[*string1] = last;
        else
                vector[*string1] = last = *string2++;
        string1++;
  }
}

void expand(arg, buffer)
char *arg;
unsigned char *buffer;
{
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
  }
}

char *strcpy(char *dest, const char *src)
{
   char *save = dest;
   while(*dest++ = *src++);
   return save;
}

size_t strlen(const char *org)
{
  const char *s = org;
  while (*s++);
  return --s - org;
}



void complement(buffer)
unsigned char *buffer;
{
  unsigned char *ptr;
  short i, index;
  unsigned char conv[ASCII + 2];

  index = 0;
  for (i = 1; i <= ASCII; i++) {
        for (ptr = buffer; *ptr; ptr++)
                if (*ptr == i) break;
        if (*ptr == '\0') conv[index++] = i & ASCII;
  }
  conv[index] = '\0';
  strcpy((char *)buffer, (char *)conv);
}

