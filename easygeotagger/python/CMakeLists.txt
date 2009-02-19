SET(BINDINGS_MAKEFILE ${CMAKE_CURRENT_BINARY_DIR}/Makefile)

IF (WIN32)
  # expect that classes are being imported
  # Note: MSVC doesn't like when the macros are quotes
  # and MSYS doesn't like them unqouted (bacause of braces)
  IF (MSVC)
    ADD_DEFINITIONS("-DMS_DLL_SPEC=__declspec(dllimport)")
	SET(DLL_IMPORT "-DMS_DLL_SPEC=__declspec(dllimport)")
  ELSE (MSVC)
    ADD_DEFINITIONS("-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllimport)\"")
	SET(DLL_IMPORT "-UMS_DLL_SPEC \"-DMS_DLL_SPEC=__declspec(dllimport)\"")
  ENDIF (MSVC)
ELSE (WIN32)
  ADD_DEFINITIONS(-DMS_DLL_SPEC=)
  SET(DLL_IMPORT "-DMS_DLL_SPEC=")
ENDIF (WIN32)

#If statements needed for platform specific libs
IF(WIN32)
  IF(MSVC)
    SET(EASYGT_LIB ${CMAKE_BINARY_DIR}/src/easygt.lib)
    SET(BINDINGS_LIB ${CMAKE_CURRENT_BINARY_DIR}/PyEasyGT.pyd)
  ELSE(MSVC)
    SET(EASYGT_LIB ${CMAKE_BINARY_DIR}/src/libeasygt.dll)
    SET(BINDINGS_LIB ${CMAKE_CURRENT_BINARY_DIR}/PyEasyGT.pyd)
  ENDIF(MSVC)
ELSE(WIN32)
  SET(EASYGT_LIB ${CMAKE_BINARY_DIR}/src/libeasygt.so)
  SET(BINDINGS_LIB ${CMAKE_CURRENT_BINARY_DIR}/PyEasyGT.so)
ENDIF(WIN32)

ADD_CUSTOM_TARGET (python ALL DEPENDS ${EASYGT_LIB} ${BINDINGS_LIB} )

FILE(GLOB SIP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.sip")

# Step 1: during configuration
# create file configure.py from configure.py.in
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/configure.py.in
               ${CMAKE_CURRENT_BINARY_DIR}/configure.py)

# Step 2: during make
# run python configure.py
# it will run SIP utility to generate sources and will prepare makefiles
# should be run everytime core or gui library has been changed
ADD_CUSTOM_COMMAND(OUTPUT ${BINDINGS_MAKEFILE} PRE_BUILD
                   COMMAND ${PYTHON_BINARY}
                   ARGS ${CMAKE_CURRENT_BINARY_DIR}/configure.py ${CMAKE_CFG_INTDIR} ${EXPORT}
                   DEPENDS ${EASYGT_LIB} ${CMAKE_CURRENT_BINARY_DIR}/configure.py ${SIP_FILES})

# Step 3: run make
ADD_CUSTOM_COMMAND(OUTPUT ${BINDINGS_LIB} PRE_LINK
                   COMMAND ${CMAKE_MAKE_PROGRAM}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   DEPENDS ${EASYGT_LIB} ${BINDINGS_MAKEFILE})


# Step 4: install built libs
INSTALL( FILES ${BINDINGS_LIB} DESTINATION ${PYTHON_PACKAGE_SUBDIR})
