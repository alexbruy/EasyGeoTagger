IF (WIN32)
# expect that classes are being exported
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllexport)\"")
  ENDIF (MSVC)
ENDIF (WIN32)

SET( EGT_CORE_ALL_HDRS
     egtexifioengine.h
     egtimageengine.h
     egtlogger.h
     egtpathbuilder.h
     egtrawthread.h
   )

SET( EGT_CORE_SRCS
     egtexifioengine.cpp
     egtimageengine.cpp
     egtpathbuilder.cpp
     egtrawthread.cpp
   )

SET( EGT_CORE_MOC_HDRS
     egtimageengine.h
     egtrawthread.h
   )

QT4_AUTOMOC( ${EGT_CORE_SRCS} )

QT4_WRAP_CPP( EGT_CORE_MOC_SRCS ${EGT_CORE_MOC_HDRS} )

ADD_LIBRARY ( easygt_core SHARED ${EGT_CORE_SRCS} ${EGT_CORE_MOC_SRCS} )

TARGET_LINK_LIBRARIES( easygt_core 
	${QT_LIBRARIES} 
	${EXIV2_LIBRARY}
    ${ZLIB_LIBRARY}	
	${EXPAT_LIBRARY}
	${LIBRAW_LIBRARY} 
	)

INSTALL( TARGETS easygt_core DESTINATION ${EASYGT_LIB_SUBDIR} )
INSTALL( FILES ${EGT_CORE_ALL_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})
