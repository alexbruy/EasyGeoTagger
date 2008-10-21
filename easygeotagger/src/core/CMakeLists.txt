SET( EGT_CORE_ALL_HDRS
     egtexifio.h
     egtimageengine.h
     egtlogger.h
     egtpathbuilder.h
   )

SET( EGT_CORE_SRCS
     egtexifio.cpp
     egtimageengine.cpp
     egtpathbuilder.cpp
   )

SET( EGT_CORE_MOC_HDRS
     egtimageengine.h
   )

QT4_AUTOMOC( ${EGT_CORE_SRCS} )

QT4_WRAP_CPP( EGT_CORE_MOC_SRCS ${EGT_CORE_MOC_HDRS} )

ADD_LIBRARY ( easygt_core SHARED ${EGT_CORE_SRCS} ${EGT_CORE_MOC_SRCS} )

TARGET_LINK_LIBRARIES( easygt_core ${QT_LIBRARIES} ${EXIV2_LIBRARY} ${LIBRAW_LIBRARY} )

INSTALL( TARGETS easygt_core DESTINATION ${EASYGT_LIB_SUBDIR} )
INSTALL( FILES ${EGT_CORE_ALL_HDRS} DESTINATION ${EASYGT_INCLUDE_SUBDIR})