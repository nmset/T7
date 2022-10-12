# 

# - Try to find Hidapi
# Once done this will define
#  HIDAPI_FOUND - System has Hidapi
#  HIDAPI_INCLUDE_DIRS - The Hidapi include directories
#  HIDAPI_LIBRARIES - The libraries needed to use Hidapi
#  HIDAPI_DEFINITIONS - Compiler switches required for using Hidapi

#find_package(PkgConfig)
#pkg_check_modules(PC_HIDAPI QUIET )
#set(HIDAPI_DEFINITIONS ${PC_HIDAPI_CFLAGS_OTHER})

find_path(HIDAPI_INCLUDE_DIR hidapi/hidapi.h
          HINTS ${PC_HIDAPI_INCLUDEDIR} ${PC_HIDAPI_INCLUDE_DIRS}
          PATH_SUFFIXES hidapi )

find_library(HIDAPI_LIBRARY NAMES libhidapi-hidraw.so
             HINTS ${PC_HIDAPI_LIBDIR} ${PC_HIDAPI_LIBRARY_DIRS} )

set(HIDAPI_LIBRARIES ${HIDAPI_LIBRARY} )
set(HIDAPI_INCLUDE_DIRS ${HIDAPI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set HIDAPI_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Hidapi  DEFAULT_MSG
                                  HIDAPI_LIBRARY HIDAPI_INCLUDE_DIR)
mark_as_advanced(HIDAPI_INCLUDE_DIR HIDAPI_LIBRARY )
