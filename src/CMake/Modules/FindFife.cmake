#------------------------------------------------------------------------------
# FindFife.cmake
#------------------------------------------------------------------------------
#
# Find the native fife includes and libraries. Detect Fife version.
#
# FIFE_INCLUDE_DIR  - where to find fife/core/version.h, etc.
# FIFE_LIBRARIES    - List of libraries when using fife.
# FIFE_FOUND        - True, if fife found.
#
#------------------------------------------------------------------------------
# Author: Jens A. Koch <jakoch@web.de>
#------------------------------------------------------------------------------

if(FIFE_INCLUDE_DIR)
    # Already in cache, be silent
    set(FIFE_FIND_QUIETLY TRUE)
endif(FIFE_INCLUDE_DIR)

find_path(FIFE_INCLUDE_DIR fife/core/version.h)

# append "fife/core" to "include" path
set(FIFE_INCLUDE_DIR ${FIFE_INCLUDE_DIR}/fife/core)

#------------------------------------------------------------------------------
#                           Detect Version
#------------------------------------------------------------------------------

if(FIFE_INCLUDE_DIR AND EXISTS "${FIFE_INCLUDE_DIR}/version.h")
   file(STRINGS "${FIFE_INCLUDE_DIR}/version.h" FIFE_VERSION_LINE REGEX "^(.+)([0-9]+)\\.([0-9]+)\\.([0-9]+)\"$")
   string(REGEX REPLACE "^[ \t]#define FIFE_VERSION \"([0-9]+)\\.([0-9]+)\\.([0-9]+)\"$" "\\1.\\2.\\3" FIFE_VERSION "${FIFE_VERSION_LINE}")
   set(FIFE_VERSION_STRING ${FIFE_VERSION})
   unset(FIFE_VERSION_LINE)
   unset(FIFE_VERSION)
endif()

# MSVC built fife may be named fife_static.
# The provided project files name the library with the lib prefix.
find_library(FIFE_LIBRARY NAMES fife fife_static libfife libfife_static)

# Handle the QUIETLY and REQUIRED arguments and set FIFE_FOUND
# to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(FIFE 
    REQUIRED_VARS FIFE_LIBRARY FIFE_INCLUDE_DIR
    VERSION_VAR   FIFE_VERSION_STRING
)

if(FIFE_FOUND)
    set(FIFE_LIBRARIES ${FIFE_LIBRARY})
endif(FIFE_FOUND)

mark_as_advanced(FIFE_INCLUDE_DIR FIFE_LIBRARY)