//optimum is 2, 3 will screw up, 1 still ok

#define BASE_SZ 2

/* (One less than the) size of the buffer being overflowed. */
#define MAXPATHLEN BASE_SZ

/* Make PATTERNLEN bigger than MAXPATHLEN -- we want to be able to overflow 
 * the buffer of length MAXPATHLEN+1 without having a tool complain about 
 * out-of-bounds reads of the pattern buffer.
 */
#define PATTERNLEN MAXPATHLEN+5

/* Size of d_name. We don't care about it; like PATTERNLEN, just make
 * it "big enough".
 */
#define MAXNAMLEN (MAXPATHLEN * 4)

#define	ismeta(c)	(((c)&(0x80)) != 0)

typedef int Char;

#define NULL ((void *)0)
#define EOS 0
#define EOF -1
#define ERR -1
#define	SEP		'/'

typedef unsigned int size_t;
typedef int bool;
#define true 1
#define false 0

int glob2 (Char *pathbuf, Char *pathend, Char *pathlim, Char *pattern)
{
  int i;
  int anymeta;
  Char tmp;

  for (anymeta = 0;;) {

    /* Copies a single string from pattern into pathend, checking for 
     * the presence of meta-characters.
     */
    i = 0;
    while (pattern[i] != EOS && pattern[i] != SEP) {
      if (ismeta(pattern[i]))
        anymeta = 1;
      if (pathend + i >= pathlim)
        return 1;
      tmp = pattern[i];
      /* BAD */
      pathend[i] = tmp;
      i++;
    }

    if (!anymeta) {
      pathend = pathend + i;
      pattern = pattern + i;
      while (*pattern == SEP) {
        // bounds check
        if (pathend >= pathlim)
          return 1;
        tmp = *pattern;
        /* BAD */
        *pathend = tmp;
        pathend++;
        pattern++;
      }
    } else {
      // stand-in for glob3 (which is recursive)
      return 0;
    }
  }

  /* NOT REACHED */
}

int main ()
{
  Char *buf;
  Char *pattern;
  Char *bound;

  Char A [MAXPATHLEN+1];
  Char B [PATTERNLEN];
  
  klee_make_symbolic(A, sizeof(A), "A");
  klee_make_symbolic(B, sizeof(B), "B");

  buf = A;
  pattern = B;

  bound = A + sizeof(A) - 1;

  glob2 (buf, buf, bound, pattern);

  return 0;
}
