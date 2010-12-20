#include <klee/klee.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main(int argc,char *argv[]){
  char name[1024];
  char keyfile[1024];
  klee_make_symbolic(name, sizeof(name), "name");
  klee_make_symbolic(keyfile, sizeof(keyfile), "keyfile");
  int fd,ret=1,err,i;
  
  struct stat statbuf;

  fd=open(keyfile,O_RDONLY);
  if(fd==-1)
    {
      fprintf(stderr,"Cannot open key file %s: %s\n",keyfile,strerror(errno));
      return 1;
    }

  err=fstat(fd,&statbuf);
  if(err==-1)
    {
      fprintf(stderr,"Unable to stat key file %s: %s\n",
	      keyfile,strerror(errno));
      goto fail;
    }

  if(statbuf.st_size>65536)
    {
      fprintf(stderr,"Key %s too large for CERT encoding\n",keyfile);
      goto fail;
    }

  if(statbuf.st_size>16384)
    fprintf(stderr,"Warning: key file %s is larger than the default"
	    " GnuPG max-cert-size\n",keyfile);

  printf("%s\tTYPE37\t\\# %u 0003 0000 00 ",
	 name,(unsigned int)statbuf.st_size+5);

  err=1;
  while(err!=0)
    {
      unsigned char buffer[1024];

      err=read(fd,buffer,1024);
      if(err==-1)
	{
	  fprintf(stderr,"Unable to read key file %s: %s\n",
		  keyfile,strerror(errno));
	  goto fail;
	}

      for(i=0;i<err;i++)
	printf("%02X",buffer[i]);
    }

  printf("\n");

  ret=0;

 fail:
  close(fd);

  return ret;
}
