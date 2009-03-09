# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for EASYGT library
#
# If it's found it sets EASYGT_FOUND to TRUE
# and following variables are set:
#    EASYGT_INCLUDE_DIR
#    EASYGT_LIBRARY

IF(WIN32)
  IF(MSVC)
  ELSE(MSVC)
  ENDIF(MSVC)
ELSE(WIN32)
  IF(APPLE)
  ELSE(APPLE)
  ENDIF(APPLE)
ENDIF(WIN32)

IF (WIN32)
  IF (MSVC)
    FIND_LIBRARY(EASYGT_LIBRARY NAMES easygt PATHS
      "c:/OSGeo4W/lib"
      "$ENV{LIB_DIR}/lib"
    )
    FIND_PATH(EASYGT_INCLUDE_DIR egtplugininterface.h
      "c:/OSGeo4W/include/easygt"
      "$ENV{LIB_DIR}/include/easygt"
    )
    #SET(EASYGT_PLUGIN_DIR "c:/OSGeo4W/apps/easygt/plugins")
  ELSE (MSVC)
    FIND_LIBRARY(EASYGT_LIBRARY NAMES easygt PATHS
      c:/msys/local/
    )
    FIND_PATH(EASYGT_INCLUDE_DIR egtplugininterface.h
      c:/msys/local/include/easygt
    )
    #SET(EASYGT_PLUGIN_DIR "./plugins")
  ENDIF (MSVC)
ELSE (WIN32)
  IF (APPLE)
    FIND_LIBRARY(EASYGT_LIBRARY NAMES easygt PATHS
      /Library/Frameworks.EASYGT.framework/lib
      /usr/lib
      /usr/local/lib
    )
    FIND_PATH(EASYGT_INCLUDE_DIR egtplugininterface.h
      /Library/Frameworks.EASYGT.framework/include
      /usr/include/easygt
      /usr/local/include/eastgt
    )
    FIND_PATH(EASYGT_PLUGIN_DIR libgpsexifeditor.so
      /Library/Frameworks.EASYGT.framework/lib/easygt
      /usr/lib/easygt
      /usr/local/lib/easygt
    )
  ELSE (APPLE)
    FIND_LIBRARY(EASYGT_LIBRARY NAMES easygt PATHS
      /usr/lib64
      /usr/lib
      /usr/local/lib
    )
    FIND_PATH(EASYGT_INCLUDE_DIR egtplugininterface.h
      /usr/include/easygt
      /usr/local/include/easygt
    )
    FIND_PATH(EASYGT_PLUGIN_DIR libgpsexifeditor.so
      /usr/lib64/easygt
      /usr/lib/easygt
      /usr/local/lib/easygt
    )
  ENDIF (APPLE)
ENDIF (WIN32)


IF (EASYGT_INCLUDE_DIR AND EASYGT_LIBRARY AND EASYGT_PLUGIN_DIR)
   SET(EASYGT_FOUND TRUE)
ENDIF (EASYGT_INCLUDE_DIR AND EASYGT_LIBRARY AND EASYGT_PLUGIN_DIR)

#ADD_DEFINITIONS( -DPLUGINPATH=\\\"${EASYGT_PLUGIN_DIR}\\\" )

IF (EASYGT_FOUND)

   IF (NOT EASYGT_FIND_QUIETLY)
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_LIBRARY}")
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_INCLUDE_DIR}")
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_PLUGIN_DIR}")
   ENDIF (NOT EASYGT_FIND_QUIETLY)

ELSE (EASYGT_FOUND)
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_LIBRARY}")
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_INCLUDE_DIR}")
      MESSAGE(STATUS "Found EASYGT: ${EASYGT_PLUGIN_DIR}")
   IF (EASYGT_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find EASYGT")
   ENDIF (EASYGT_FIND_REQUIRED)

ENDIF (EASYGT_FOUND)
