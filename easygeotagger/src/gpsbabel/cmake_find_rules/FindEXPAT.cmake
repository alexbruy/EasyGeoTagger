# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for EXPAT library
#
# If it's found it sets EXPAT_FOUND to TRUE
# and following variables are set:
#    EXPAT_INCLUDE_DIR
#    EXPAT_LIBRARY


FIND_PATH(EXPAT_INCLUDE_DIR expat.h
  /usr/local/include/
  /usr/include/
  #MSVC
  "$ENV{LIB_DIR}/include"
  "c:/libs_babel/expat/Source/lib"
  #mingw
  c:/msys/local/include
  )

FIND_LIBRARY(EXPAT_LIBRARY NAMES expat PATHS 
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  "C:/libs_babel/expat/Bin"
  #mingw
  c:/msys/local/lib
  )

IF (EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)
   SET(EXPAT_FOUND TRUE)
ENDIF (EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)


IF (EXPAT_FOUND)

   IF (NOT EXPAT_FIND_QUIETLY)
      MESSAGE(STATUS "Found EXPAT: ${EXPAT_LIBRARY}")
      MESSAGE(STATUS "Found EXPAT: ${EXPAT_INCLUDE_DIR}")
   ENDIF (NOT EXPAT_FIND_QUIETLY)

ELSE (EXPAT_FOUND)

   IF (EXPAT_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find EXPAT")
   ENDIF (EXPAT_FIND_REQUIRED)

ENDIF (EXPAT_FOUND)
