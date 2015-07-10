 /*
 * This code was written by Agathoklis D. Hatzimanikas
 * with ideas from various sources around the OS universe
 * and from S-Lang sources study  
 * You may distribute it under the terms of the GNU General Public
 * License.
 */

# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <security/pam_appl.h>
# include <string.h>
# include <pwd.h>
# include <grp.h>
# include <sys/file.h>
# include <sys/stat.h>
# include <errno.h>
# include <slang.h>

SLANG_MODULE(ayios);

static int initgroups_intrin (char *user, int *gid)
{
   int retval;
   retval = initgroups (user, (gid_t) *gid);
   if (-1 == retval)
     (void) SLerrno_set_errno (errno);

  return retval;
}

int custom_converation (int num_msg, const struct pam_message** msg, struct pam_response** resp, void* appdata_ptr)
{
   struct pam_response* reply = (struct pam_response* ) malloc (sizeof (struct pam_response));
   reply[0].resp = (char*) appdata_ptr;
   reply[0].resp_retcode = 0;

   *resp = reply;

   return PAM_SUCCESS;
}

static int auth_intrin (char *user, char* pass) 
{
   char* password = (char*) malloc (strlen (pass) + 1);

   strcpy (password, pass);

   struct pam_conv pamc = {custom_converation, password};
   pam_handle_t* pamh; 
   int retval;
   
   if ((retval = pam_start ("exit", user, &pamc, &pamh)) == PAM_SUCCESS)
     {
     retval = pam_authenticate (pamh, 0);
     
     if (pam_end (pamh, 0) != PAM_SUCCESS)
       return -1; 

     return retval == PAM_SUCCESS ? 0 : -1;
     }
   else
     return -1;
}

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
    (void) SLerrno_set_errno (retval);
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
    (void) SLerrno_set_errno (retval);
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
    (void) SLerrno_set_errno (retval);
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
    (void) SLerrno_set_errno (retval);
    (void) SLang_push_null ();
    }
  else
     push_pwd_struct (&pwent);
}

static void realpath_intrin (char *path)
{
   int path_max;

#ifdef PATH_MAX
  path_max = PATH_MAX;
#else
  path_max = pathconf (path, _PC_PATH_MAX);
  if (path_max <= 0)
    path_max = 4096;
#endif

   char p[path_max];
   
   if (realpath (path, p) == 0)
     {
     (void) SLerrno_set_errno (errno);
     (void) SLang_push_null ();
     }
   else
     (void) SLang_push_string (p);
}

static int fileexists_intrin (char *path)
{
  if (-1 == access (path, F_OK))
    return 0;

  return 1;
}

static int istype (int st_mode, char *what)
{
   int ret;

// stat_is() code from upstream
   if (!strcmp (what, "sock")) ret = S_ISSOCK(st_mode);
   else if (!strcmp (what, "fifo")) ret = S_ISFIFO(st_mode);
   else if (!strcmp (what, "blk")) ret = S_ISBLK(st_mode);
   else if (!strcmp (what, "chr")) ret = S_ISCHR(st_mode);
   else if (!strcmp (what, "dir")) ret = S_ISDIR(st_mode);
   else if (!strcmp (what, "reg")) ret = S_ISREG(st_mode);
   else if (!strcmp (what, "lnk")) ret = S_ISLNK(st_mode);
   else return 0;

   return (char) (ret != 0);
}

static int istype_intrin (void)
{
   char *what;
   int ret;
   int st_mode;

   if (-1 == SLang_pop_slstring (&what))
     return 0;

   if (SLANG_NULL_TYPE == SLang_peek_at_stack ())
     {
     SLang_pop_null ();
     return 0;
     }
   
   if (-1 == SLang_pop_int (&st_mode))
     return 0;
  
   return istype (st_mode, what);
}

static int _isdirectory_intrin (char *dir)
{
  int ret = fileexists_intrin (dir);
  if (0 == ret)
    return 0;

  struct stat st;
 
  ret = lstat (dir, &st);
  
  if (-1 == ret)
   {
   (void) SLerrno_set_errno (errno);
   return 0;
   }

  return istype (st.st_mode, "dir");
}

