SET(EGT_CORE_SRCS
    egtimageengine.cpp
    egtpathbuilder.cpp
    egtexifio.cpp
   )

SET(EGT_CORE_MOC_HDRS
    egtimageengine.h
    egtplugininterface.h
   )

QT4_AUTOMOC(${EGT_CORE_SRCS})

QT4_WRAP_CPP(EGT_CORE_MOC_SRCS ${EGT_CORE_MOC_HDRS})

ADD_LIBRARY (egt_core SHARED ${EGT_CORE_SRCS} ${EGT_CORE_MOC_SRCS})

TARGET_LINK_LIBRARIES(egt_core ${QT_LIBRARIES} ${EXIV2_LIBRARY} ${LIBRAW_LIBRARY})

INSTALL(TARGETS egt_core DESTINATION lib)