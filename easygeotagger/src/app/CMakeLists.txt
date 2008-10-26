IF (WIN32)
# expect that classes are being exported
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllexport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

SET( EGT_APP_ALL_HDRS
    egtapplication.h
    egtapplicationinterface.h
    egtpluginmanager.h
    egtplugininterface.h
   )

SET( EGT_APP_SRCS
    egtapplication.cpp
    egtapplicationinterface.cpp
    egtpluginmanager.cpp
   )
   
SET( EGT_APP_MOC_HDRS
    egtapplicationinterface.h
    egtpluginmanager.h
    egtplugininterface.h
   )

QT4_AUTOMOC( ${EGT_APP_SRCS} )

QT4_WRAP_CPP( EGT_APP_MOC_SRCS ${EGT_APP_MOC_HDRS} )

ADD_LIBRARY (easygt_app SHARED ${EGT_APP_SRCS} ${EGT_APP_MOC_SRCS} )

ADD_DEPENDENCIES( easygt_app easygt_core easygt_gui )

TARGET_LINK_LIBRARIES( easygt_app 
	easygt_core 
	easygt_gui
    ${EXPAT_LIBRARY}	
	${QT_LIBRARIES} 
	)

INSTALL( TARGETS easygt_app DESTINATION ${EASYGT_LIB_SUBDIR} )
INSTALL( FILES ${EGT_APP_ALL_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})