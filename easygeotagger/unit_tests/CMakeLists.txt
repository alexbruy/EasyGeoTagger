cmake_minimum_required(VERSION 2.6)

PROJECT(UnitTestExif)

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

FIND_PACKAGE(Qt4 REQUIRED)

FIND_PACKAGE(EASYGT REQUIRED)
IF (NOT EASYGT_FOUND)
  MESSAGE (SEND_ERROR "EASYGT dependency was not found!")
ENDIF (NOT EASYGT_FOUND)


INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
  ${UnitTestExif_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${EASYGT_INCLUDE_DIR}
  .
)

SET(UnitTestExif_SRCS
    main.cpp
    unittestgui.cpp
    egtgpsexifengine_unittest.cpp
    egtphotoexifengine_unittest.cpp
   )

SET(UnitTestExif_MOC_HDRS
    unittestgui.h
    egtgpsexifengine_unittest.h
    egtphotoexifengine_unittest.h
   )
   
SET(UnitTestExif_UIS
    unittestguibase.ui
   )



QT4_AUTOMOC(${UnitTestExif_SRCS})
QT4_WRAP_UI(UnitTestExif_UIS_H ${UnitTestExif_UIS})
QT4_WRAP_CPP(UnitTestExif_MOC_SRCS ${UnitTestExif_MOC_HDRS})

IF(WIN32)
  ADD_EXECUTABLE(UnitTestExif WIN32 ${UnitTestExif_SRCS} ${UnitTestExif_UIS_H} ${UnitTestExif_MOC_SRCS})
ELSE(WIN32)
  ADD_EXECUTABLE(UnitTestExif ${UnitTestExif_SRCS} ${UnitTestExif_UIS_H} ${UnitTestExif_MOC_SRCS})
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(UnitTestExif ${QT_LIBRARIES} ${EASYGT_LIBRARY})

SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
INSTALL(TARGETS UnitTestExif DESTINATION ${CMAKE_INSTALL_PREFIX})
