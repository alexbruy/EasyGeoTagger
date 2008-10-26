IF (WIN32)
# expect that classes are being exported
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllexport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

SET(EGT_GUI_SRCS
    egtitemdelegate.cpp
    egtmainwindow.cpp
    egtplugindisplaywidget.cpp
   )
   
SET(EGT_GUI_MOC_HDRS
    egtitemdelegate.h
    egtmainwindow.h
    egtplugindisplaywidget.h
   )

SET (EGT_GUI_RCCS gui.qrc)

QT4_ADD_RESOURCES(EGT_GUI_RCCS_SRCS ${EGT_GUI_RCCS})

QT4_AUTOMOC(${EGT_GUI_SRCS})

QT4_WRAP_CPP(EGT_GUI_MOC_SRCS ${EGT_GUI_MOC_HDRS})

ADD_LIBRARY (easygt_gui SHARED ${EGT_GUI_SRCS} ${EGT_GUI_MOC_SRCS} ${EGT_GUI_RCCS_SRCS})

ADD_DEPENDENCIES(easygt_gui easygt_ui easygt_core)

TARGET_LINK_LIBRARIES(easygt_gui 
	easygt_core
	${QT_LIBRARIES}
	)

INSTALL(TARGETS easygt_gui DESTINATION ${EASYGT_LIB_SUBDIR} )
INSTALL( FILES ${EGT_GUI_MOC_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})