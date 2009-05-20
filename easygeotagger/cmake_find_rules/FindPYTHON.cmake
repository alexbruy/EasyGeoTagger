# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for PYTHON library
#
# If it's found it sets PYTHON_FOUND to TRUE
# and following variables are set:
#    PYTHON_INCLUDE_DIR
#    PYTHON_LIBRARY

FIND_PROGRAM (PYTHON_BINARY python PATHS
  /usr/bin
  /usr/local/bin
  c:/python2.6
  c:/python2.5
  )

FIND_PATH(PYTHON_INCLUDE_DIR Python.h
  /usr/include/python2.6/
  /usr/include/python2.5/
  #MSVC
  "c:/OSGeo4W/include"
  "c:/Python26/include"
  "c:/Python25/include"
  "$ENV{LIB_DIR}/include/PYTHON"
  #mingw
  c:/msys/local/include/PYTHON
  )

SET( PYTHON_PACKAGES python2.6/dist-packages )
FIND_LIBRARY(PYTHON_LIBRARY NAMES python2.6 PATHS
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "c:/OSGeo4W/lib"
  "c:/Python26/lib"
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )

IF (NOT PYTHON_LIBRARY)
SET( PYTHON_PACKAGES python2.5/site-packages )
FIND_LIBRARY(PYTHON_LIBRARY NAMES python2.5 PATHS
  /usr/local/lib
  /usr/lib
  #MSVC
  "c:/OSGeo4W/lib"
  "c:/Python25/lib"
  "$ENV{LIB_DIR}/lib"
  #mingw
  c:/msys/local/lib
  )
ENDIF (NOT PYTHON_LIBRARY)


IF (PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY AND PYTHON_BINARY)
   SET(PYTHON_FOUND TRUE)
ENDIF (PYTHON_INCLUDE_DIR AND PYTHON_LIBRARY AND PYTHON_BINARY)


IF (PYTHON_FOUND)

   IF (NOT PYTHON_FIND_QUIETLY)
      MESSAGE(STATUS "Found PYTHON: ${PYTHON_BINARY}")
      MESSAGE(STATUS "Found PYTHON: ${PYTHON_LIBRARY}")
      MESSAGE(STATUS "Found PYTHON: ${PYTHON_INCLUDE_DIR}")
   ENDIF (NOT PYTHON_FIND_QUIETLY)

ELSE (PYTHON_FOUND)

   IF (PYTHON_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find PYTHON")
   ENDIF (PYTHON_FIND_REQUIRED)

ENDIF (PYTHON_FOUND)
