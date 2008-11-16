# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for EASYGT library
#
# If it's found it sets EASYGT_FOUND to TRUE
# and following variables are set:
#    EASYGT_INCLUDE_DIR
#    EASYGT_LIBRARY


FIND_PATH(EASYGT_INCLUDE_DIR egtplugininterface.h
  /usr/local/include/easygt
  /usr/include/easygt
  #MSVC
  "$ENV{LIB_DIR}/include/easygt"
  #mingw
  c:/msys/local/include/easygt
  )

FIND_LIBRARY(EASYGT_LIBRARY NAMES easygt PATHS
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (EASYGT_INCLUDE_DIR AND EASYGT_LIBRARY)
   SET(EASYGT_FOUND TRUE)
ENDIF (EASYGT_INCLUDE_DIR AND EASYGT_LIBRARY)


IF (EASYGT_FOUND)

   IF (NOT EASYGT_FIND_QUIETLY)
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_LIBRARY}")
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_INCLUDE_DIR}")
   ENDIF (NOT EASYGT_FIND_QUIETLY)

ELSE (EASYGT_FOUND)

   IF (EASYGT_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find EASYGT")
   ENDIF (EASYGT_FIND_REQUIRED)

ENDIF (EASYGT_FOUND)
