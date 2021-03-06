/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Bill Chapman

****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#define __NO_STRING_INLINES  /* long strcmp macro in asserts gets c89 upset */
#include <string.h>

#include "wnlib.h"
#include "wnasrt.h"
#include "wnnop.h"
#include "wncmp.h"
#include "wncpy.h"
#include "wnmem.h"

#include "wnskl.h"
#include "wnskll.h"


#define SIZE  100000


local void lo_test_main(void)
{
  wn_sklist sklist;
  wn_skhandle handle;

  wn_gpmake("general_free");

    wn_mkintsklist(&sklist, .33);

    wn_skins(&handle, sklist, (ptr)  8);
    wn_skins(&handle, sklist, (ptr)  4);
    wn_skins(&handle, sklist, (ptr) 17);
    wn_skins(&handle, sklist, (ptr) 11);
    wn_skins(&handle, sklist, (ptr)  2);

    handle = WN_SKIP_NEXTS(sklist->handle)[0];
    wn_assert(handle->key == (ptr) 2);

    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(handle->key == (ptr) 4);

    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(handle->key == (ptr) 8);

    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(handle->key == (ptr) 11);

    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(handle->key == (ptr) 17);

  wn_gpfree();
} /* lo_test_main */


#define LO_MIN_KEY 123
#define LO_MAX_KEY 12345

#define LO_MIN_SECONDARY_KEY 201
#define LO_MAX_SECONDARY_KEY 257

local int lo_scores[LO_MAX_KEY+1];

local void lo_test_act(wn_skhandle handle)
{
  ++ lo_scores[(long) handle->key];
}

