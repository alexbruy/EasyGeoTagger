SET(EGT_UIS
    egtmainwindowgui.ui
   )
   
QT4_WRAP_UI(EGT_UIS_H ${QGIS_UIS})

ADD_CUSTOM_TARGET(egt_ui ALL DEPENDS ${EGT_UIS_H})