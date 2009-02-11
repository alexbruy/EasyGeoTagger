IF (WIN32)
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllimport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}/src/plugins/google_plugin
)

SET( GOOGLE_SRCS
    egtgoogledisplaywidget.cpp
    egtgoogleplugin.cpp
   )

SET( GOOGLE_MOC_HDRS
    egtgoogledisplaywidget.h
    egtgoogleplugin.h
   )

SET( GOOGLE_UIS
    egtgoogledisplaywidgetbase.ui
   )

QT4_WRAP_UI( GOOGLE_UIS_H ${GOOGLE_UIS})

QT4_AUTOMOC( ${GOOGLE_SRCS} )
QT4_WRAP_CPP( GOOGLE_MOC_SRCS ${GOOGLE_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( googleplugin MODULE ${GOOGLE_SRCS} ${GOOGLE_MOC_SRCS} ${GOOGLE_UIS_H})
TARGET_LINK_LIBRARIES( googleplugin ${QT_LIBRARIES} easygt )

INSTALL( TARGETS googleplugin DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
