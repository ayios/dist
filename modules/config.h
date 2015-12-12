/* fork */
#define HAVE_SYS_WAIT_H 1

/* socket */
#define HAVE_SOCKET 1
#define HAVE_SOCKETPAIR 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_SYS_UN_H 1

/*  rand */
#define SIZEOF_INT 4

/*
#define SIZEOF_SHORT 2
#define SIZEOF_LONG 8
#define SIZEOF_FLOAT 4
#define SIZEOF_DOUBLE 8
#define SIZEOF_LONG_LONG 8

#define SIZEOF_OFF_T 8
#define SIZEOF_SIZE_T 8
#define HAVE_LONG_LONG 1

#define HAVE_STDLIB_H 1

#define HAVE_UNISTD_H 1

#define HAVE_TERMIOS_H 1

#define HAVE_MEMORY_H 1

#define HAVE_MALLOC_H 1

#define HAVE_MEMSET 1

#define HAVE_MEMCPY 1

#define HAVE_SETLOCALE 1
#define HAVE_NEWLOCALE 1
#define HAVE_LOCALECONV 1
#define HAVE_LOCALE_H 1
#define HAVE_XLOCALE_H 1
#define HAVE_LANGINFO_H 1
#define HAVE_NL_LANGINFO_CODESET 1

#define HAVE_FENV_H 1

#define HAVE_FECLEAREXCEPT 1
#define HAVE_VFSCANF 1
#define HAVE_STRTOD 1
#define HAVE_ATOLL 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOD_L 1

#define HAVE_FCNTL_H 1

#define HAVE_VSNPRINTF 1
#define HAVE_SNPRINTF 1

#define HAVE_SYS_FCNTL_H 1

#define HAVE_SYS_STATVFS_H 1

#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_TIMES_H 1
#define HAVE_SYS_TIME_H 1

#define HAVE_UTIME_H 1
#define HAVE_UTIME 1
#define HAVE_UTIMES 1

#define HAVE_GETITIMER 1
#define HAVE_SETITIMER 1
#define HAVE_SYS_MMAN_H 1

#define HAVE_SYS_RESOURCE_H 1
#define HAVE_GETRUSAGE 1
*/


/*
#define HAVE_ATEXIT 1
#define HAVE_ON_EXIT 1
#define HAVE_PUTENV 1
#define HAVE_GETCWD 1
#define HAVE_TCGETATTR 1
#define HAVE_TCSETATTR 1
#define HAVE_CFGETOSPEED 1
#define HAVE_LSTAT 1
#define HAVE_KILL 1
#define HAVE_KILLPG 1
#define HAVE_CHOWN 1
#define HAVE_LCHOWN 1
#define HAVE_VSNPRINTF 1
#define HAVE_POPEN 1
#define HAVE_UMASK 1
#define HAVE_READLINK 1
#define HAVE_SYMLINK 1
#define HAVE_LINK 1
#define HAVE_TIMES 1
#define HAVE_GMTIME 1
#define HAVE_GMTIME_R 1
#define HAVE_LOCALTIME_R 1
#define HAVE_CTIME_R 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_MKTIME 1
#define HAVE_MKFIFO 1
#define HAVE_MMAP 1
#define HAVE_TTYNAME_R 1
#define HAVE_TTYNAME 1
#define HAVE_STATVFS 1
#define HAVE_GETPPID 1
#define HAVE_GETGID 1
#define HAVE_GETEGID 1
#define HAVE_GETEUID 1
#define HAVE_GETUID 1

#define HAVE_SETGID 1
#define HAVE_SETUID 1

#define HAVE_GETPGID 1
#define HAVE_GETPGRP 1
#define HAVE_SETPGID 1
#define HAVE_SETPGRP 1
#define HAVE_GETSID 1
#define HAVE_SETSID 1

#define HAVE_GETPRIORITY 1
#define HAVE_SETPRIORITY 1
#define HAVE_ACOSH 1
#define HAVE_ASINH 1
#define HAVE_ATANH 1

#define HAVE_FINITE 1
#define HAVE_ISINF 1
#define HAVE_ISNAN 1
#define HAVE_HYPOT 1
#define HAVE_ATAN2 1
#define HAVE_FREXP 1
#define HAVE_FREXPF 1
#define HAVE_LDEXP 1
#define HAVE_LDEXPF 1
#define HAVE_SINCOS 1
#define HAVE_SINCOSF 1

#define HAVE_DIRENT_H 1

#define HAVE_FSEEKO 1
#define HAVE_SETVBUF 1

#define HAVE_DLFCN_H 1

#define HAVE_SYS_UTSNAME_H 1
#define HAVE_UNAME 1

#define HAVE_ALARM 1
#define HAVE_PAUSE 1
#define HAVE_CONFSTR 1
#define HAVE_SYSCONF 1
#define HAVE_PATHCONF 1

#define HAVE_ENVIRON 1

#define HAVE_PCRE_H 1

#define HAVE_SIGLONGJMP 1

#define HAVE_SIGACTION 1
#define HAVE_SIGPROCMASK 1
#define HAVE_SIGEMPTYSET 1
#define HAVE_SIGADDSET 1

#define HAVE_ICONV 1
#define ICONV_CONST
#define HAVE_FORK 1
#define HAVE_WAITPID 1
#if defined(HAVE_SIGADDSET) && defined(HAVE_SIGEMPTYSET)
# if defined(HAVE_SIGACTION) && defined(HAVE_SIGPROCMASK)
#  define SLANG_POSIX_SIGNALS
# endif
#endif

#ifdef _AIX
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE 1
# endif
# ifndef _ALL_SOURCE
#  define _ALL_SOURCE 1
# endif

#endif

#if defined(ultrix) && !defined(__GNUC__)
# ifndef NO_PROTOTYPES
#  define NO_PROTOTYPES
# endif
#endif

#ifndef unix
# define unix 1
#endif

#ifndef __unix__
# define __unix__ 1
#endif

#ifndef HAVE_LONG_LONG
# define SIZEOF_LONG_LONG 8
# define SIZEOF_LONG_LONG 8
#endif
*/
