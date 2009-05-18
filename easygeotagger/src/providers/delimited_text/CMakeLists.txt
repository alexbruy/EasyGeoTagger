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

SET( DELIMITED_TEXT_SRCS
    egtdelimitedtextconfigurationdialog.cpp
    egtdelimitedtextprovider.cpp
   )

SET( DELIMITED_TEXT_MOC_HDRS
    egtdelimitedtextconfigurationdialog.h
    egtdelimitedtextprovider.h
   )

SET( DELIMITED_TEXT_UIS
     egtdelimitedtextconfigurationdialog.ui
   )

QT4_WRAP_UI( DELIMITED_TEXT_UIS_H ${DELIMITED_TEXT_UIS} )
QT4_AUTOMOC( ${DELIMITED_TEXT_SRCS} )
QT4_WRAP_CPP( DELIMITED_TEXT_MOC_SRCS ${DELIMITED_TEXT_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( delimitedtextprovider MODULE ${DELIMITED_TEXT_SRCS} ${DELIMITED_TEXT_MOC_SRCS} ${DELIMITED_TEXT_UIS_H})
TARGET_LINK_LIBRARIES( delimitedtextprovider 
                       ${QT_LIBRARIES}
                       easygt
                     )

INSTALL( TARGETS delimitedtextprovider DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
