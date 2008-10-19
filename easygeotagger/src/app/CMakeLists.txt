SET( EGT_APP_SRCS
    egtapplication.cpp
    egtapplicationinterface.cpp
    egtpluginmanager.cpp
   )
SET( EGT_APP_MOC_HDRS
    egtpluginmanager.h
   )

QT4_AUTOMOC( ${EGT_APP_SRCS} )

QT4_WRAP_CPP( EGT_APP_MOC_SRCS ${EGT_APP_MOC_HDRS} )

ADD_LIBRARY (egt_app SHARED ${EGT_APP_SRCS} ${EGT_APP_MOC_SRCS} )

ADD_DEPENDENCIES( egt_app egt_ui egt_core )

TARGET_LINK_LIBRARIES( egt_app ${QT_LIBRARIES} )

INSTALL( TARGETS egt_app DESTINATION lib )