#include <klee/klee.h>
#define _DIGIT          0x4
#define _SPACE          0x8
#define _BLANK          0x40
#define _HEX            0x80

unsigned short _ctype[257] = 
{
  _SPACE+_BLANK,          // 20 SPACE
  _DIGIT+_HEX,            // 30 0
  _DIGIT+_HEX,            // 31 1
  _DIGIT+_HEX,            // 32 2
  _DIGIT+_HEX,            // 33 3
  _DIGIT+_HEX,            // 34 4
  _DIGIT+_HEX,            // 35 5
  _DIGIT+_HEX,            // 36 6
  _DIGIT+_HEX,            // 37 7
  _DIGIT+_HEX,            // 38 8
  _DIGIT+_HEX,            // 39 9
  // and the rest are 0...
};

unsigned short *_pctype = _ctype + 1;

int isdigit(int c)
{
  return _pctype[c] & _DIGIT;
}

int isspace(int c)
{
  return _pctype[c] & _SPACE;
}

long atol(register const char *nptr)
{
        long total = 0;
        int minus = 0;

        while (isspace(*nptr)) nptr++;
        if (*nptr == '+') nptr++;
        else if (*nptr == '-') {
                minus = 1;
                nptr++;
        }
        klee_merge();
        while (isdigit(*nptr)) {
                total *= 10;
                total += (*nptr++ - '0');
        }
        klee_merge();
        return minus ? -total : total;
}

long
find_value (char *start, long int length)
{
  while (length != 0L)
    {
      if (isdigit (*start))
        return atol (start);
      klee_merge();
      length--;
      start++;
    }
  klee_merge();
  return 0l;
}

struct keyfield
{
  int startwords;       /* Number of words to skip. */
  int startchars;       /* Number of additional chars to skip. */
  int endwords;         /* Number of words to ignore at end. */
  int endchars;         /* Ditto for characters of last word. */
  char ignore_blanks;   /* Non-zero means ignore spaces and tabs. */
  char fold_case;       /* Non-zero means case doesn't matter. */
  char reverse;         /* Non-zero means compare in reverse order. */
  char numeric;         /* Non-zeros means field is ASCII numeric. */
  char positional;      /* Sort according to file position. */
  char braced;          /* Count balanced-braced groupings as fields. */
};

#define SIZE 4

int
main ()
{
  struct keyfield keyfield[SIZE];
  char start1[SIZE];
  long int length1=SIZE;
  long int pos1=0;
  char start2[SIZE];
  long int length2=SIZE;
  long int pos2=0;
  klee_make_symbolic(keyfield, sizeof(keyfield), "*keyfield");
  klee_make_symbolic(start1, sizeof(start1), "*start1");
  klee_make_symbolic(start2, sizeof(start2), "*start2");

  int char_order[256];
  int i;
  for (i = 1; i < 256; i++)
    char_order[i] = i;

  for (i = '0'; i <= '9'; i++)
    char_order[i] += 512;

  for (i = 'a'; i <= 'z'; i++)
    {
      char_order[i] = 512 + i;
      char_order[i + 'A' - 'a'] = 512 + i;
  }

  if (keyfield->positional)
    {
      if (pos1 > pos2)
        return 1;
      else
        return -1;
      klee_merge();
    }
  if (keyfield->numeric)
    {
      long value = find_value (start1, length1) - find_value (start2, length2);
      if (value > 0)
        return 1;
      if (value < 0)
        return -1;
      return 0;
    }
  else
    {
      char *p1 = start1;
      char *p2 = start2;
      char *e1 = start1 + length1;
      char *e2 = start2 + length2;

      while (1)
        {
          int c1, c2;

          if (p1 == e1)
            c1 = 0;
          else
            c1 = *p1++;
          klee_merge();
          if (p2 == e2)
            c2 = 0;
          else
            c2 = *p2++;
          klee_merge();
          if (char_order[c1] != char_order[c2])
            return char_order[c1] - char_order[c2];
          if (!c1)
            break;
        }

      /* Strings are equal except possibly for case.  */
      p1 = start1;
      p2 = start2;
      while (1)
        {
          int c1, c2;

          if (p1 == e1)
            c1 = 0;
          else
            c1 = *p1++;
          klee_merge();
          if (p2 == e2)
            c2 = 0;
          else
            c2 = *p2++;
          klee_merge();
          if (c1 != c2)
            /* Reverse sign here so upper case comes out last.  */
            return c2 - c1;
          if (!c1)
            break;
        }

      return 0;
    }
}