static int __isdirectory_intrin (char *dir)
{
  int ret = fileexists_intrin (dir);
  if (0 == ret)
    return 0;

  struct stat st;
 
  ret = stat (dir, &st);
  
  if (-1 == ret)
   {
   (void) SLerrno_set_errno (errno);
   return 0;
   }

  return istype (st.st_mode, "dir");
}

static SLang_CStruct_Field_Type Fstat_Struct [] =
{
   MAKE_CSTRUCT_INT_FIELD(struct stat, st_dev, "st_dev", 0),
   MAKE_CSTRUCT_INT_FIELD(struct stat, st_ino, "st_ino", 0),
   MAKE_CSTRUCT_INT_FIELD(struct stat, st_mode, "st_mode", 0),
   MAKE_CSTRUCT_INT_FIELD(struct stat, st_nlink, "st_nlink", 0),
   MAKE_CSTRUCT_UINT_FIELD(struct stat, st_uid, "st_uid", 0),
   MAKE_CSTRUCT_UINT_FIELD(struct stat, st_gid, "st_gid", 0),
   MAKE_CSTRUCT_INT_FIELD(struct stat, st_rdev, "st_rdev", 0),
   MAKE_CSTRUCT_UINT_FIELD(struct stat, st_size, "st_size", 0),
   MAKE_CSTRUCT_UINT_FIELD(struct stat, st_atime, "st_atime", 0),
   MAKE_CSTRUCT_UINT_FIELD(struct stat, st_mtime, "st_mtime", 0),
   MAKE_CSTRUCT_UINT_FIELD(struct stat, st_ctime, "st_ctime", 0),
   SLANG_END_CSTRUCT_TABLE
};

static void fstat_intrin (void)
{
   struct stat st;
   int status;
   int fd;
   SLang_MMT_Type *mmt = NULL;
   SLFile_FD_Type *f = NULL;
   
   switch (SLang_peek_at_stack ())
     {
      case SLANG_FILE_FD_TYPE:
	if (-1 == SLfile_pop_fd (&f))
          return;
	if (-1 == SLfile_get_fd (f, &fd))
          {
          SLfile_free_fd (f);
          return;
          }
        break;

      case SLANG_FILE_PTR_TYPE:
        {
        FILE *fp;
        if (-1 == SLang_pop_fileptr (&mmt, &fp))
          return;
        fd = fileno (fp);
        }
        break;
       
      case SLANG_INT_TYPE:
	if (-1 == SLang_pop_int (&fd))
          {
          (void) SLerrno_set_errno (SL_TYPE_MISMATCH);
	  return;
          }
        break;

      default:
        SLdo_pop_n (SLang_Num_Function_Args);
        (void) SLerrno_set_errno (SL_TYPE_MISMATCH);
        (void) SLang_push_null ();
        return;
     }

   status = fstat (fd, &st);

   if (status == 0)
     SLang_push_cstruct ((VOID_STAR) &st, Fstat_Struct);
   else
     {
     (void) SLerrno_set_errno (errno);
     (void) SLang_push_null ();
     }

   if (f != NULL) SLfile_free_fd (f);
   if (mmt != NULL) SLang_free_mmt (mmt);
}

static SLang_Intrin_Fun_Type Ayios_Intrinsics [] =
{
   MAKE_INTRINSIC_SS("auth", auth_intrin, SLANG_INT_TYPE),
   MAKE_INTRINSIC_SI("initgroups", initgroups_intrin, SLANG_INT_TYPE),
   MAKE_INTRINSIC_S("realpath", realpath_intrin, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_S("fileexists", fileexists_intrin, SLANG_INT_TYPE),
   MAKE_INTRINSIC_0("istype", istype_intrin, SLANG_INT_TYPE),
   MAKE_INTRINSIC_0("fstat", fstat_intrin, SLANG_VOID_TYPE),
   MAKE_INTRINSIC_S("_isdirectory", _isdirectory_intrin, SLANG_INT_TYPE),
   MAKE_INTRINSIC_S("__isdirectory", __isdirectory_intrin, SLANG_INT_TYPE),
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
