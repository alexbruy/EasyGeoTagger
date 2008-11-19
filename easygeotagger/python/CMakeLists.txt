SET(BINDINGS_MAKEFILE ${CMAKE_CURRENT_BINARY_DIR}/Makefile)

#If statements needed for platform specific libs
SET(EASYGT_LIB ${CMAKE_BINARY_DIR}/src/libeasygt.so)
SET(BINDINGS_LIB ${CMAKE_CURRENT_BINARY_DIR}/PyEasyGT.so)

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
