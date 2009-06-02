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

SET( KML_SRCS
    egtkmlprovider.cpp
   )

SET( KML_MOC_HDRS
    egtkmlprovider.h
   )

SET( KML_UIS
    configurationdialog.ui
   )

QT4_WRAP_UI( KML_UIS_H ${KML_UIS} )
QT4_AUTOMOC( ${KML_SRCS} )
QT4_WRAP_CPP( KML_MOC_SRCS ${KML_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( kmlprovider MODULE ${KML_SRCS} ${KML_MOC_SRCS} ${KML_UIS_H})
TARGET_LINK_LIBRARIES( kmlprovider 
                       ${QT_LIBRARIES}
                       easygt
                     )

INSTALL( TARGETS kmlprovider DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
