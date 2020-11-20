#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "./options.h"

void read_options(int argc, char **argv, struct opts* options)
{
  // get options
  char c;
  while((c = getopt(argc, argv, "i:o:")) != -1) {
    switch (c)
    {
      case 'i':
        options->iflag = true;
        options->iargs = optarg;
        if(!strcmp(options->iargs, "rdrand"))
          options->hw_gen = true;
        else if(!strcmp(options->iargs, "mrand48_r"))
          options->sw_gen = true;
        else if(options->iargs[0] == '/') {
          options->sw_gen = true;
          options->data_provided = options->iargs;
        }
        options->valid = true; 
        break;
      case 'o':
        options->oflag = true;
        options->oargs = optarg;
        if(!strcmp(options->oargs, "stdio")) 
          options->stdout = true;
        else
        {
          if(atoi(options->oargs) > 0) {
            options->Nout = true;
            options->blockSize = atoi(options->oargs);
          }
        }
        options->valid = true;
        break;
      case '?':
      case ':':
        break;
    }
  }

  if (optind >= argc) {
    return;
  }
  options->nbytes = atol(argv[optind]);
  if (options->nbytes >= 0) {
    options->valid = true;
  }

  //default blockSize
  if (options->blockSize == 0)
    options->blockSize = options->nbytes;

}