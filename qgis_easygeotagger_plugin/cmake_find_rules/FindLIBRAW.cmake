# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for LIBRAW library
#
# If it's found it sets LIBRAW_FOUND to TRUE
# and following variables are set:
#    LIBRAW_INCLUDE_DIR
#    LIBRAW_LIBRARY


FIND_PATH(LIBRAW_INCLUDE_DIR libraw/libraw.h
  /usr/local/include/
  /usr/include/
  #MSVC
  "c:/OSGeo4W/include/"
  "$ENV{LIB_DIR}/include/libraw"
  #mingw
  c:/msys/local/include/libraw
  )

FIND_LIBRARY(LIBRAW_LIBRARY NAMES raw PATHS 
  /usr/lib64
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "c:/OSGeo4W/lib/"
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (LIBRAW_INCLUDE_DIR AND LIBRAW_LIBRARY)
   SET(LIBRAW_FOUND TRUE)
ENDIF (LIBRAW_INCLUDE_DIR AND LIBRAW_LIBRARY)


IF (LIBRAW_FOUND)

   IF (NOT LIBRAW_FIND_QUIETLY)
      MESSAGE(STATUS "Found LIBRAW: ${LIBRAW_LIBRARY}")
      MESSAGE(STATUS "Found LIBRAW: ${LIBRAW_INCLUDE_DIR}")
   ENDIF (NOT LIBRAW_FIND_QUIETLY)

ELSE (LIBRAW_FOUND)

   IF (LIBRAW_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find LIBRAW")
   ENDIF (LIBRAW_FIND_REQUIRED)

ENDIF (LIBRAW_FOUND)
