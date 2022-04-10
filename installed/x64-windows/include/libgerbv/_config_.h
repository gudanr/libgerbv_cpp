/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

#ifdef GUDANR
/* Relative path from bindir to datadir */
#define BINDIR_TO_DATADIR "../share"

/* Relative path from bindir to exec_prefix */
#define BINDIR_TO_EXECPREFIX ".."

/* Relative path from bindir to pkgdatadir */
#define BINDIR_TO_PKGDATADIR "../share/gerbv"
#endif

/* Define to 1 to enable debugging code */
#define DEBUG 1

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#ifndef GUDANR
#define ENABLE_NLS 0
#endif

/* Default border coefficient for export */
#define GERBV_DEFAULT_BORDER_COEFF 0.05

/* Default unit to display in statusbar */
#define GERBV_DEFAULT_UNIT GERBV_MMS

#ifdef GUDANR
/* Directory separator char */
#define GERBV_DIR_SEPARATOR_C '/'

/* Directory separator string */
#define GERBV_DIR_SEPARATOR_S "/"

/* Search path separator string */
#define GERBV_PATH_DELIMETER ""

/* Define to 1 if you have the `canonicalize_file_name' function. */
#define HAVE_CANONICALIZE_FILE_NAME 1

/* Define to 1 if you have the Mac OS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the Mac OS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define if you have the iconv() function and it works. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `dl' library (-ldl). */
#define HAVE_LIBDL 1

/* Define to 1 if you have the `dxflib' library (-ldxflib). */
/* #undef HAVE_LIBDXFLIB */

/* Define to 1 if you have the `efence' library (-lefence). */
/* #undef HAVE_LIBEFENCE */

/* Define to 1 if you have the <libgen.h> header file. */
#define HAVE_LIBGEN_H 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `realpath' function. */
#define HAVE_REALPATH 1

/* Define to 1 if you have the <regex.h> header file. */
#define HAVE_REGEX_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlwr' function. */
#define HAVE_STRLWR 1

/* Define to 1 if you have the <sys/mman.h> header file. */
#define HAVE_SYS_MMAN_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Define if canonicalize_file_name is not declared in system header files. */
/* #undef NEED_DECLARATION_CANONICALIZE_FILE_NAME */

/* Name of package */
#define PACKAGE "gerbv"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "gerbv"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "gerbv 2.8.3-dev~82d59a"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gerbv"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.8.3-dev~82d59a"

/* Scheme interpreter not used standalone */
#define STANDALONE 0

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* DL Sun method */
#define SUN_DL 1

/* ASCII names in Scheme interpreter */
#define USE_ASCII_NAMES 1

/* Colon Hooks in Scheme interpreter */
#define USE_COLON_HOOKS 1

/* Dynamic linking in Scheme interpreter */
#define USE_DL 1

/* Use extension interface of Scheme interpreter */
#define USE_INTERFACE 1

/* Math in Scheme interpreter */
#define USE_MATH 1

/* String Hooks in Scheme interpreter */
#define USE_STRING_HOOKS 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif

#else	// GUDANR

#define HAVE_STRLWR 1

#endif	// GUDANR


/* Version number of package */
#define VERSION "2.8.3-dev~82d59a"

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */
