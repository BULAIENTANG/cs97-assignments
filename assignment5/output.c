#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include "./output.h"

bool
writebytes (unsigned long long x, int nbytes, int blockSize)
{
  if(blockSize < nbytes) {
    while (nbytes > 0) {
      unsigned long long *xptr;
      xptr = (unsigned long long *) malloc(blockSize);
      if(xptr == NULL)
        return false;
      xptr = &x;
      write(1, xptr, blockSize);
      nbytes -= blockSize;
    }
    return true;
  }

  do
    {
      if (putchar (x) < 0)
	      return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}