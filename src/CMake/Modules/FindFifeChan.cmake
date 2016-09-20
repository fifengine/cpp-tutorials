# - FindFifeChan.cmake
# Find the native fifechan includes and libraries
#
# FIFECHAN_INCLUDE_DIR - where to find fifechan/fifechan.h, etc.
# FIFECHAN_LIBRARIES - List of libraries when using fifechan.
# FIFECHAN_FOUND - True if fifechan found.

if(FIFECHAN_INCLUDE_DIR)
    # Already in cache, be silent
    set(FIFECHAN_FIND_QUIETLY TRUE)
endif(FIFECHAN_INCLUDE_DIR)

find_path(FIFECHAN_INCLUDE_DIR fifechan/fifechan.hpp)

# MSVC built fifechan may be named fifechan_static.
# The provided project files name the library with the lib prefix.
find_library(FIFECHAN_LIBRARY NAMES fifechan fifechan_static libfifechan libfifechan_static)

# Handle the QUIETLY and REQUIRED arguments and set FIFECHAN_FOUND
# to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FIFECHAN DEFAULT_MSG FIFECHAN_LIBRARY FIFECHAN_INCLUDE_DIR)

if(FIFECHAN_FOUND)
    set(FIFECHAN_LIBRARIES ${FIFECHAN_LIBRARY})
endif(FIFECHAN_FOUND)

mark_as_advanced(FIFECHAN_INCLUDE_DIR FIFECHAN_LIBRARY)