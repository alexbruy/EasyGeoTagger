SUBDIRS( ui plugins providers )

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
     app/egtexifeditor.h
     app/egtexiftagcontrol.h
     app/egtexiftaggroup.h
     app/egtplugininterface.h
     app/egtpythonconsolebase.h

     core/egtexifengine.h
     core/egtgpsexifengine.h
     core/egtimagefactory.h
     core/egtlogger.h
     core/egtpathbuilder.h
     core/egtphotoexifengine.h
     core/egtrawimagereader.h
     core/egtdataprovider.h
     core/egtgpsdatatable.h

   )

SET( EGT_CORE_SRCS
     core/egtdataprovider.cpp
     core/egtexifengine.cpp
     core/egtgpsdatatable.cpp
     core/egtgpsexifengine.cpp
     core/egtimagefactory.cpp
     core/egtpathbuilder.cpp
     core/egtphotoexifengine.cpp
     core/egtrawimagereader.cpp
   )

SET( EGT_CORE_MOC_HDRS
     core/egtdataprovider.h
     core/egtgpsdatatable.h
     core/egtimagefactory.h
     core/egtrawimagereader.h
   )

SET( EGT_APP_SRCS
    app/egtapplication.cpp
    app/egtapplicationinterface.cpp
    app/egtdataprovidermanager.cpp
    app/egtexifeditor.cpp
    app/egtexiftagcontrol.cpp
    app/egtexiftaggroup.cpp
    app/egtitemdelegate.cpp
    app/egtmainwindow.cpp
    app/egtpluginmanager.cpp
    app/egtplugindisplaywidget.cpp

   )

SET( EGT_APP_MOC_HDRS
    app/egtapplication.h
    app/egtapplicationinterface.h
    app/egtdataprovidermanager.h
    app/egtexifeditor.h
    app/egtexiftagcontrol.h
    app/egtexiftaggroup.h
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

#A little hack to get the main lib to always relink, stop a infinit loop with the python build
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/forcerelink.in_cmake
               ${CMAKE_CURRENT_BINARY_DIR}/forcerelink.cmake
               @ONLY)
ADD_CUSTOM_TARGET( egt_force_relink COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/forcerelink.cmake )
#end hack

ADD_LIBRARY ( easygt SHARED ${EGT_CORE_SRCS} ${EGT_CORE_MOC_SRCS} ${EGT_APP_SRCS} ${EGT_APP_MOC_SRCS} ${EGT_APP_RCCS_SRCS} ${EGT_PYTHON_SRCS} ${EGT_PYTHON_MOC_SRCS} )

ADD_DEPENDENCIES( easygt easygt_ui egt_force_relink )

TARGET_LINK_LIBRARIES( easygt
  ${QT_LIBRARIES}
  ${EXIV2_LIBRARY}
  ${ZLIB_LIBRARY}
  ${EXPAT_LIBRARY}
  ${LIBRAW_LIBRARY}
  ${PYTHON_LIBRARY}
  )

INSTALL( FILES ${EGT_INSTALL_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})
INSTALL( TARGETS easygt DESTINATION ${EASYGT_LIB_SUBDIR} )
