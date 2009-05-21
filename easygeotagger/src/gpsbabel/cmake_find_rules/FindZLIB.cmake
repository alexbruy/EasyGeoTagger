# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for ZLIB library
#
# If it's found it sets ZLIB_FOUND to TRUE
# and following variables are set:
#    ZLIB_INCLUDE_DIR
#    ZLIB_LIBRARY


FIND_PATH(ZLIB_INCLUDE_DIR zlib.h
  /usr/local/include/
  /usr/include/
  #MSVC
  "$ENV{LIB_DIR}/include"
  "C:/libs_babel/zlib/include"
  #mingw
  c:/msys/local/include
  )

FIND_LIBRARY(ZLIB_LIBRARY NAMES z PATHS #zdll in windows
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  "C:/libs_babel/zlib/lib"
  #mingw
  c:/msys/local/lib
  )

IF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
   SET(ZLIB_FOUND TRUE)
ENDIF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)


IF (ZLIB_FOUND)

   IF (NOT ZLIB_FIND_QUIETLY)
      MESSAGE(STATUS "Found ZLIB: ${ZLIB_LIBRARY}")
      MESSAGE(STATUS "Found ZLIB: ${ZLIB_INCLUDE_DIR}")
   ENDIF (NOT ZLIB_FIND_QUIETLY)

ELSE (ZLIB_FOUND)

   IF (ZLIB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find ZLIB")
   ENDIF (ZLIB_FIND_REQUIRED)

ENDIF (ZLIB_FOUND)
