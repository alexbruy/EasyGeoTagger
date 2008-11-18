# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for PYTHON library
#
# If it's found it sets PYTHON_FOUND to TRUE
# and following variables are set:
#    PYTHON_INCLUDE_DIR
#    PYTHON_LIBRARY


FIND_PATH(PYTHON_INCLUDE_DIR Python.h
  /usr/local/include/python2.5/
  /usr/include/python2.5/
  #MSVC
  "$ENV{LIB_DIR}/include/PYTHON"
  #mingw
  c:/msys/local/include/PYTHON
  )

FIND_LIBRARY(PYTHON_LIBRARY NAMES python2.5 PATHS 
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY)
   SET(PYTHON_FOUND TRUE)
ENDIF (PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY)


IF (PYTHON_FOUND)

   IF (NOT PYTHON_FIND_QUIETLY)
      MESSAGE(STATUS "Found PYTHON: ${PYTHON_LIBRARY}")
      MESSAGE(STATUS "Found PYTHON: ${PYTHON_INCLUDE_DIR}")
   ENDIF (NOT PYTHON_FIND_QUIETLY)

ELSE (PYTHON_FOUND)

   IF (PYTHON_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find PYTHON")
   ENDIF (PYTHON_FIND_REQUIRED)

ENDIF (PYTHON_FOUND)
