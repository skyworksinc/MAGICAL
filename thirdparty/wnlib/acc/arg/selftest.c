/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Will Naylor, Bill Chapman

****************************************************************************/

#include <stdio.h>
#define __NO_STRING_INLINES  /* long strcmp macro in asserts gets c89 upset */
#include <string.h>

#include "wnlib.h"
#include "wnargp.h"
#include "wnasrt.h"

/* ---------------------------------------------------------------- */
/* this test was mostly copied from the original man page */

local void lo_man_page_test(void)
{
  int argc;
  char **argv;
  static bool recover;
  static char *control_filename,*error_filename,*in_filename,*out_filename;

  static wn_arg_format arg_format_array[] =
  {
    WN_KEY_BOOL(&recover,"r"),
    WN_KEY_STRING(&control_filename,"g","control_file",WN_REQUIRED),
    WN_KEY_STRING(&error_filename,"log","error_file",WN_OPTIONAL),
    WN_POS_STRING(&in_filename,"in_file",WN_REQUIRED),
    WN_POS_STRING(&out_filename,"out_file",WN_OPTIONAL),
    WN_ARG_TERMINATE
  };

  error_filename = "dastoinf.log";  /* defaults */
  in_filename = "net.inf";

  wn_parse_into_args(&argc, &argv, "selftest myreqfile.txt -g myreqf2.txt");
  wn_parse_args(argc,argv,arg_format_array);

  wn_assert(!recover);
  wn_assert(!strcmp(control_filename, "myreqf2.txt"));
  wn_assert(!strcmp(error_filename, "dastoinf.log"));
  wn_assert(!strcmp(in_filename, "myreqfile.txt"));
  wn_assert(!out_filename);

  wn_parse_into_args(&argc, &argv,
  /**/	"selftest my.txt -r -g my2.txt -log myerrfile.log myoutfile.out");
  wn_parse_args(argc,argv,arg_format_array);

  wn_assert(recover == TRUE);
  wn_assert(!strcmp(control_filename, "my2.txt"));
  wn_assert(!strcmp(error_filename, "myerrfile.log"));
  wn_assert(!strcmp(in_filename, "my.txt"));
  wn_assert(!strcmp(out_filename, "myoutfile.out"));
} /* lo_man_page_test */


local void lo_test_new_types(void)
{
  int argc;
  char **argv, **argv2;
  static bool a = TRUE, b = TRUE, c = TRUE;
  static int si = 5;
  static unsigned su = 2000000000;	/*     gcc can't cope with large
  **					** unsigned constants */
  static double d = 9.0;

  static wn_arg_format arg_format_array[] =
  {
    WN_KEY_BOOL(&a, "a"),
    WN_KEY_BOOL(&b, "b"),
    WN_KEY_BOOL(&c, "c"),
    WN_KEY_INT(&si, "si", "si_units", WN_OPTIONAL),
    WN_KEY_UNSIGNED(&su, "u", "u_units", WN_OPTIONAL),
    WN_KEY_DOUBLE(&d, "d", "d_units", WN_OPTIONAL),
    WN_ARG_TERMINATE
  };

  /* everything's optional with defaults set, let's try them */
  wn_parse_into_args(&argc, &argv, "selftest");
  wn_argscpy(&argv2, argc, argv);
  wn_parse_args(argc, argv2, arg_format_array);

  wn_assert(!a && !b && !c);
  wn_assert(5 == si);
  wn_assert(2000000000 == su);
  wn_assert(9.0 == d);

  wn_freeargs(argc, argv2);

  wn_parse_into_args(&argc, &argv,
  /**/			"selftest -a -b -si -6 -u 2000000001 -d 7e1");
  wn_parse_args(argc, argv, arg_format_array);

  wn_assert(a && b && !c);
  wn_assert(-6 == si);
  wn_assert(2000000001 == su);
  wn_assert(70.0 == d);
} /* lo_test_new_types */


int main(int argc, char *argv[])
{
  fprintf(stderr, "Testing arg stuff...\n");

  lo_man_page_test();
  lo_test_new_types();

  fprintf(stderr,"  ok!!!!!!\n");

  return 0;
} /* main */
