#------------------------------------------------------------------------------
#                          Fifengine C++ Tutorials                                         
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

project(fife-cpp-tutorials)

#------------------------------------------------------------------------------
#                                 Configure                                          
#------------------------------------------------------------------------------

# set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Create a "Release" build by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Select build type. Options are: 'Debug', 'Release', 'RelWithDebInfo' and 'MinSizeRel'."
    FORCE
  )
  message(STATUS "Set CMAKE_BUILD_TYPE to default '${CMAKE_BUILD_TYPE}'.")
endif(NOT CMAKE_BUILD_TYPE)

# Do not allow an in-source-tree build, request an out-of-source-tree build.
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "#Please build outside of the source tree!\n                        
                       Make a separate folder for building and run CMake there:\n
                       mkdir _build; cd _build; cmake ..\n
                       Before that, cleanup:\nrm -rf CMakeCache.txt CMakeFiles")
endif()

# Disable in-source builds and modifications to the source tree.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

#------------------------------------------------------------------------------
#                                 Display Status                                         
#------------------------------------------------------------------------------

message("")
message("== Building ${PROJECT_NAME} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "    Source directory is '${PROJECT_SOURCE_DIR}'.")
message(STATUS "    Build  directory is '${PROJECT_BINARY_DIR}'.")
message(STATUS "    Installation path is '${CMAKE_INSTALL_PREFIX}' (set -DCMAKE_INSTALL_PREFIX=<path> to change).")
message(STATUS "    CXX Compiler Flags: '${CMAKE_CXX_FLAGS}'.")
message(STATUS "    Dependencies folder: '${PROJECT_SOURCE_DIR}/../dependencies'.")
message("")

#------------------------------------------------------------------------------
#                                 Compiler Options                                         
#------------------------------------------------------------------------------

if(WIN32)

  if(MSVC)

    # ensure we use minimal "windows.h" lib without the crazy min max macros    
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"WIN32_LEAN_AND_MEAN\"")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D \"NOMINMAX\"")

    # disable SAFESEH - to avoid "LNK2026: module unsafe" on "OpenAL32.lib"
    SET(CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} /D \"SAFESEH:NO\"")
    SET(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

  endif(MSVC)

endif(WIN32)

#------------------------------------------------------------------------------
#                           Find Dependencies - Windows                                        
#------------------------------------------------------------------------------

# Tell CMake where to search for binaries (dlls), includes and libs on Windows
if(WIN32)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "${PROJECT_SOURCE_DIR}/../dependencies/bin"
    "${PROJECT_SOURCE_DIR}/../dependencies/include"
    "${PROJECT_SOURCE_DIR}/../dependencies/include/SDL2"
    "${PROJECT_SOURCE_DIR}/../dependencies/include/OpenAL"
    "${PROJECT_SOURCE_DIR}/../dependencies/lib/x86"
    "${PROJECT_SOURCE_DIR}/../dependencies/lib"
    "${PROJECT_SOURCE_DIR}/../dependencies/lib/boost" 
  )
  set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_PREFIX_PATH})
endif(WIN32)

#------------------------------------------------------------------------------	
# 							   Configure Boost
#------------------------------------------------------------------------------

add_definitionS(-DBOOST_ALL_NO_LIB)

# use static libs
set(Boost_USE_STATIC_LIBS ON)     
set(Boost_USE_MULTITHREADED ON)

#------------------------------------------------------------------------------
#                       Add Subdirectories for Tutorials
#------------------------------------------------------------------------------

add_subdirectory(tutorial_1)

#------------------------------------------------------------------------------
#                            Package Tutorials
#------------------------------------------------------------------------------

# include system runtimes (e.g. msvcp140.dll) into "<install_prefix>/bin" folder 
# how can i change/remove the "bin" suffix to install into <install_prefix>?
include(InstallRequiredSystemLibraries)

# set package type depending on OS
#if(WIN32)
#  set(CPACK_GENERATOR ${CPACK_GENERATOR} ZIP)
#elseif(APPLE)
#  set(CPACK_GENERATOR ${CPACK_GENERATOR} DragNDrop)
#else()
#  set(CPACK_GENERATOR ${CPACK_GENERATOR} TGZ)
#endif()

#set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")

# tell CPack all of the components to install..
#set(CPACK_COMPONENTS_ALL executable libraries)

#set(CPACK_MONOLITHIC_INSTALL on)

# tell cpack to strip debug symbols from files
#set(CPACK_STRIP_FILES 1)

# this must be the last call
#include(CPack)