# Based on FindEXIV2 Contributed by Tim Sutton 
#
# CMake module to search for LIBUSB library
#
# If it's found it sets LIBUSB_FOUND to TRUE
# and following variables are set:
#    LIBUSB_INCLUDE_DIR
#    LIBUSB_LIBRARY


FIND_PATH(LIBUSB_INCLUDE_DIR usb.h
  /usr/local/include/
  /usr/include/
  #MSVC
  "$ENV{LIB_DIR}/include"
  "c:/libs_babel/libusb-win32-device-bin-0.1.12.1/include"
  #mingw
  c:/msys/local/include
  )

FIND_LIBRARY(LIBUSB_LIBRARY NAMES usb PATHS 
  /usr/local/lib 
  /usr/lib 
  #MSVC
  "$ENV{LIB_DIR}/lib"
  "C:/libs_babel/libusb-win32-device-bin-0.1.12.1/lib/msvc"
  #mingw
  c:/msys/local/lib
  )

IF (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARY)
   SET(LIBUSB_FOUND TRUE)
ENDIF (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARY)


IF (LIBUSB_FOUND)

   IF (NOT LIBUSB_FIND_QUIETLY)
      MESSAGE(STATUS "Found LIBUSB: ${LIBUSB_LIBRARY}")
      MESSAGE(STATUS "Found LIBUSB: ${LIBUSB_INCLUDE_DIR}")
   ENDIF (NOT LIBUSB_FIND_QUIETLY)

ELSE (LIBUSB_FOUND)

   IF (LIBUSB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find LIBUSB")
   ENDIF (LIBUSB_FIND_REQUIRED)

ENDIF (LIBUSB_FOUND)
