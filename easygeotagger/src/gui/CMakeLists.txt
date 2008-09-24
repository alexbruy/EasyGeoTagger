SET(EGT_GUI_SRCS
    egtitemdelegate.cpp
    egtmainwindow.cpp
   )
   
SET(EGT_GUI_MOC_HDRS
    egtitemdelegate.h
    egtmainwindow.h
   )

QT4_AUTOMOC(${EGT_GUI_SRCS})

QT4_WRAP_CPP(EGT_GUI_MOC_SRCS ${EGT_GUI_MOC_HDRS})

ADD_LIBRARY (egt_gui SHARED ${EGT_GUI_SRCS} ${EGT_GUI_MOC_SRCS})

TARGET_LINK_LIBRARIES(egt_gui ${QT_LIBRARIES} ${GDAL_LIBRARY})

INSTALL(TARGETS egt_gui DESTINATION lib)