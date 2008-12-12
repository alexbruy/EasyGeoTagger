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

SET( GPS_EXIF_EDITOR_SRCS
    egtexifeditor.cpp
    egtexiftagcontrol.cpp
    egtexiftaggroup.cpp
    egtgpsexifeditor.cpp
   )

SET( GPS_EXIF_EDITOR_MOC_HDRS
    egtexifeditor.h
    egtexiftagcontrol.h
    egtexiftaggroup.h
    egtgpsexifeditor.h
   )

SET( GPS_EXIF_EDITOR_UIS
   )
   
SET ( GPS_EXIF_EDITOR_RCCS
     egtgpsexifeditor.qrc
    )

QT4_WRAP_UI( GPS_EXIF_EDITOR_UIS_H ${GPS_EXIF_EDITOR_UIS} )

QT4_ADD_RESOURCES( GPS_EXIF_EDITOR_RCCS_SRCS ${GPS_EXIF_EDITOR_RCCS} )

QT4_AUTOMOC( ${GPS_EXIF_EDITOR_SRCS} )

QT4_WRAP_CPP( GPS_EXIF_EDITOR_MOC_SRCS ${GPS_EXIF_EDITOR_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( exifeditor MODULE
             ${GPS_EXIF_EDITOR_SRCS}
             ${GPS_EXIF_EDITOR_MOC_SRCS}
             ${GPS_EXIF_EDITOR_RCCS_SRCS}
             ${GPS_EXIF_EDITOR_UIS_H}
           )

TARGET_LINK_LIBRARIES( exifeditor
  ${QT_LIBRARIES}
  ${EXIV2_LIBRARY}
  easygt
  )

INSTALL( TARGETS exifeditor DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
