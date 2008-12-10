SUBDIRS( ui plugins )

IF (WIN32)
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllexport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

SET( EGT_INSTALL_HDRS
     app/egtapplication.h
     app/egtapplicationinterface.h
     app/egtplugininterface.h
     app/egtpythonconsolebase.h

     core/egtexifioengine.h
     core/egtimageengine.h
     core/egtlogger.h
     core/egtpathbuilder.h
     core/egtrawimagereader.h

   )

SET( EGT_CORE_SRCS
     core/egtexifioengine.cpp
     core/egtimageengine.cpp
     core/egtpathbuilder.cpp
     core/egtrawimagereader.cpp
   )

SET( EGT_CORE_MOC_HDRS
     core/egtimageengine.h
     core/egtrawimagereader.h
   )

SET( EGT_APP_SRCS
    app/egtapplication.cpp
    app/egtapplicationinterface.cpp
    app/egtitemdelegate.cpp
    app/egtmainwindow.cpp
    app/egtpluginmanager.cpp
    app/egtplugindisplaywidget.cpp

   )

SET( EGT_APP_MOC_HDRS
    app/egtapplication.h
    app/egtapplicationinterface.h
    app/egtitemdelegate.h
    app/egtmainwindow.h
    app/egtplugininterface.h
    app/egtpluginmanager.h
    app/egtplugindisplaywidget.h
   )

IF ( PYTHON_BINDINGS )
  SET( EGT_PYTHON_SRCS app/egtpythonconsolebase.cpp )
  SET( EGT_PYTHON_MOC_HDRS app/egtpythonconsolebase.h )
ENDIF ( PYTHON_BINDINGS )

SET (EGT_APP_RCCS app/app.qrc)

QT4_AUTOMOC( ${EGT_CORE_SRCS} ${EGT_APP_SRCS} ${EGT_PYTHON_SRCS} )

QT4_WRAP_CPP( EGT_APP_MOC_SRCS ${EGT_APP_MOC_HDRS} )
QT4_WRAP_CPP( EGT_CORE_MOC_SRCS ${EGT_CORE_MOC_HDRS} )
QT4_WRAP_CPP( EGT_PYTHON_MOC_SRCS ${EGT_PYTHON_MOC_HDRS} )

QT4_ADD_RESOURCES( EGT_APP_RCCS_SRCS ${EGT_APP_RCCS} )

ADD_LIBRARY ( easygt SHARED ${EGT_CORE_SRCS} ${EGT_CORE_MOC_SRCS} ${EGT_APP_SRCS} ${EGT_APP_MOC_SRCS} ${EGT_APP_RCCS_SRCS} ${EGT_PYTHON_SRCS} ${EGT_PYTHON_MOC_SRCS})

ADD_DEPENDENCIES( easygt  easygt_ui )

TARGET_LINK_LIBRARIES( easygt
  ${QT_LIBRARIES}
  ${EXIV2_LIBRARY}
  ${ZLIB_LIBRARY}
  ${EXPAT_LIBRARY}
  ${LIBRAW_LIBRARY}
  ${PYTHON_LIBRARY}
  )

SET( EasyGT_APP_SRCS
     main.cpp
   )

QT4_AUTOMOC( ${EasyGT_APP_SRCS} )

IF( WIN32 )
  IF ( CMAKE_BUILD_TYPE MATCHES Debug )
    ADD_EXECUTABLE( easygeotagger ${EasyGT_APP_SRCS} )
  ELSE ( CMAKE_BUILD_TYPE MATCHES Debug )
    ADD_EXECUTABLE( easygeotagger WIN32 ${EasyGT_APP_SRCS} )
  ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )
ELSE( WIN32 )
  ADD_EXECUTABLE( easygeotagger ${EasyGT_APP_SRCS} )
ENDIF( WIN32 )

SET_TARGET_PROPERTIES( easygeotagger PROPERTIES
  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${EASYGT_LIB_SUBDIR}
  INSTALL_RPATH_USE_LINK_PATH true
  )

TARGET_LINK_LIBRARIES( easygeotagger easygt ${QT_LIBRARIES} )

INSTALL( FILES ${EGT_INSTALL_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})
INSTALL( TARGETS easygeotagger DESTINATION ${EASYGT_BIN_SUBDIR} )
INSTALL( TARGETS easygt DESTINATION ${EASYGT_LIB_SUBDIR} )
