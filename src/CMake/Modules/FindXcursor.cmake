#------------------------------------------------------------------------------
# FindXcursor.cmake
#------------------------------------------------------------------------------
#
# Find the X11 Xcursor lib and define the following vars:
#
#  XCURSOR_FOUND         - true, if XCURSOR_INCLUDE_DIR and XCURSOR_LIBRARY are found
#  XCURSOR_LIBRARIES     - Set when XCURSOR_LIBRARY is found
#  XCURSOR_INCLUDE_DIRS  - Set when XCURSOR_INCLUDE_DIR is found
#
#  XCURSOR_INCLUDE_DIR   - where to find Xcursor.h
#  XCURSOR_LIBRARY       - the Xcursor library
#
#------------------------------------------------------------------------------
# Author: Jens A. Koch <jakoch@web.de>
#------------------------------------------------------------------------------

find_path(XCURSOR_INCLUDE_DIR NAMES X11/Xcursor/Xcursor.h
          PATH_SUFFIXES X11/Xcursor
          PATHS /opt/X11/include
)

find_library(XCURSOR_LIBRARY NAMES Xcursor
          PATHS /opt/X11/lib          
)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xcursor DEFAULT_MSG XCURSOR_LIBRARY XCURSOR_INCLUDE_DIR)

if(XCURSOR_FOUND)
  set( XCURSOR_LIBRARIES ${XCURSOR_LIBRARY} )
  set( XCURSOR_INCLUDE_DIRS ${XCURSOR_INCLUDE_DIR} )
endif()

mark_as_advanced(XCURSOR_INCLUDE_DIR XCURSOR_LIBRARY)
