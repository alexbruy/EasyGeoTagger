PROJECT(EasyGT)

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
  cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

IF (WIN32)
  # expect that classes are being imported
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllimport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DMS_DLL_SPEC=)
ENDIF (WIN32)

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_find_rules ${CMAKE_MODULE_PATH})

SET (PYTHON_BINDINGS FALSE CACHE BOOL "Flag to indicate if pyghon bindings should be built")

SET (QT_USE_QT3SUPPORT FALSE)
SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTNETWORK TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTSVG TRUE)
SET (QT_USE_QTXML TRUE)
FIND_PACKAGE(Qt4 REQUIRED)

FIND_PACKAGE(EXIV2 REQUIRED)
IF (NOT EXIV2_FOUND)
  MESSAGE (SEND_ERROR "EXIV2 dependency was not found!")
ENDIF (NOT EXIV2_FOUND)

FIND_PACKAGE(LIBRAW REQUIRED)
IF (NOT LIBRAW_FOUND)
  MESSAGE (SEND_ERROR "LIBRAW dependency was not found!")
ENDIF (NOT LIBRAW_FOUND)

IF (PYTHON_BINDINGS)
  FIND_PACKAGE( PYTHON REQUIRED )
  IF (NOT PYTHON_FOUND)
    MESSAGE (SEND_ERROR "PYTHON dependency was not found!")
  ENDIF (NOT PYTHON_FOUND)
ENDIF (PYTHON_BINDINGS)

#MinGW seems to build the exe in Release mode but plugins in Debug mode as default
ADD_DEFINITIONS(-DQT_NO_DEBUG)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DEGTDEBUG=1)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

# set up the install locations
IF ( WIN32 )
  #Make this a find routine -- just for the lib
  FIND_LIBRARY( EXPAT_LIBRARY NAMES expat PATHS C:/devel/bin/msys/1.0/home/pete/local/bin/ )
  FIND_LIBRARY( ZLIB_LIBRARY NAMES z PATHS C:/devel/bin/msys/1.0/home/pete/local/lib/ )

  SET ( CMAKE_INSTALL_PREFIX   ./install )
  SET ( EASYGT_BIN_SUBDIR     . )
  SET ( EASYGT_LIB_SUBDIR     . )
  SET ( EASYGT_PLUGIN_SUBDIR  ./easygt )
  SET ( EASYGT_INCLUDE_SUBDIR ./include/easygt )
  SET ( PYTHON_PACKAGE_SUBDIR . )
  
  ADD_DEFINITIONS( -DPLUGINPATH=\\\"${EASYGT_PLUGIN_SUBDIR}\\\" )
ELSE ( WIN32 )
  SET( CMAKE_INSTALL_PREFIX   /usr/local )
  SET ( EASYGT_BIN_SUBDIR     bin )
  SET ( EASYGT_LIB_SUBDIR     lib )
  SET ( EASYGT_PLUGIN_SUBDIR  lib/easygt )
  SET ( EASYGT_INCLUDE_SUBDIR include/easygt )
  SET ( PYTHON_PACKAGE_SUBDIR lib/python2.5/site-packages )
  
  ADD_DEFINITIONS( -DPLUGINPATH=\\\"${CMAKE_INSTALL_PREFIX}/${EASYGT_PLUGIN_SUBDIR}\\\" )
ENDIF ( WIN32 )

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
  ${EXIV2_INCLUDE_DIR}
  ${LIBRAW_INCLUDE_DIR}
  ${PYTHON_INCLUDE_DIR}
  ${EasyGT_SOURCE_DIR}/src/app
  ${EasyGT_SOURCE_DIR}/src/core
  ${CMAKE_BINARY_DIR}/src/ui
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SUBDIRS( src )

IF (PYTHON_BINDINGS)
  SUBDIRS( python )
ENDIF (PYTHON_BINDINGS)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH ".:lib:./lib:./lib/plugins")
