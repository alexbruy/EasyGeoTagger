SET(EGT_UIS
    egtmainwindowgui.ui
    egtpythonconsoledialog.ui
    egtkeyselectiondialog.ui
    egtdelimitedtextconfigurationdialog.ui
   )
   
QT4_WRAP_UI(EGT_UIS_H ${EGT_UIS})

ADD_CUSTOM_TARGET(easygt_ui ALL DEPENDS ${EGT_UIS_H})

#INSTALL( FILES ${EGT_UIS_H} DESTINATION ${EASYGT_INCLUDE_SUBDIR})
