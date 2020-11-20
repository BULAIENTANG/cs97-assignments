#ifndef OPTION_H
#define OPTION_H
#include <stdbool.h>

struct opts {
  bool valid;
  long long nbytes;
  char* data_provided;
  unsigned int blockSize;
  bool iflag;
  bool oflag;
  char* iargs;
  char* oargs;

  bool hw_gen;
  bool sw_gen;
  bool stdout;
  bool Nout;
};

void read_options(
  int argc,
  char* argv[],
  struct opts* opts
);

#endif