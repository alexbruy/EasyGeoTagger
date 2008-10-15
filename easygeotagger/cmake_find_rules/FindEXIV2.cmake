# Contributed by Tim Sutton 
# CMake module to search for EXIV2 library
#
# If it's found it sets EXIV2_FOUND to TRUE
# and following variables are set:
#    EXIV2_INCLUDE_DIR
#    EXIV2_LIBRARY


FIND_PATH(EXIV2_INCLUDE_DIR exiv2/exif.hpp
  /usr/local/include/
  /usr/include/
  #MSVC
  "$ENV{LIB_DIR}/include/exiv2"
  #mingw
  c:/msys/local/include/exiv2
  )

FIND_LIBRARY(EXIV2_LIBRARY NAMES exiv2 PATHS 
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (EXIV2_INCLUDE_DIR AND EXIV2_LIBRARY)
   SET(EXIV2_FOUND TRUE)
ENDIF (EXIV2_INCLUDE_DIR AND EXIV2_LIBRARY)


IF (EXIV2_FOUND)

   IF (NOT EXIV2_FIND_QUIETLY)
      MESSAGE(STATUS "Found EXIV2: ${EXIV2_LIBRARY}")
      MESSAGE(STATUS "Found EXIV2: ${EXIV2_INCLUDE_DIR}")
   ENDIF (NOT EXIV2_FIND_QUIETLY)

ELSE (EXIV2_FOUND)

   IF (EXIV2_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find EXIV2")
   ENDIF (EXIV2_FIND_REQUIRED)

ENDIF (EXIV2_FOUND)
