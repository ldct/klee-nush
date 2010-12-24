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

#define N 12

main()
{
        long total = 0;
        int minus = 0;

        char nptr_buf[N];
        klee_make_symbolic(nptr_buf, sizeof(nptr_buf), "nptr");
        nptr_buf[N-1] = 0;

        int i = 0;  

        while (isspace(nptr_buf[i])) i++;

        if (nptr_buf[i] == '+') i++;
        else if (nptr_buf[i] == '-') {
                minus = 1;
                i++;
        }

        while (isdigit(nptr_buf[i])) {
                total *= 10;
                total += (nptr_buf[i] - '0');
                i++;
        }
        return minus ? -total : total;
}
