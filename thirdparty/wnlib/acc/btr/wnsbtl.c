/**************************************************************************

wn_mkstrbtree(&tree)

**************************************************************************/
/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Will Naylor

****************************************************************************/
#include <string.h>

#include "wnlib.h"
#include "wnnop.h"
#include "wnmem.h"
#include "wncmp.h"
#include "wncpy.h"
#include "wnstr.h"

#include "wnbtr.h"
#include "wnbtrl.h"



void wn_mkstrbtree(wn_btree *ptree)
{
  wn_mkbtree(ptree,
	     (int (*)(ptr,ptr))(strcmp),
	     (void (*)(ptr *,ptr))(wn_stracpy),
	     (void (*)(ptr))(wn_free));
}

