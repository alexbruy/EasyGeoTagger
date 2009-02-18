# MODIFIED from QGIS - PJE

# CMake module to search for GEOS library
#
# If it's found it sets GEOS_FOUND to TRUE
# and following variables are set:
#    GEOS_INCLUDE_DIR


FIND_PATH(GEOS_INCLUDE_DIR geos_c.h 
  #apple
  /Library/Frameworks/GOES.framework/include
  #*nix
  /usr/local/include 
  /usr/include 
  #MSVC
  "c:/OSGeo4W/include/"
  "$ENV{LIB_DIR}/include"
   #mingw
  c:/msys/local/include
  )

IF (GEOS_INCLUDE_DIR )
   SET(GEOS_FOUND TRUE)
ENDIF (GEOS_INCLUDE_DIR)


IF (GEOS_FOUND)

   IF (NOT GEOS_FIND_QUIETLY)
      MESSAGE(STATUS "Found GEOS: ${GEOS_INCLUDE_DIR}")
   ENDIF (NOT GEOS_FIND_QUIETLY)

ELSE (GEOS_FOUND)

   IF (GEOS_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find GEOS")
   ENDIF (GEOS_FIND_REQUIRED)

ENDIF (GEOS_FOUND)
