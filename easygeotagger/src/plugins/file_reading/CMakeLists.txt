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

SET( FILE_READER_SRCS
    egtfilereader.cpp
    egtfilereadercontrols.cpp
   )

SET( FILE_READER_MOC_HDRS
    egtfilereader.h
    egtfilereadercontrols.h
   )

SET( FILE_READER_UIS
    egtfilereadercontrolsgui.ui
   )



QT4_WRAP_UI( FILE_READER_UIS_H ${FILE_READER_UIS} )

QT4_ADD_RESOURCES( FILE_READER_RCCS_SRCS ${FILE_READER_RCCS} )

QT4_AUTOMOC( ${FILE_READER_SRCS} )

QT4_WRAP_CPP( FILE_READER_MOC_SRCS ${FILE_READER_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( filereader MODULE 
             ${FILE_READER_SRCS}
             ${FILE_READER_MOC_SRCS}
             ${FILE_READER_RCCS_SRCS}
             ${FILE_READER_UIS_H} 
           )

TARGET_LINK_LIBRARIES( filereader
  ${QT_LIBRARIES}
  ${EXIV2_LIBRARY}
  easygt
  )

INSTALL( TARGETS filereader DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
