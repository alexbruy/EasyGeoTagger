SET(CMAKE_COLOR_MAKEFILE ON)

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


IF (WIN32)
  # expect that classes are being imported by default
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  # import qgis classes
  IF (MSVC)
    ADD_DEFINITIONS("-DGUI_EXPORT=__declspec(dllimport)")
    ADD_DEFINITIONS("-DCORE_EXPORT=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("\"-DGUI_EXPORT=__declspec(dllimport)\"")
    ADD_DEFINITIONS("\"-DCORE_EXPORT=__declspec(dllimport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DGUI_EXPORT=)
  ADD_DEFINITIONS(-DCORE_EXPORT=)
ENDIF (WIN32)


# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_find_rules ${CMAKE_MODULE_PATH})

FIND_PACKAGE(QGIS REQUIRED)
IF (NOT QGIS_FOUND)
  MESSAGE (SEND_ERROR "QGIS dependency was not found!")
ENDIF (NOT QGIS_FOUND)

FIND_PACKAGE(GDAL REQUIRED)
IF (NOT GDAL_FOUND)
  MESSAGE (SEND_ERROR "GDAL dependency was not found!")
ENDIF (NOT GDAL_FOUND)

FIND_PACKAGE(GEOS REQUIRED)
IF (NOT GEOS_FOUND)
  MESSAGE (SEND_ERROR "GEOS dependency was not found!")
ENDIF (NOT GEOS_FOUND)

FIND_PACKAGE(EXIV2 REQUIRED)
IF (NOT EXIV2_FOUND)
  MESSAGE (SEND_ERROR "EXIV2 dependency was not found!")
ENDIF (NOT EXIV2_FOUND)

FIND_PACKAGE(EASYGT REQUIRED)
IF (NOT EASYGT_FOUND)
  MESSAGE (SEND_ERROR "EASYGT dependency was not found!")
ENDIF (NOT EASYGT_FOUND)

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  ADD_DEFINITIONS(-DQGISDEBUG=1)
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)
########################################################
# Files

SET (exifprovider_SRCS
     qgsexifprovider.cpp
)

SET (exifprovider_MOC_HDRS
     qgsexifprovider.h
)

SET (QT_USE_QT3SUPPORT FALSE)
SET (QT_USE_QTXML TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )
########################################################
# Build

QT4_WRAP_CPP (exifprovider_MOC_SRCS  ${exifprovider_MOC_HDRS})

QT4_ADD_RESOURCES(exifprovider_RCC_SRCS ${exifprovider_RCCS})

ADD_LIBRARY (exifprovider MODULE ${exifprovider_SRCS} ${exifprovider_MOC_SRCS} ${exifprovider_RCC_SRCS})

INCLUDE_DIRECTORIES(
     ${EASYGT_INCLUDE_DIR}
     ${GDAL_INCLUDE_DIR}
     ${GEOS_INCLUDE_DIR}
     ${CMAKE_CURRENT_BINARY_DIR}
     ${QT_INCLUDE_DIR}
     ${QGIS_INCLUDE_DIR}
     .
)

TARGET_LINK_LIBRARIES(exifprovider
  ${EASYGT_LIBRARY}
  ${QT_LIBRARIES}
  ${QGIS_CORE_LIBRARY} 
  ${QGIS_GUI_LIBRARY}
)


########################################################
# Install

INSTALL(TARGETS exifprovider
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})
