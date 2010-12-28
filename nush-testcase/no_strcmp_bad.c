//OPTIMUM IS 1

#define BASE_SZ 5

/* Size of the buffer being overflowed */
#define USERSZ BASE_SZ

/* One less than the number of iterations the first FOR loop must go
 * through in order to hit the error. 
 *
 * XXX Not sure how this affects analysis difficulty, except that the
 * XXX bigger this is, the bigger INSZ has to be.*/
#define J 2

/* Must read at least NPFLEN characters and contain the string NPF in
 * order to cal ftpls(). */
#define NPF "N"
#define NPFSZ 1

/* Size of the input buffer. Also affects a loop bound. */
#define INSZ USERSZ + NPFSZ + J + 2

#define NULL ((void *)0)
#define EOS 0
#define EOF -1
#define ERR -1

typedef unsigned int size_t;
typedef int bool;
#define true 1
#define false 0

char *strchr(const char *s, int c)
{
  int i;
  for (i = 0; s[i] != EOS; i++)
    if (s[i] == c)
      return &s[i];

  return (c == EOS) ? &s[i] : NULL;
}

char *r_strcpy (char *dest, const char *src)
{
  int i;
  char tmp;
  for (i = 0; ; i++) {
    tmp = src[i];
    /* r_strcpy RELEVANT */
    dest[i] = tmp; // DO NOT CHANGE THE POSITION OF THIS LINE
    if (src[i] == EOS)
      break;
  }
  return dest;
}


void ftpls (char *line)
{
    int j;

    /* Stop at either:
     *  (1) first char before EOS which isn't in "-rwxdls", or,
     *  (2) first EOS
     */
    for(j = 0; line[j] != EOS; ++j) {
      if (!strchr("-", line[j]))  //change to rwxdls IF YOU ADD r YOU DECREASE TIME!!! ZOMG!!!111
        break;
    }
    if(j == J && line[j] == ' ') {	/* long list */
      /* BUG! No bounds check. */
      char user[USERSZ];
      /* BAD */
      r_strcpy (user, line + j);
    }
}

#define N 8
int main ()
{
  char out [INSZ];
  int out_l;
  bool dirmode;
  static const char npf[] = NPF;
  const int npfsize = NPFSZ;
  int c;

  int ndi[N];
  klee_make_symbolic(ndi, sizeof(ndi), "ndi");
  ndi[N-1] = EOF;
  
  int *p = ndi;

  dirmode = false;

  out_l = 0;
  out[INSZ-1] = EOS;

top:

  while((c = *p++) != EOF) {
    if(c == '\r')
      c = '\n';
    if(c == '\n') {
      if(dirmode) {
        ftpls(out);
      } else {
        if(!out_l)
          continue;
        if (out_l > npfsize) {
          dirmode = true;
          goto top;
        }
      }
      out_l = 0;
    } else {
      out[out_l] = c;
      out_l++;
      if (out_l > INSZ-1)
        return ERR;
    }
  }

  return 0;
}
