SET(EGT_GUI_SRCS
    egtitemdelegate.cpp
    egtmainwindow.cpp
    egtplugindisplaywidget.cpp
    egtplugindock.cpp
   )
   
SET(EGT_GUI_MOC_HDRS
    egtitemdelegate.h
    egtmainwindow.h
    egtplugindisplaywidget.h
    egtplugindock.h
   )

SET (EGT_GUI_RCCS gui.qrc)

QT4_ADD_RESOURCES(EGT_GUI_RCCS_SRCS ${EGT_GUI_RCCS})

QT4_AUTOMOC(${EGT_GUI_SRCS})

QT4_WRAP_CPP(EGT_GUI_MOC_SRCS ${EGT_GUI_MOC_HDRS})

ADD_LIBRARY (egt_gui SHARED ${EGT_GUI_SRCS} ${EGT_GUI_MOC_SRCS} ${EGT_GUI_RCCS_SRCS})

ADD_DEPENDENCIES(egt_gui egt_ui egt_core)

TARGET_LINK_LIBRARIES(egt_gui ${QT_LIBRARIES} )

INSTALL(TARGETS egt_gui DESTINATION lib)