local void lo_test_sklist_copy(void)
{
  wn_sklist sklist;
  wn_skhandle handle;
  int i, j;

  wn_gpmake("general_free");

    wn_mkintsklist(&sklist, .33);

    for (i = LO_MIN_KEY;  i <= LO_MAX_KEY;  ++i)
    {
      wn_skins(&handle, sklist, (ptr) i);
      handle->contents = (ptr) (i + 2);
    }

    wn_skverify(sklist);

    wn_skget(&handle, sklist, (ptr) 0, WN_SK_MIN);
    wn_assert(handle->key == (ptr) LO_MIN_KEY);

    wn_skget(&handle, sklist, (ptr) 0, WN_SK_MAX);
    wn_assert(handle->key == (ptr) LO_MAX_KEY);

    wn_skget(&handle, sklist, (ptr) (LO_MIN_KEY - 5), WN_SK_LT);
    wn_assert(!handle);

    wn_skget(&handle, sklist, (ptr) (LO_MIN_KEY - 5), WN_SK_LE);
    wn_assert(!handle);

    wn_skget(&handle, sklist, (ptr) (LO_MIN_KEY - 5), WN_SK_EQ);
    wn_assert(!handle);

    wn_skget(&handle, sklist, (ptr) (LO_MIN_KEY - 5), WN_SK_GE);
    wn_assert(LO_MIN_KEY == (long) handle->key);

    wn_skget(&handle, sklist, (ptr) (LO_MIN_KEY - 5), WN_SK_GT);
    wn_assert(LO_MIN_KEY == (long) handle->key);

    wn_skget(&handle, sklist, (ptr) LO_MIN_KEY, WN_SK_LT);
    wn_assert(!handle);

    wn_skget(&handle, sklist, (ptr) LO_MAX_KEY, WN_SK_GT);
    wn_assert(!handle);

    wn_skget(&handle, sklist, (ptr) (LO_MAX_KEY + 5), WN_SK_LT);
    wn_assert(LO_MAX_KEY == (long) handle->key);

    wn_skget(&handle, sklist, (ptr) (LO_MAX_KEY + 5), WN_SK_LE);
    wn_assert(LO_MAX_KEY == (long) handle->key);

    wn_skget(&handle, sklist, (ptr) (LO_MAX_KEY + 5), WN_SK_GE);
    wn_assert(!handle);

    wn_skget(&handle, sklist, (ptr) (LO_MAX_KEY + 5), WN_SK_GT);
    wn_assert(!handle);

    for (i = LO_MIN_KEY;  i <= LO_MAX_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_EQ);
      wn_assert((ptr) i == handle->key);

      wn_skget(&handle, sklist, (ptr) i, WN_SK_LE);
      wn_assert((ptr) i == handle->key);

      wn_skget(&handle, sklist, (ptr) i, WN_SK_GE);
      wn_assert((ptr) i == handle->key);
    }

    for (i = LO_MIN_KEY+1;  i <= LO_MAX_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_LT);
      wn_assert((ptr) (i-1) == handle->key);
    }

    for (i = LO_MIN_KEY;  i <= LO_MAX_KEY-1;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_GT);
      wn_assert((ptr) (i+1) == handle->key);
    }

    for (i = 0;  i <= LO_MAX_SECONDARY_KEY;  ++i)
    {
      j = (i * 11) % (LO_MAX_SECONDARY_KEY + 1);
      if (j >= LO_MIN_SECONDARY_KEY  &&  j <= LO_MAX_SECONDARY_KEY)
      {
	wn_skins(&handle, sklist, (ptr) j);
	handle->contents = (ptr) (j+4);
      }
    }

    wn_skverify(sklist);

    for (i = 0;  i <= LO_MAX_KEY;  ++i)
    {
      lo_scores[i] = 0;
    }
    wn_skact(sklist, lo_test_act, (ptr) LO_MIN_KEY, WN_SK_GE,
    /**/			     (ptr) LO_MAX_KEY, WN_SK_LE);

    for (i = 0;  i < LO_MIN_KEY;  ++i)
    {
      wn_assert(0 == lo_scores[i]);
    }
    for (i = LO_MIN_KEY;  i < LO_MIN_SECONDARY_KEY;  ++i)
    {
      wn_assert(1 == lo_scores[i]);
    }
    for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
    {
      wn_assert(2 == lo_scores[i]);
    }
    for (i = LO_MAX_SECONDARY_KEY+1;  i <= LO_MAX_KEY;  ++i)
    {
      wn_assert(1 == lo_scores[i]);
    }

    for (i = LO_MIN_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_EQ);

      wn_skdel(handle, sklist);
    }

    wn_skverify(sklist);

    for (i = LO_MIN_KEY;  i < LO_MIN_SECONDARY_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_EQ);

      wn_assert(!handle);
    }
    for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_EQ);

      wn_assert((ptr) i == handle->key);
    }

    for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_EQ);

      wn_skmove(handle, sklist, (ptr) -i);
    }

    wn_skverify(sklist);

    for (i = LO_MIN_SECONDARY_KEY;  i <= LO_MAX_SECONDARY_KEY;  ++i)
    {
      wn_skget(&handle, sklist, (ptr) i, WN_SK_EQ);

      wn_assert(!handle);

      wn_skget(&handle, sklist, (ptr) -i, WN_SK_EQ);

      wn_assert((ptr) -i == handle->key);
    }

#if 0
    for (i = 0;  i <= LO_MAX_SECONDARY_KEY - LO_MIN_SECONDARY_KEY;  ++i)
    {
      wn_skget_handle_of_index(&handle, sklist, i);

      wn_assert(handle->key == (ptr) (i - LO_MAX_SECONDARY_KEY));

      wn_skget_index_of_handle(&j, sklist, handle);

      wn_assert(i == j);
    }
#endif

    wn_assert(LO_MAX_KEY + 1 - LO_MIN_SECONDARY_KEY == wn_skcount(sklist));

    wn_skverify(sklist);

    wn_freesklist(sklist);

  wn_gpfree();
} /* lo_test_sklist_copy */


