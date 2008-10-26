
IF (WIN32)
  # expect that classes are being imported
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  IF (MSVC)
    ADD_DEFINITIONS("-DDllDef=__declspec(dllexport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UDllDef \"-DDllDef=__declspec(dllexport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DDllDef=)
ENDIF (WIN32)

ADD_DEFINITIONS(-DLIBRAW_BUILDLIB)

INCLUDE_DIRECTORIES(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

SET( LIBRAW_SRCS
     internal/dcraw_common.cpp
     internal/foveon.cpp
     src/libraw_c_api.cpp
     src/libraw_cxx.cpp
   )

ADD_LIBRARY ( raw SHARED ${LIBRAW_SRCS} )

TARGET_LINK_LIBRARIES( raw wsock32 )