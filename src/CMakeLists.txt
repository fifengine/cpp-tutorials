# define the minimum cmake version supported
cmake_minimum_required(VERSION 2.6)

# define project name
project(fife-cpp-tutorials)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

if(WIN32)
	set(OUTPUT_DIR ${PROJECT_SOURCE_DIR}/../bin)
	set(LIBRARY_BASE_DIR ${PROJECT_SOURCE_DIR}/../../build/win32/static_libs)
	set(INC_BASE_DIR ${PROJECT_SOURCE_DIR}/../../build/win32/includes)
	set(EXT_DLL_BASE_DIR ${PROJECT_SOURCE_DIR}/../../build/win32/binaries)
	
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/boost_1_47_0;${CMAKE_INCLUDE_PATH}")
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/sdl_image;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/sdl;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/sdl_ttf;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/zlib;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/openal/AL;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/libpng;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/libvorbis;${CMAKE_INCLUDE_PATH})
	set(CMAKE_INCLUDE_PATH "${INC_BASE_DIR}/libogg;${CMAKE_INCLUDE_PATH})
	
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${OUTPUT_DIR})
	foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
		string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
	endforeach()
	
	if(MSVC10)
		set(CMAKE_LIBRARY_PATH ${LIBRARY_BASE_DIR}/msvc2010)
		set(EXT_DLL_DIR ${EXT_DLL_BASE_DIR}/msvc2010)
	elseif (MSVC90)
		set(CMAKE_LIBRARY_PATH ${LIBRARY_BASE_DIR}/msvc2008)
		set(EXT_DLL_DIR ${EXT_DLL_BASE_DIR}/msvc2008)
	elseif(MSVC80)
		set(CMAKE_LIBRARY_PATH ${LIBRARY_BASE_DIR}/msvc2005)
		set(EXT_DLL_DIR ${EXT_DLL_BASE_DIR}/msvc2005)
	elseif(MINGW)
		set(CMAKE_LIBRARY_PATH ${LIBRARY_BASE_DIR}/mingw)
		set(EXT_DLL_DIR ${EXT_DLL_BASE_DIR}/mingw)
	endif()
	
	# make sure boost options are set properly
	add_definitionS(-DBOOST_ALL_NO_LIB)
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_MULTITHREADED ON)
	unset(Boost_INCLUDE_DIR CACHE)
	unset(Boost_LIBRARY_DIRS CACHE)
endif()

# add src subdirectory to project
add_subdirectory(tutorial_1)