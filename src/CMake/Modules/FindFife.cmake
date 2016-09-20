# - FindFife.cmake
# Find the native fife includes and libraries
#
# FIFE_INCLUDE_DIR - where to find fife/core/version.h, etc.
# FIFE_LIBRARIES - List of libraries when using fife.
# FIFE_FOUND - True if fife found.

if(FIFE_INCLUDE_DIR)
    # Already in cache, be silent
    set(FIFE_FIND_QUIETLY TRUE)
endif(FIFE_INCLUDE_DIR)

find_path(FIFE_INCLUDE_DIR fife/core/version.h)

# MSVC built fife may be named fife_static.
# The provided project files name the library with the lib prefix.
find_library(FIFE_LIBRARY NAMES fife fife_static libfife libfife_static)

# Handle the QUIETLY and REQUIRED arguments and set FIFE_FOUND
# to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FIFE DEFAULT_MSG FIFE_LIBRARY FIFE_INCLUDE_DIR)

if(FIFE_FOUND)
    set(FIFE_LIBRARIES ${FIFE_LIBRARY})
endif(FIFE_FOUND)

mark_as_advanced(FIFE_INCLUDE_DIR FIFE_LIBRARY)