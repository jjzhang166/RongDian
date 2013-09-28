#pragma  once

typedef unsigned int UINT32, *PUINT32;

/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
#define HAVE_DOPRNT 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define if the gl_s library is installed (SGI GL library) */
#undef HAVE_LIBGL_S

/* Define if the X11 library is installed */
#undef HAVE_LIBX11

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP	1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the <varargs.h> header file. */
#define HAVE_VARARGS_H 1

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

#define HAVE_IO_H 1

/* Name of package */
#define PACKAGE "GifLib"

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#define PACKAGE_NAME "GifLib"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GifLib 4.1.4"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "GifLib 4.1.4"

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.1.4"

/* Define to 1 if the C compiler supports function prototypes. */
#undef PROTOTYPES

/* Define to 1 if the `setvbuf' function takes the buffering type as its
   second argument and the buffer pointer as the third, as on System V before
   release 3. */
#undef SETVBUF_REVERSED

/* The size of a `unsigned int', as computed by sizeof. */
#define SIZEOF_UNSIGNED_INT sizeof(unsigned int)

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG sizeof(unsigned long)

/* Define to 1 if you have the ANSI C header files. */
#undef STDC_HEADERS

/* Set to an unsigned 32 bit integer available on this system */
//#undef UINT32
#ifdef _WIN32
#define u_int32_t uint32_t
#endif

/* Version number of package */
#define VERSION "4.1.4"

/* Define to 1 if the X Window System is missing or not being used. */
#define DISPLAY_MISSING 1

/* Define like PROTOTYPES; this can be used by system headers. */
#undef __PROTOTYPES

/* Define to empty if `const' does not conform to ANSI C. */
#undef const
