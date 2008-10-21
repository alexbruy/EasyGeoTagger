SET(CMAKE_COLOR_MAKEFILE ON)
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

FIND_PACKAGE(LIBRAW REQUIRED)
IF (NOT LIBRAW_FOUND)
  MESSAGE (SEND_ERROR "LIBRAW dependency was not found!")
ENDIF (NOT LIBRAW_FOUND)

FIND_PATH( EASYGT_INCLUDE_DIR main.cpp /home/pete/devel/easygeotagger/trunk/easygeotagger/src )
FIND_PATH( EASYGT_HACK_INCLUDE_DIR ui_egtmainwindowgui.h /home/pete/devel/easygeotagger/trunk/easygeotagger/build/src/ui )
FIND_LIBRARY( EASYGT_APP_LIBRARY egt_app /home/pete/devel/easygeotagger/trunk/easygeotagger/build/install/lib )
FIND_LIBRARY( EASYGT_CORE_LIBRARY egt_core /home/pete/devel/easygeotagger/trunk/easygeotagger/build/install/lib )
FIND_LIBRARY( EASYGT_GUI_LIBRARY egt_gui /home/pete/devel/easygeotagger/trunk/easygeotagger/build/install/lib )

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

#IF (CMAKE_BUILD_TYPE MATCHES Debug)
#  ADD_DEFINITIONS(-DEVISDEBUG=1)
#ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

########################################################
# Files

SET (easygeotagger_SRCS
     easygeotagger.cpp
     easygeotaggeridtool.cpp
     easygeotaggergui.cpp
)

SET (easygeotagger_UIS easygeotaggerguibase.ui)

SET (easygeotagger_MOC_HDRS
     easygeotagger.h
     easygeotaggeridtool.h
     easygeotaggergui.h
)

SET (easygeotagger_RCCS  easygeotagger.qrc)

SET (QT_USE_QT3SUPPORT FALSE)
SET (QT_USE_QTGUI TRUE)
SET (QT_USE_QTSQL TRUE)
SET (QT_USE_QTSVG TRUE)
SET (QT_USE_QTXML TRUE)
SET (QT_USE_QTNETWORK TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )
########################################################
# Build

QT4_WRAP_UI (easygeotagger_UIS_H  ${easygeotagger_UIS})

QT4_WRAP_CPP (easygeotagger_MOC_SRCS  ${easygeotagger_MOC_HDRS})

QT4_ADD_RESOURCES(easygeotagger_RCC_SRCS ${easygeotagger_RCCS})

ADD_LIBRARY (easygeotaggerplugin MODULE ${easygeotagger_SRCS} ${easygeotagger_MOC_SRCS} ${easygeotagger_RCC_SRCS} ${easygeotagger_UIS_H})

INCLUDE_DIRECTORIES(
     ${CMAKE_CURRENT_BINARY_DIR}
     ${QT_INCLUDE_DIR}
     ${GDAL_INCLUDE_DIR}
     ${GEOS_INCLUDE_DIR}
     ${QGIS_INCLUDE_DIR}
     ${EASYGT_INCLUDE_DIR}/app
     ${EASYGT_INCLUDE_DIR}/core
     ${EASYGT_INCLUDE_DIR}/gui
     ${EASYGT_HACK_INCLUDE_DIR}
     .
)

TARGET_LINK_LIBRARIES(easygeotaggerplugin
  ${QT_LIBRARIES}
  ${QGIS_CORE_LIBRARY} 
  ${QGIS_GUI_LIBRARY}
  ${GEOS_LIBRARY}
  ${GDAL_LIBRARY}
  ${EXIV2_LIBRARY}
  ${LIBRAW_LIBRARY}
  ${EASYGT_APP_LIBRARY}
  ${EASYGT_CORE_LIBRARY}
  ${EASYGT_GUI_LIBRARY}
)


########################################################
# Install

INSTALL(TARGETS easygeotaggerplugin
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

