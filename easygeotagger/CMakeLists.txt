PROJECT(EasyGT)
# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_find_rules ${CMAKE_MODULE_PATH})

FIND_PACKAGE(Qt4 REQUIRED)

FIND_PACKAGE(EXIV2 REQUIRED)
IF (NOT EXIV2_FOUND)
  MESSAGE (SEND_ERROR "EXIV2 dependency was not found!")
ENDIF (NOT EXIV2_FOUND)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DEGTDEBUG=1)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
  ${EXIV2_INCLUDE_DIR}
  ${EasyGT_SOURCE_DIR}/src/core
  ${EasyGT_SOURCE_DIR}/src/exif
  ${EasyGT_SOURCE_DIR}/src/gui
  ${CMAKE_BINARY_DIR}/src/ui
  ${CMAKE_BINARY_DIR}
  .
)

SUBDIRS( src )

SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH ".:lib:./lib")

SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
