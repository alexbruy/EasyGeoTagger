# Contributed by Tim Sutton 
# CMake module to search for EXIV2 library
#
# If it's found it sets JPEG_FOUND to TRUE
# and following variables are set:
#    JPEG_INCLUDE_DIR
#    JPEG_LIBRARY


FIND_PATH(JPEG_INCLUDE_DIR jpeglib.h
  /usr/include
  #MSVC
  "$ENV{LIB_DIR}/include"
  #mingw
  c:/msys/local/include
  )

FIND_LIBRARY(JPEG_LIBRARY NAMES jpeg PATHS 
  /usr/lib 
  /usr/local/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (JPEG_INCLUDE_DIR AND JPEG_LIBRARY)
   SET(JPEG_FOUND TRUE)
ENDIF (JPEG_INCLUDE_DIR AND JPEG_LIBRARY)

IF (JPEG_FOUND)

   IF (NOT JPEG_FIND_QUIETLY)
      MESSAGE(STATUS "Found JPEG: ${JPEG_LIBRARY}")
      MESSAGE(STATUS "Found JPEG: ${JPEG_INCLUDE_DIR}")
   ENDIF (NOT JPEG_FIND_QUIETLY)

ELSE (JPEG_FOUND)

   IF (JPEG_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find lib JPEG")
   ENDIF (JPEG_FIND_REQUIRED)

ENDIF (JPEG_FOUND)
