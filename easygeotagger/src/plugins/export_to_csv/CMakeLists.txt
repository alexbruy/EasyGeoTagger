IF (WIN32)
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllimport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}
  .
)

SET( EXPORT_TO_CSV_SRCS
    egtexporttocsv.cpp
   )

SET( EXPORT_TO_CSV_MOC_HDRS
    egtexporttocsv.h
   )

QT4_AUTOMOC( ${EXPORT_TO_CSV_SRCS} )
QT4_WRAP_CPP( EXPORT_TO_CSV_MOC_SRCS ${EXPORT_TO_CSV_MOC_HDRS} )

ADD_DEFINITIONS( -DQT_PLUGIN )

ADD_library( exporttocsv MODULE ${EXPORT_TO_CSV_SRCS} ${EXPORT_TO_CSV_MOC_SRCS} )
TARGET_LINK_LIBRARIES( exporttocsv 
  ${QT_LIBRARIES}
  ${EXIV2_LIBRARY}
  easygt
  )

INSTALL( TARGETS exporttocsv DESTINATION ${EASYGT_PLUGIN_SUBDIR} )
