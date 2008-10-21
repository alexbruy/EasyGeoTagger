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

TARGET_LINK_LIBRARIES( easygt_app easygt_core easygt_gui ${QT_LIBRARIES} )

INSTALL( TARGETS easygt_app DESTINATION ${EASYGT_LIB_SUBDIR} )
INSTALL( FILES ${EGT_APP_ALL_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})