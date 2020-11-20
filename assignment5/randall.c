/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

#include "./output.h"
#include "./options.h"
#include "./rand64-hw.h"
#include "./rand64-sw.h"

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  struct opts options;
  options.valid = false;
  options.iflag = false;
  options.oflag = false;
  options.hw_gen = false;
  options.sw_gen = false;
  options.stdout = false;
  options.Nout = false;

  read_options(argc, argv, &options);

  // printf("hi\n");
  // printf("%lld\n", options.nbytes);
  // printf("%d\n", options.blockSize);
  // printf("%s\n", options.iargs);
  // printf("%s\n", options.oargs);

  if (!options.valid)
    {
      fprintf (stderr, "%s: wrong usage\n", argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (options.nbytes == 0)
    return 0;


  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize_hw) (void);
  void (*initialize_sw) (char*);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  if (options.hw_gen && rdrand_supported ())
    {
      initialize_hw = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
      initialize_hw ();
    }
  else if (options.hw_gen && !rdrand_supported ()){
    fprintf (stderr, "%s: hardware random-number generation is not supported.\n", argv[0]);
    return 1;
  }
  else
    {
      initialize_sw = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
      initialize_sw (options.data_provided);
    }
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
  {
    unsigned long long x = rand64 ();
    int outbytes = options.nbytes < wordsize ? options.nbytes : wordsize;
    if (!writebytes (x, outbytes, options.blockSize))
    {
      output_errno = errno;
      break;
    }
    options.nbytes -= outbytes;
  }
  while (0 < options.nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
  {
    errno = output_errno;
    perror ("output");
  }

  finalize ();
  return !!output_errno;
}
