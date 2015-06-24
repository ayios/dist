 /*
 * This code was written by Agathoklis Hatzimanikas
 * You may distribute under the terms of the GNU General Public
 * License.
 */

#include <limits.h>
#include <stdlib.h>
#include <slang.h>

SLANG_MODULE(ayios);

static void path_real (char *path)
{
   char *p;
   
   p = realpath (path, p);
   
   if (p == NULL)
     (void) SLang_push_null ();
   else
     (void)SLang_push_string (p);
}

static SLang_Intrin_Fun_Type Ayios_Intrinsics [] =
{
   MAKE_INTRINSIC_S("realpath", path_real, SLANG_VOID_TYPE),

   SLANG_END_INTRIN_FUN_TABLE
};

int init_ayios_module_ns (char *ns_name)
{
   SLang_NameSpace_Type *ns;

   if (NULL == (ns = SLns_create_namespace (ns_name)))
     return -1;

   if (-1 == SLns_add_intrin_fun_table (ns, Ayios_Intrinsics, NULL))
     return -1;

   return 0;
}
