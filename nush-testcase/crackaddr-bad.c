#define MACROEXPAND	((unsigned char)0201)	/* macro expansion */
#define MAXNAME		30		/* max length of a name */
#define EOS 0
#define NULL ((void *)0)

enum bool {false, true}; 


int	ColonOkInAddr;	/* single colon legal in address */
char	*MustQuoteChars;	/* quote these characters in phrases */

char *strcpy (char *dest, char *src)
{
  int i;
  char tmp;
  for (i = 0; ; i++) {
    tmp = src[i];
    dest[i] = tmp;
    if (src[i] == EOS)
      break;
  }
  return dest;
}

char *strchr(const char *s, int c)
{
  int i;
  for (i = 0; s[i] != EOS; i++)
    if (s[i] == c)
      return &s[i];

  return (c == EOS) ? &s[i] : NULL;
}

int isspace (int c)
{
  return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
      || c == ' ');
}


char *
crackaddr(addr)
	register char *addr;
{
	register char *p;
	register char c;
	int cmtlev;
	int realcmtlev;
	int anglelev, realanglelev;
	int copylev;
	int bracklev;
	enum bool qmode;
	enum bool realqmode;
	enum bool skipping;
        enum bool putgmac = false;
	enum bool quoteit = false;
	enum bool gotangle = false;
	enum bool gotcolon = false;
	register char *bp;
	char *obp;
	char *buflim;
	char *bufhead;
	char *addrhead;
	static char buf[MAXNAME + 1];
	static char test_buf[10]; /* will use as a canary to detect overflow */
	                          /* of buf[] */
	
	strcpy(test_buf, "GOOD");

	/* strip leading spaces */
	while (*addr != '\0' && isascii((int)*addr) && isspace((int)*addr))
		addr++;

	/*
	**  Start by assuming we have no angle brackets.  This will be
	**  adjusted later if we find them.
	*/


	bp = bufhead = buf;
	obp = bp;
	buflim = &buf[sizeof buf - 7];
	p = addrhead = addr;
	copylev = anglelev = realanglelev = cmtlev = realcmtlev = 0;
	bracklev = 0;
	qmode = realqmode = false;

	while ((c = *p++) != '\0')
	{
		/*
		**  If the buffer is overful, go into a special "skipping"
		**  mode that tries to keep legal syntax but doesn't actually
		**  output things.
		*/

	  skipping = (unsigned)(bp >= buflim);
              
		if (copylev > 0 && !(unsigned)skipping)
		      { 
		      /*BAD*/
		      __TESTCLAIM_1:
		      *bp++ = c;
		      }


		/* check for backslash escapes */
		if (c == '\\')
		{
			/* arrange to quote the address */
			if (cmtlev <= 0 && !(unsigned)qmode)
				quoteit = true;

			if ((c = *p++) == '\0')
			{
				/* too far */
				p--;
				goto putg;
			}
			if (copylev > 0 && !(unsigned)skipping)
			  {  
			    /*BAD*/
			    *bp++ = c;
			  }
			goto putg;
		}

		/* check for quoted strings */
		if (c == '"' && cmtlev <= 0)
		{
		  qmode = (unsigned)!(unsigned)qmode;
			if (copylev > 0 && !(unsigned)skipping)
			  realqmode = (unsigned)!(unsigned)realqmode;
			continue;
		}
		if ((unsigned)qmode)
			goto putg;

		/* check for comments */
		if (c == '(')
		{
			cmtlev++;

			/* allow space for closing paren */
			if (!(unsigned)skipping)
			{
				buflim--;
				realcmtlev++;
				if (copylev++ <= 0)
				{
					if (bp != bufhead)
					  {
					    /*BAD*/
					    *bp++ = ' ';
					  }
				     
					/*BAD*/
					*bp++ = c;
				}
			}
		}
		if (cmtlev > 0)
		{
			if (c == ')')
			{
				cmtlev--;
				copylev--;
				if (!(unsigned)skipping)
				{
					realcmtlev--;
					buflim++;
				}
			}
			continue;
		}
		else if (c == ')')
		{
			/* syntax error: unmatched ) */
			if (copylev > 0 && !((unsigned)skipping))
				bp--;
		}

		/* count nesting on [ ... ] (for IPv6 domain literals) */
		if (c == '[')
			bracklev++;
		  else if (c == ']')
			bracklev--;

		/* check for group: list; syntax */
		if (c == ':' && anglelev <= 0 && bracklev <= 0 &&
		    !((unsigned)gotcolon) && !((unsigned)ColonOkInAddr))
		{		        
			register char *q;

			if (*p == ':' || *p == '.')
			{
				if (cmtlev <= 0 && !((unsigned)qmode))
					quoteit = true;
				if (copylev > 0 && !((unsigned)skipping))
				{
				  /*BAD*/
				  *bp++ = c;
				  /*BAD*/
				  *bp++ = *p;
				}
				p++;
				goto putg;
			}

			gotcolon = true;

			bp = bufhead;
			if ((unsigned)quoteit)
			{
			  /*BAD*/
			  *bp++ = '"';

			  /* back up over the ':' and any spaces */
			  --p;
			  
			  while (isascii((int) *--p) && isspace((int) *p))
				  continue;
			  p++;
			}
			for (q = addrhead; q < p; )
			{
				c = *q++;
				if (bp < buflim)
				{
					if ((unsigned)quoteit && c == '"')
					  /*BAD*/	
					  *bp++ = '\\';
					/*BAD*/
					*bp++ = c;
				
				}
			}
			if ((unsigned)quoteit)
			{
				if (bp == &bufhead[1])
					bp--;
				else{
				  /*BAD*/
				  *bp++ = '"';
				}
				while ((c = *p++) != ':')
				  {
				    if (bp < buflim){
				      /*BAD*/
				      *bp++ = c;
				    }
				  }
				/*BAD*/
				*bp++ = c;
			}

			/* any trailing white space is part of group: */
			while (isascii((int) *p) && isspace((int)*p) && bp < buflim)
			  {
			    /*BAD*/
			    *bp++ = *p++;
			  }
			copylev = 0;
			putgmac = quoteit = false;
			bufhead = bp;
			addrhead = p;
			continue;
		}

		if (c == ';' && copylev <= 0 && !((unsigned)ColonOkInAddr))
		{
			if (bp < buflim)
			  /*BAD*/
			  *bp++ = c;
		}

		/* check for characters that may have to be quoted */
		if (strchr(MustQuoteChars, c) != NULL)
		{
			/*
			**  If these occur as the phrase part of a <>
			**  construct, but are not inside of () or already
			**  quoted, they will have to be quoted.  Note that
			**  now (but don't actually do the quoting).
			*/

			if (cmtlev <= 0 && !(unsigned)qmode)
				quoteit = true;
		}

		/* check for angle brackets */
		if (c == '<')
		{
			register char *q;

			/* assume first of two angles is bogus */
			if ((unsigned)gotangle)
				quoteit = true;
			gotangle = true;

			/* oops -- have to change our mind */
			anglelev = 1;
			if (!(unsigned)skipping)
				realanglelev = 1;

			bp = bufhead;
			if ((unsigned)quoteit)
			  {
			    /*BAD*/
			    *bp++ = '"';
			    
			    /* back up over the '<' and any spaces */
			    --p;
			    while (isascii((int)*--p) && isspace((int)*p))
			      continue;
			    p++;
			  }
			for (q = addrhead; q < p; )
			  {
				c = *q++;
				if (bp < buflim)
				  {
				    if ((unsigned)quoteit && c == '"')
				      /*BAD*/
				      *bp++ = '\\';
				    /*BAD*/
				    *bp++ = c;
				}
			}
			if ((unsigned)quoteit)
			{
			  if (bp == &buf[1])
				  bp--;
				else
				  /*BAD*/
				  *bp++ = '"';
			  while ((c = *p++) != '<')
			    {
			      if (bp < buflim)
				/*BAD*/
				*bp++ = c;
			    }
			  /*BAD*/
			  *bp++ = c;
			}
			copylev = 0;
			putgmac = quoteit = false;
			continue;
		}

		if (c == '>')
		{
			if (anglelev > 0)
			{
				anglelev--;
				if (!(unsigned)skipping)
				{
					realanglelev--;
					buflim++;
				}
			}
			else if (!(unsigned)skipping)
			{
				/* syntax error: unmatched > */
				if (copylev > 0)
					bp--;
				quoteit = true;
				continue;
			}
			if (copylev++ <= 0)
			  /*BAD*/
			  *bp++ = c;
			continue;
		}

		/* must be a real address character */
	putg:
		if (copylev <= 0 && !(unsigned)putgmac)
		  {
		    if (bp > bufhead && bp[-1] == ')')
		      /*BAD*/
		      *bp++ = ' ';
		    /*BAD*/
		    *bp++ = MACROEXPAND;
		    /*BAD*/
		    *bp++ = 'g';
		    putgmac = true;
		  }
	}
	
	/* repair any syntactic damage */
	if ((unsigned)realqmode)
	  /*BAD*/
	  *bp++ = '"';
	while (realcmtlev-- > 0)
	  /*BAD*/
	  *bp++ = ')';
	while (realanglelev-- > 0)
	  /*BAD*/
	  *bp++ = '>';
	/*BAD*/
	*bp++ = '\0';
	
	return buf;
}


int main(){

  char address[5];
  klee_make_symbolic (address, sizeof(address), "address");
  char *res_addr;

  MustQuoteChars = "@,;:\\()[].'";

  res_addr = crackaddr(address);

  return 0;
}

/*

</source>

*/

