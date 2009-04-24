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

SET( GPS_PLUGIN_SRCS
    egtgpsplugin.cpp
    egtgraphicaldelimitedtextfilereader.cpp
    egtreaderfactory.cpp
    egtgpsdisplaywidget.cpp
   )



SET( GPS_PLUGIN_MOC_HDRS
    egtgpsplugin.h
    egtreaderfactory.h
    egtgraphicaldelimitedtextfilereader.h
    egtgpsdisplaywidget.h
   )

SET( GPS_PLUGIN_UIS
    filetypedialog.ui
    textdelimiter.ui
    egtgpsdisplaywidgetbase.ui
    egtsynchronize.ui
   )

QT4_WRAP_UI( GPS_PLUGIN_UIS_H ${GPS_PLUGIN_UIS} )

QT4_AUTOMOC( ${GPS_PLUGIN_SRCS} )
QT4_WRAP_CPP( GPS_PLUGIN_MOC_SRCS ${GPS_PLUGIN_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( gpsplugin MODULE 
             ${GPS_PLUGIN_SRCS}
             ${GPS_PLUGIN_MOC_SRCS}
             ${GPS_PLUGIN_UIS_H} 
           )
TARGET_LINK_LIBRARIES( gpsplugin
                       ${QT_LIBRARIES}
                       easygt
                     )
		   
		   
INSTALL( TARGETS gpsplugin DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
