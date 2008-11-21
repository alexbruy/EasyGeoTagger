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

SET( MINIMAL_EXIF_EDITOR_SRCS
    egtminimalexifeditor.cpp
    egtminimalexifeditorcontrols.cpp
   )

SET( MINIMAL_EXIF_EDITOR_MOC_HDRS
    egtminimalexifeditor.h
    egtminimalexifeditorcontrols.h
   )

SET( MINIMAL_EXIF_EDITOR_UIS
    egtminimalexifeditorcontrolsgui.ui
   )
   
SET ( MINIMAL_EXIF_EDITOR_RCCS 
     egtminimalexifeditor.qrc
    )

QT4_WRAP_UI( MINIMAL_EXIF_EDITOR_UIS_H ${MINIMAL_EXIF_EDITOR_UIS} )

QT4_ADD_RESOURCES( MINIMAL_EXIF_EDITOR_RCCS_SRCS ${MINIMAL_EXIF_EDITOR_RCCS} )

QT4_AUTOMOC( ${MINIMAL_EXIF_EDITOR_SRCS} )

QT4_WRAP_CPP( MINIMAL_EXIF_EDITOR_MOC_SRCS ${MINIMAL_EXIF_EDITOR_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( minimalexifeditor MODULE 
             ${MINIMAL_EXIF_EDITOR_SRCS}
             ${MINIMAL_EXIF_EDITOR_MOC_SRCS}
             ${MINIMAL_EXIF_EDITOR_RCCS_SRCS}
             ${MINIMAL_EXIF_EDITOR_UIS_H} 
           )

TARGET_LINK_LIBRARIES( minimalexifeditor
  ${QT_LIBRARIES}
  ${EXIV2_LIBRARY}
  easygt
  )

INSTALL( TARGETS minimalexifeditor DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
