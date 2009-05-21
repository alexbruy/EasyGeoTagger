/* LIBGPSBABEL config.h. This file was created manually using the files generated from config.h.in by configure under windows and linux respectively  */
#ifdef WIN32

/* 0 for most-used character sets */
#undef CET_WANTED

/* 1 to enable the CSV formats support */
#undef CSVFMTS_ENABLED

/* 1 to enable all the filters. */
#undef FILTERS_ENABLED

/* Defined if you have libexpat */
//#undef HAVE_LIBEXPAT

/* Define to 1 if you have the `m' library (-lm). */
#undef HAVE_LIBM

/* Defined if you have libusb */
#undef HAVE_LIBUSB

/* Define to 1 if you have the `z' library (-lz). */
#undef HAVE_LIBZ

/* Define to 1 if you have the `nanosleep' function. */
#undef HAVE_NANOSLEEP

/* Define to 1 if you have the `sleep' function. */
#undef HAVE_SLEEP

/* Define if we have va_copy */
#undef HAVE_VA_COPY/**/

/* Define if we have __va_copy */
#undef HAVE___VA_COPY/**/

/* Define as 1 if your va_list type is an array */
#undef HAVE_VA_LIST_AS_ARRAY

/* 1 to enable as many formats as possible */
#undef MAXIMAL_ENABLED

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the release name of this package. */
#undef PACKAGE_RELEASE

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* 1 to enable Palm PDB support */
#undef PDBFMTS_ENABLED

/* 1 to enable shapefile support */
#undef SHAPELIB_ENABLED

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#undef WORDS_BIGENDIAN

/* 1 to inhibit our use of zlib. */
#undef ZLIB_INHIBITED

#else

/* 0 for most-used character sets */
#define CET_WANTED 0

/* 1 to enable the CSV formats support */
#define CSVFMTS_ENABLED 1

/* 1 to enable all the filters. */
#define FILTERS_ENABLED 1

/* Defined if you have libexpat */
#define HAVE_LIBEXPAT 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Defined if you have libusb */
/*Pushed to makefile #define HAVE_LIBUSB 1*/ 

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the `nanosleep' function. */
#define HAVE_NANOSLEEP 1

/* Define to 1 if you have the `sleep' function. */
#define HAVE_SLEEP 1

/* 1 to enable as many formats as possible */
#define MAXIMAL_ENABLED 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "BUG-REPORT-ADDRESS"

/* Define to the full name of this package. */
#define PACKAGE_NAME "GPSBabel"

/* Define to the release name of this package. */
#define PACKAGE_RELEASE "-beta20080217"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "GPSBabel 1.3.5"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "gpsbabel"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.3.5"

/* 1 to enable Palm PDB support */
#define PDBFMTS_ENABLED 1

/* 1 to enable shapefile support */
#define SHAPELIB_ENABLED 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* 1 to inhibit our use of zlib. */
/* #undef ZLIB_INHIBITED */

#endif

