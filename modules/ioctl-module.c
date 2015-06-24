/* Copyright (C) 2007 Laurent Perez <laurent.perez@unicaen.fr> */
/* Released under the terms of the GNU GPL (version 2 or later). */

#include <sys/ioctl.h>
#include <slang.h>
#include <errno.h>

SLANG_MODULE (ioctl);

/*
\function{ioctl}
\synopsis{(Integer_Type, BString_Type) ioctl (FD_Type fd, UInteger_Type request [, BString_Type arg])}
\description
Perform a ioctl system call.
In C ioctl () function, the third argument can be of various types
(void, int, struct). We can't wrap all possibly structures in a slang
module. This argument is therefore pass to the ioctl function as a
binary string specially crafted with slang pack function. 
Most ioctl requests use the third argument for output values. A few
ioctl requests use the return value as an output parameter. Slang ioctl
function return both.
\example
Eject CD-ROM tray :
#v+
fd = open ("/dev/cdrom", O_RDONLY | O_NONBLOCK);
() = ioctl (fd, 0x00005309);
close (fd);
#v-
\seealso{man ioctl, man ioctl_list}
\done
*/

void sl_ioctl (void)
{
   char *argp = NULL;
   int fd, retval;
   unsigned int length, request;
   SLFile_FD_Type *f;
   SLang_BString_Type *bstr;
   
   if (SLang_Num_Function_Args == 3)
     {
	if (SLang_pop_bstring (&bstr) == -1)
	  goto usage_error;	
	if (SLang_pop_uint ((unsigned int *) &request) == -1)
	  goto usage_error;
	if (SLfile_pop_fd (&f) == -1)
	  goto usage_error;
	argp = SLbstring_get_pointer (bstr, &length);
     }
   else if (SLang_Num_Function_Args == 2)
     {
	if (SLang_pop_uint ((unsigned int *) &request) == -1)
	  goto usage_error;
	if (SLfile_pop_fd (&f) == -1)
	  goto usage_error;
     }
   else
     goto usage_error;
   
   if (SLfile_get_fd (f, &fd) == -1)
     return;
      
   if ((retval = ioctl (fd, request, argp)) == -1)
     {
	SLang_set_error (SL_OS_Error);
	(void) SLerrno_set_errno (errno);
	return;
     }   
   
   (void) SLang_push_int (retval);
   if (argp != NULL)
     (void) SLang_push_bstring (bstr);
   return;
   
   usage_error :
   SLang_set_error (SL_Usage_Error);
   return;
}

static SLang_Intrin_Fun_Type SLioctl_Intrinsics [] =
{
   MAKE_INTRINSIC_0 ("ioctl", sl_ioctl, SLANG_VOID_TYPE),
   SLANG_END_INTRIN_FUN_TABLE
};

int init_ioctl_module_ns (char *ns_name)
{
   SLang_NameSpace_Type *ns;
   
   ns = SLns_create_namespace (ns_name);
   if (ns == NULL)
     return -1;
   
   if (-1 == SLns_add_intrin_fun_table (ns, SLioctl_Intrinsics, "__ioctl__"))
          return -1;
   
   return 0;
}
