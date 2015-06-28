 /*
 * This code was written by Agathoklis D. Hatzimanikas
 * You may distribute under the terms of the GNU General Public
 * License.
 */

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/mount.h>
#include <slang.h>

SLANG_MODULE(ayios);

static int push_grp_struct (struct group *grent)
{
#define NUM_GR_FIELDS 4
   static SLFUTURE_CONST char *field_names[NUM_GR_FIELDS] =
     {
     "gr_name", "gr_gid", "gr_passwd", "gr_mem",
     };
   SLtype field_types[NUM_GR_FIELDS];
   VOID_STAR field_values[NUM_GR_FIELDS];
   SLang_Array_Type *at;
   SLindex_Type idx;
   
   int ndx;
   int i;

   i = 0;

   field_values[i] = &grent->gr_name;
   field_types[i] =  SLANG_STRING_TYPE;

   i++;

   field_values[i] = &grent->gr_gid;
   field_types[i] = SLANG_INT_TYPE;

   i++;

   field_values[i] = grent->gr_passwd;
   field_types[i] = SLANG_NULL_TYPE;

   i++;
   
   for (ndx = 0; grent->gr_mem[ndx] != NULL; ndx++);
   
   idx = ndx;

   at = SLang_create_array (SLANG_STRING_TYPE, 0, NULL, &idx, 1);

   for (idx = 0; grent->gr_mem[idx] != NULL; idx++)
     if (-1 == SLang_set_array_element (at, &idx, &grent->gr_mem[idx]))
       return;

   field_values[i] = &at;
   field_types[i] = SLANG_ARRAY_TYPE;

   (void) SLstruct_create_struct (NUM_GR_FIELDS, field_names, field_types, field_values);

   if (at != NULL)
     SLang_free_array (at);
}

static void getgrname_intrin (char *name)
{
  struct group grent;
  struct group *grentp;
  char *buf;
  size_t bufsize;
  int retval;
  
  bufsize = sysconf (_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)
     bufsize = 16384;

  buf = malloc (bufsize);
  
  retval = getgrnam_r (name, &grent, buf, bufsize, &grentp);

  if (grentp == NULL)
    {
    SLerrno_set_errno (retval);
    (void) SLang_push_null ();
    }
  else
     push_grp_struct (&grent);
}

static void getgrgid_intrin (int *gid)
{
  struct group grent;
  struct group *grentp;
  char *buf;
  size_t bufsize;
  int retval;
  
  bufsize = sysconf (_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)
     bufsize = 16384;

  buf = malloc (bufsize);
  
  retval = getgrgid_r ((gid_t) *gid, &grent, buf, bufsize, &grentp);

  if (grentp == NULL)
    {
    SLerrno_set_errno (retval);
    (void) SLang_push_null ();
    }
  else
     push_grp_struct (&grent);
}

typedef struct
  {
  struct passwd pw;
  } Pwd_Type;

static SLang_CStruct_Field_Type Pwd_Struct [] =
{
   MAKE_CSTRUCT_FIELD(Pwd_Type, pw.pw_name, "pw_name", SLANG_STRING_TYPE, 0),
   MAKE_CSTRUCT_FIELD(Pwd_Type, pw.pw_passwd, "pw_passwd", SLANG_STRING_TYPE, 0),
   MAKE_CSTRUCT_INT_FIELD(Pwd_Type, pw.pw_uid, "pw_uid", 0),
   MAKE_CSTRUCT_INT_FIELD(Pwd_Type, pw.pw_gid, "pw_gid", 0),
   MAKE_CSTRUCT_FIELD(Pwd_Type, pw.pw_gecos, "pw_gecos", SLANG_STRING_TYPE, 0),
   MAKE_CSTRUCT_FIELD(Pwd_Type, pw.pw_dir, "pw_dir", SLANG_STRING_TYPE, 0),
   MAKE_CSTRUCT_FIELD(Pwd_Type, pw.pw_shell, "pw_shell", SLANG_STRING_TYPE, 0),
   SLANG_END_CSTRUCT_TABLE
};

static int push_pwd_struct (struct passwd *pwent)
{
   Pwd_Type s;

   s.pw = *pwent;
   return SLang_push_cstruct ((VOID_STAR) &s, Pwd_Struct);
}

static void getpwuid_intrin (int *uid)
{
  struct passwd pwent;
  struct passwd *pwentp;
  char *buf;
  size_t bufsize;
  int retval;
  
  bufsize = sysconf (_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)
     bufsize = 16384;

  buf = malloc (bufsize);
  
  retval = getpwuid_r ((uid_t) *uid, &pwent, buf, bufsize, &pwentp);

  if (pwentp == NULL)
    {
    SLerrno_set_errno (retval);
    (void) SLang_push_null ();
    }
  else
     push_pwd_struct (&pwent);
}

static void getpwnan_intrin (char *name)
{
  struct passwd pwent;
  struct passwd *pwentp;
  char *buf;
  size_t bufsize;
  int retval;
  
  bufsize = sysconf (_SC_GETPW_R_SIZE_MAX);
  if (bufsize == -1)
     bufsize = 16384;

  buf = malloc (bufsize);
   
  retval = getpwnam_r (name, &pwent, buf, bufsize, &pwentp);

  if (pwentp == NULL)
    {
    SLerrno_set_errno (retval);
    (void) SLang_push_null ();
    }
  else
     push_pwd_struct (&pwent);
}

static void realpath_intrin (char *path)
{
   char *p;
   
   p = realpath (path, p);
   
   if (p == NULL)
     {
     (void) SLang_push_null ();
     return;
     }
   else
     (void)SLang_push_string (p);

  free (p);
}

static SLang_Intrin_Fun_Type Ayios_Intrinsics [] =
{
   MAKE_INTRINSIC_S("realpath", realpath_intrin, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_S("getpwnam", getpwnan_intrin, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_I("getpwuid", getpwuid_intrin, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_S("getgrnam", getgrname_intrin, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_I("getgrgid", getgrgid_intrin, SLANG_VOID_TYPE),

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
