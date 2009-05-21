# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for LIBMATH library
#
# If it's found it sets LIBMATH_FOUND to TRUE
# and following variables are set:
#    LIBMATH_LIBRARY


FIND_LIBRARY(LIBMATH_LIBRARY NAMES m PATHS 
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (LIBMATH_LIBRARY)
   SET(LIBMATH_FOUND TRUE)
ENDIF (LIBMATH_LIBRARY)


IF (LIBMATH_FOUND)

   IF (NOT LIBMATH_FIND_QUIETLY)
      MESSAGE(STATUS "Found LIBMATH: ${LIBMATH_LIBRARY}")
   ENDIF (NOT LIBMATH_FIND_QUIETLY)

ELSE (LIBMATH_FOUND)

   IF (LIBMATH_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find LIBMATH")
   ENDIF (LIBMATH_FIND_REQUIRED)

ENDIF (LIBMATH_FOUND)
