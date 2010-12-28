//merge too slow.

#define NULL ((void *)0)
#define EOS 0
#define EOF -1
#define ERR -1

#define BASE_SZ 3

/* I had size_t being an unsigned long before, but that led to the
 * infamous "Equality without matching types" error when I used a
 * size_t to index into an array. */
typedef int size_t;
typedef int bool;
#define true 1
#define false 0

/* Size of the input buffer. Since this example is a read overflow,
 * there is no output buffer. Must be at least 2 for things to work. */
#define INSZ BASE_SZ + 1

/* Size of a buffer used in gd_full.c; will affect a loop bound, so is
 * important for that example. */
#define ENTITY_NAME_LENGTH_MAX 8

/* The number of entities in entities[] and NR_OF_ENTITIES must
 * match. NR_OF_ENTITIES affects the number of iterations of search()
 * in gd_full_bad.c, so varying it should affect difficulty of that
 * example.
 *
 * Note that this is a *very* chopped-down array of entities -- see
 * entities.h in the gd sources for the real one. */
struct entities_s {
  char	*name;
  int	value;
};
struct entities_s entities[] = {
  {"AElig", 198},
  {"Aacute", 193},
  {"Acirc", 194},
};
#define NR_OF_ENTITIES 3

/* These things don't matter. */
#define Tcl_UniChar int
#define gdFTEX_Unicode 0
#define gdFTEX_Shift_JIS 1
#define gdFTEX_Big5 2


struct entities_s * search(struct entities_s *key,
                           struct entities_s *base,
                           int nmemb)
{
  int iter;
  for (iter = 0; iter < nmemb; iter++)
    if (strcmp (key->name, base[iter].name) == 0)
      return base + iter;
  return NULL;
}

#define N 5
int ndi[N];
int *ndip;

int strcmp (const char *s1, const char *s2)
{
  int i;
  for (i = 0; s1[i] == s2[i]; i++)
    if (s1[i] == EOS)
      return 0;
  return s1[i] - s2[i];
}

/* All we care about is what it returns, and that it doesn't enable us
 * to skip past the terminating EOS. */
static int
gdTcl_UtfToUniChar (char *str, Tcl_UniChar * chPtr)
{
  int byte;
  char entity_name_buf[ENTITY_NAME_LENGTH_MAX+1];
  char *p;
  struct entities_s key, *res;

  byte = (unsigned char)(str[0]);

  /* If we see an ampersand, treat what follows as an HTML4.0 entity, 
   * terminated with a semicolon 
   */
  if (byte == '&') { 
    int i, n = 0;
    
    byte = *((unsigned char *) (str + 1));
    if (byte == '#') {
      byte = *((unsigned char *) (str + 2));
      if (byte == 'x' || byte == 'X') {
        for (i = 3; i < 8; i++) {
          byte = *((unsigned char *) (str + i));
          if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
          else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
          else if (byte >= '0' && byte <= '9')
            byte = byte - '0';
          else
            break; /* end FOR */
          n = (n * 16) + byte;
        }
      }
      else {
        for (i = 2; i < 8; i++) {
          byte = (unsigned char)(str[i]);
          if (byte >= '0' && byte <= '9')
            n = (n * 10) + (byte - '0');
          else
            break;
        }
      }
      /* In each of the above branches, i was computed. It's the number of 
       * characters representing an HTML4.0 entity. Increment i since we're 
       * also consuming the semicolon.
       *
       * If this branch isn't taken, we're not dealing with an HTML4.0 
       * entity, so we go back to the start of str, and process it as 
       * UTF-8.
       */
      if (byte == ';') {
        return ++i;
      }
    }
    /* This isn't an HTML4.0 entity. But it could be an entity name --
     * they're listed in the entities[] array. So we read until we see
     * an EOS or a comma, and write what we're seeing into
     * entity_name_buf as we go, using it to do a search. 
     *
     * ...
     *
     * Ugh. That'll be hard. 
     *
     * What can we vary? The size of this entities[] buffer, I
     * guess. We'd definitely have to provide a bsearch stub -- which
     * would easily be a linear search, since we don't care about
     * runtime performance.
     *
     */
    else {
      key.name = p = entity_name_buf;
      for (i = 1; i < 1 + ENTITY_NAME_LENGTH_MAX; i++) {
        byte = (unsigned char)(str[i]);
        if (byte == EOS)
          break;
        if (byte == ';') {
          *p++ = EOS;
          /* This was originally bsearch(), which is a polymorphic
           * function that uses void pointers and size arguments. I
           * think most tools and tool users would rewrite the program
           * as a first phase, using type information to choose the
           * implementation of bsearch.
           */
          res = search(&key, entities, NR_OF_ENTITIES);
          if (res != NULL) {
            return ++i;
          }
          break;
        }
        *p++ = byte;
      } /* End FOR */
    }
  }

  /* This is the UTF-8 case. */
  byte = (unsigned char)(str[0]);
  if (byte < 0xC0) {
    return 1;
  }
  else if (byte < 0xE0) {
    if ((str[1] & 0xC0) == 0x80) {
      return 2;
    }
    return 1;
  }
  else if (byte < 0xF0) {
    if (((str[1] & 0xC0) == 0x80) && ((str[2] & 0xC0) == 0x80)) {
      return 3;
    }
    return 1;
  }

  return 1;
}



/* Greatly, GREATLY simplified. There's a bunch of cruft that doesn't
 * have to do with the manipulation of "string". */
void gdImageStringFTEx (char *string) {
  int next;
  int encoding;
  int i;
  int ch;
  int len;

  encoding = *ndip++;
  if (encoding > 2 || encoding < 0)
    return;

  next = 0;
  /* Unsafe read -- next can be out of bounds. */
  /* BAD */
__TESTCLAIM_1:
  for (i=0; string[next] != EOS; i++)
    {
      /* grabbing a character and storing it in an int
       *
       * this'll fill the low-order byte, and keep more space free for
       * extra bytes for Unicode encoding, etc.
       */
      ch = string[next];

      /* carriage returns */
      if (ch == '\r')
	{
	  next++;
	  continue;
	}
      /* newlines */
      if (ch == '\n')
	{
	  next++;
	  continue;
	}


      switch (encoding)
        {
        case gdFTEX_Unicode:
          {
            len = gdTcl_UtfToUniChar (string + next, &ch);
            next += len;
          }
          break;
        case gdFTEX_Shift_JIS:
          {
            unsigned char c;
            c = (unsigned char) string[next];
            if (0xA1 <= c && c <= 0xFE)
              {
                next++;
              }
            /* We're incrementing next twice, which could make us skip
             * the terminating EOS character. The read of "string"
             * could then be out of bounds. */
            next++;
          }
          break;
        case gdFTEX_Big5:
          {
            ch = (string[next]) & 0xFF;	/* don't extend sign */
            next++;
            if (ch >= 161
                && string[next] != EOS)
              {
                next++;
              }
          }
          break;
        }
    }
}

int main ()
{
  char in [INSZ];
  klee_make_symbolic(in, sizeof(in), "in");
  in [INSZ-1] = EOS;

  klee_make_symbolic(ndi, sizeof(ndi), "ndi");
  ndip = ndi;

  gdImageStringFTEx (in);

  return 0;
}

