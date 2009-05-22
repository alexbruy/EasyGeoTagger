IF (WIN32)
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllimport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_BINARY_DIR}
)

SET( GPX_SRCS
    egtgpxprovider.cpp
   )

SET( GPX_MOC_HDRS
    egtgpxprovider.h
   )

SET( GPX_UIS
   )

QT4_WRAP_UI( GPX_TEXT_UIS_H ${GPX_TEXT_UIS} )
QT4_AUTOMOC( ${GPX_SRCS} )
QT4_WRAP_CPP( GPX_MOC_SRCS ${GPX_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( gpxprovider MODULE ${GPX_SRCS} ${GPX_MOC_SRCS} ${GPX_UIS_H})
TARGET_LINK_LIBRARIES( gpxprovider 
                       ${QT_LIBRARIES}
                       easygt
                     )

INSTALL( TARGETS gpxprovider DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
