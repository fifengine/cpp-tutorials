#------------------------------------------------------------------------------
#                          Fifengine C++ Tutorial 1                                        
#------------------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

file(GLOB TUTORIAL1_SRC *.cpp *.h)

add_executable(Tutorial1 ${TUTORIAL1_SRC})
set_target_properties(Tutorial1 PROPERTIES DEBUG_POSTFIX "_d")

if(UNIX)
	include_directories(/usr/local/include/fife/core)
	set_property(TARGET fife PROPERTY IMPORTED_LOCATION /usr/local/lib/libfife.a)
	find_package(X11 REQUIRED)
	target_link_libraries(Tutorial1 ${X11_LIBRARY})
	target_link_libraries(Tutorial1 Xcursor)
	target_link_libraries(Tutorial1 fifechan)
	target_link_libraries(Tutorial1 fifechan_sdl)
	target_link_libraries(Tutorial1 fifechan_opengl)
	target_link_libraries(Tutorial1 vorbis)
endif()

if(WIN32)
    add_library(fife      		SHARED IMPORTED)
	add_library(fifechan  		SHARED IMPORTED)
	add_library(fifechan_sdl 	SHARED IMPORTED)
	add_library(fifechan_opengl SHARED IMPORTED)
	add_library(vorbis 			SHARED IMPORTED)
	add_library(vorbisfile 		SHARED IMPORTED)
	add_library(ogg 			SHARED IMPORTED)

	add_custom_command(
		TARGET Tutorial1 POST_BUILD
		COMMAND ${CMAKE_COMMAND} 
		-DEXT_DLL_DIR="${EXT_DLL_DIR}" 
		-DDEST_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
		-P "${CMAKE_SOURCE_DIR}/CopyExtDlls.cmake"
		COMMENT "copying dlls..."
	)

endif()

find_package(Boost 1.33.1 COMPONENTS system filesystem regex)
find_package(Fife REQUIRED)
find_package(FifeChan REQUIRED)
find_package(Ogg REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(TINYXML REQUIRED)
find_package(Vorbis REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(  
	${Boost_INCLUDE_DIR}
	${OGG_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${VORBIS_INCLUDE_DIR}
	${ZLIB_INCLUDE_DIR}
    ${FIFE_INCLUDE_DIR}
    ${FIFECHAN_INCLUDE_DIR}
)

target_link_libraries(Tutorial1 ${Boost_LIBRARIES})
target_link_libraries(Tutorial1 ${FIFE_LIBRARIES})
target_link_libraries(Tutorial1 ${FIFECHAN_LIBRARIES})
target_link_libraries(Tutorial1 ${OGG_LIBRARIES})
target_link_libraries(Tutorial1 ${OPENAL_LIBRARY})
target_link_libraries(Tutorial1 ${OPENGL_LIBRARY})
target_link_libraries(Tutorial1 ${PNG_LIBRARIES})
target_link_libraries(Tutorial1 ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(Tutorial1 ${SDL2_LIBRARY})
target_link_libraries(Tutorial1 ${SDL2_TTF_LIBRARIES})
target_link_libraries(Tutorial1 ${TinyXML_LIBRARIES})
target_link_libraries(Tutorial1 ${VORBIS_LIBRARIES})
target_link_libraries(Tutorial1 ${VORBIS_LIBRARY})
target_link_libraries(Tutorial1 ${VORBISFILE_LIBRARY})
target_link_libraries(Tutorial1 ${ZLIB_LIBRARIES})
