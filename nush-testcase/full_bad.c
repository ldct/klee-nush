//no matter what, merge is slow. !@#$
#define NULL ((void *)0)
#define EOS 0
#define EOF -1
#define ERR -1

#define BASE_SZ 5

#define LDAP "ldap"
#define LDAP_SZ 4

#define TOKEN_SZ BASE_SZ + 1
#define URI_SZ LDAP_SZ + 1 + 1 + TOKEN_SZ + 2

typedef unsigned int size_t;

unsigned strlen(char *s)
{
  int i;
  i = 0; 
  while (s[i] != EOS)
    ++i;
  klee_merge();
  return i;
}

int strncmp (const char *s1, const char *s2, size_t n)
{
  int i;
  int retval;
  i = 0;
  do {
    retval = s1[i] - s2[i];
    if (i >= n-1) return retval;
    if (retval != 0) return retval;
    if (s1[i] == EOS) return 0;
    i++;
  } while (1);
}

void escape_absolute_uri (char *uri, int scheme)
{
  int cp;
  char *token[TOKEN_SZ];
  int c;

  if (scheme == 0
      || strlen(uri) < scheme) {
    return;
  }

  cp = scheme;

  if (uri[cp-1] == '/') {
    while (uri[cp] != EOS
           && uri[cp] != '/') {
      ++cp;
    }

    if (uri[cp] == EOS || uri[cp+1] == EOS) return;
    ++cp;

    scheme = cp;

    if (strncmp(uri, LDAP, LDAP_SZ) == 0) {
      c = 0;
      token[0] = uri;
      while (uri[cp] != EOS
             && c < TOKEN_SZ) {
        if (uri[cp] == '?') {
          ++c;
          token[c] = uri + cp + 1;
          uri[cp] = EOS;
        }
        ++cp;
      }
      return;
    }
  }

  return;
}

int main ()
{
  char uri [URI_SZ];
  int scheme;

  klee_make_symbolic(uri, sizeof(uri), "uri");

  uri [URI_SZ-1] = EOS;
  scheme = LDAP_SZ + 2;

  escape_absolute_uri (uri, scheme);

  return 0;
}