local void lo_test_sklist_string(void)
{
  wn_sklist sklist;
  wn_skhandle handle;

  wn_gpmake("general_free");

    wn_mkstrsklist(&sklist, 0.25);

    wn_skins(&handle, sklist, (ptr) "a");
    wn_skins(&handle, sklist, (ptr) "BB");
    wn_skins(&handle, sklist, (ptr) "c");
    wn_skins(&handle, sklist, (ptr) "b");
    wn_skins(&handle, sklist, (ptr) "cdg");
    wn_skins(&handle, sklist, (ptr) "ca");
    wn_skins(&handle, sklist, (ptr) "ddd");

    handle = WN_SKIP_NEXTS(sklist->handle)[0];
    wn_assert(!strcmp((char *) handle->key, "BB"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!strcmp((char *) handle->key, "a"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!strcmp((char *) handle->key, "b"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!strcmp((char *) handle->key, "c"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!strcmp((char *) handle->key, "ca"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!strcmp((char *) handle->key, "cdg"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!strcmp((char *) handle->key, "ddd"));
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!handle);

    wn_freesklist(sklist);

  wn_gpfree();
} /* lo_test_sklist_string */


local void lo_test_sklist_double(void)
{
  wn_sklist sklist;
  wn_skhandle handle;
  double a;

  wn_gpmake("general_free");

    wn_mkdoublesklist(&sklist, 0.25);

    a = 1.75;  wn_skins(&handle, sklist, (ptr) &a);
    a = 6;     wn_skins(&handle, sklist, (ptr) &a);
    a = 0.5;   wn_skins(&handle, sklist, (ptr) &a);
    a = 2.5;   wn_skins(&handle, sklist, (ptr) &a);
    a = 10;    wn_skins(&handle, sklist, (ptr) &a);
    a = 7.25;  wn_skins(&handle, sklist, (ptr) &a);
    a = 0;     wn_skins(&handle, sklist, (ptr) &a);

    handle = WN_SKIP_NEXTS(sklist->handle)[0];
    wn_assert(0    == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(0.5  == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(1.75 == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(2.5  == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(6    == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(7.25 == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(10   == * (double *) handle->key);
    handle = WN_SKIP_NEXTS(handle)[0];
    wn_assert(!handle);

    wn_freesklist(sklist);

  wn_gpfree();
} /* lo_test_sklist_double */


local void lo_test_for_skl(void)
{
  wn_sklist sklist;
  wn_skhandle handle;
  static int results[][2] = { {2, 4},    {4, 10}, {8, 16},
  /**/			      {11, 20}, {17, 34}, {9, 99} };
  int i;

  WN_GPBEGIN("general_free") {
    wn_gplabel("test_for_skl_1");

    /* we only need one group, this is just to test if they nest */
    WN_GPBEGIN("general_free") {
      wn_gplabel("test_for_skl_2");

      wn_mkintsklist(&sklist, .33);

      wn_skins(&handle, sklist, (ptr)  8);
      handle->contents = (ptr) 16;
      wn_skins(&handle, sklist, (ptr)  4);
      handle->contents = (ptr)  10;
      wn_skins(&handle, sklist, (ptr) 17);
      handle->contents = (ptr) 34;
      wn_skins(&handle, sklist, (ptr) 11);
      handle->contents = (ptr) 20;
      wn_skins(&handle, sklist, (ptr)  2);
      handle->contents = (ptr)  4;

      i = 0;
      WN_FOR_SKL(long, v, sklist, long, k)
	wn_assert(k == results[i][0]);
	wn_assert(v == results[i][1]);

	++ i;
      WN_END_FOR_SKL()

      i = 0;
      WN_FOR_SKL(long, v, sklist, long, k)
	++ i;

	if (2 == i)
	/**/	    break;
      WN_END_FOR_SKL()
      wn_assert(2 == i);

    } WN_GPEND();
  } WN_GPEND();
} /* lo_test_for_skl */


int main(void)
{

  printf("testing skip list...\n");

  lo_test_main();

  lo_test_sklist_copy();

  lo_test_sklist_string();

  lo_test_sklist_double();

  lo_test_for_skl();

  printf("  ok!!!!!!\n");

  return 0;
} /* main */
