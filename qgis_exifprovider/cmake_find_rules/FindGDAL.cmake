#MODIFIED from QGIS - PJE

## Once run this will define: 
## 
## GDAL_FOUND       = system has GDAL lib
##
## GDAL_LIBRARY     = full path to the library
##
## GDAL_INCLUDE_DIR      = where to find headers 
##
## Magnus Homann


IF(WIN32)

  IF (MINGW)
    FIND_PATH(GDAL_INCLUDE_DIR gdal.h /usr/local/include /usr/include c:/msys/local/include)
  ENDIF (MINGW)

  IF (MSVC)
    FIND_PATH(GDAL_INCLUDE_DIR gdal.h
       "c:/OSGeo4W/include/"
       "$ENV{LIB_DIR}/include/gdal"
       )

    # NOTE: under msvc you should add the following to your target link libraries
    # list as they are required by gdal
    #odbc32 odbccp32

  ENDIF (MSVC)
  
  
ELSE(WIN32)
  IF(UNIX) 

    # try to use framework on mac
    IF (APPLE)
      SET (GDAL_MAC_PATH /Library/Frameworks/GDAL.framework/unix/bin)
    ENDIF (APPLE)

    SET(GDAL_CONFIG_PREFER_PATH "$ENV{GDAL_HOME}/bin" CACHE STRING "preferred path to GDAL (gdal-config)")
    FIND_PROGRAM(GDAL_CONFIG gdal-config
      ${GDAL_CONFIG_PREFER_PATH}
      ${GDAL_MAC_PATH}
      /usr/local/bin/
      /usr/bin/
      )
    # MESSAGE("DBG GDAL_CONFIG ${GDAL_CONFIG}")
    
    IF (GDAL_CONFIG) 
      # set INCLUDE_DIR to prefix+include
      EXEC_PROGRAM(${GDAL_CONFIG}
        ARGS --prefix
        OUTPUT_VARIABLE GDAL_PREFIX)
      #SET(GDAL_INCLUDE_DIR ${GDAL_PREFIX}/include CACHE STRING INTERNAL)
      FIND_PATH(GDAL_INCLUDE_DIR 
        gdal.h 
        ${GDAL_PREFIX}/include/gdal
        ${GDAL_PREFIX}/include
        /usr/local/include 
        /usr/include 
        )

    ELSE(GDAL_CONFIG)
      MESSAGE("FindGDAL.cmake: gdal-config not found. Please set it manually. GDAL_CONFIG=${GDAL_CONFIG}")
    ENDIF(GDAL_CONFIG)

  ENDIF(UNIX)
ENDIF(WIN32)


IF (GDAL_INCLUDE_DIR)
   SET(GDAL_FOUND TRUE)
ENDIF (GDAL_INCLUDE_DIR)

IF (GDAL_FOUND)

   IF (NOT GDAL_FIND_QUIETLY)
      MESSAGE(STATUS "Found GDAL: ${GDAL_INCLUDE_DIR}")
   ENDIF (NOT GDAL_FIND_QUIETLY)

ELSE (GDAL_FOUND)

   MESSAGE(FATAL_ERROR "Could not find GDAL")

ENDIF (GDAL_FOUND)
