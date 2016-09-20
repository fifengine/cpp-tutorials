#------------------------------------------------------------------------------
#                          Fifengine C++ Tutorial 1                                        
#------------------------------------------------------------------------------

cmake_minimum_required (VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

file(GLOB TUTORIAL1_SRC *.cpp *.h)

add_executable(Tutorial1 ${TUTORIAL1_SRC})
set_target_properties(Tutorial1 PROPERTIES DEBUG_POSTFIX "_d")

add_library(fife STATIC IMPORTED)

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

	add_library(fifechan STATIC IMPORTED)
	add_library(fifechan_sdl STATIC IMPORTED)
	add_library(fifechan_opengl STATIC IMPORTED)
	add_library(vorbis_static STATIC IMPORTED)
	add_library(vorbisfile_static STATIC IMPORTED)
	add_library(ogg_static STATIC IMPORTED)

	find_library(FIFE_LIB fife)
	set_property(TARGET fife PROPERTY IMPORTED_LOCATION ${FIFE_LIB})

	find_library(FIFECHAN fifechan)
	set_property(TARGET fifechan PROPERTY IMPORTED_LOCATION ${FIFECHAN})
	find_library(FIFECHAN_SDL fifechan_sdl)
	set_property(TARGET fifechan_sdl PROPERTY IMPORTED_LOCATION ${FIFECHAN_SDL})
	find_library(FIFECHAN_OPENGL fifechan_opengl)
	set_property(TARGET fifechan_opengl PROPERTY IMPORTED_LOCATION ${FIFECHAN_OPENGL})
	target_link_libraries(Tutorial1 fifechan fifechan_sdl fifechan_opengl)	

	find_library(VORBIS vorbis_static)
	set_property(TARGET vorbis_static PROPERTY IMPORTED_LOCATION ${VORBIS})
	target_link_libraries(Tutorial1 vorbis_static)

	find_library(VORBIS_FILE vorbisfile_static)
	set_property(TARGET vorbisfile_static PROPERTY IMPORTED_LOCATION ${VORBIS_FILE})
	target_link_libraries(Tutorial1 vorbisfile_static)

	find_library(OGG ogg_static)
	set_property(TARGET ogg_static PROPERTY IMPORTED_LOCATION ${OGG})
	target_link_libraries(Tutorial1 ogg_static)
	
	add_custom_command(
		TARGET Tutorial1 POST_BUILD
		COMMAND ${CMAKE_COMMAND} 
		-DEXT_DLL_DIR="${EXT_DLL_DIR}" 
		-DDEST_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}"
		-P "${CMAKE_SOURCE_DIR}/CopyExtDlls.cmake"
		COMMENT "copying dlls..."
	)

endif()

#find_package(FIFE REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost 1.33.1 COMPONENTS system filesystem regex)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(TINYXML REQUIRED)
find_package(Ogg REQUIRED)
find_package(Vorbis REQUIRED)

include_directories(    
    ${PROJECT_SOURCE_DIR}/../dependencies/include/fife/core
    ${PROJECT_SOURCE_DIR}/../dependencies/include/fifechan
    ${FIFE_INCLUDE_DIR}
    ${FIFECHAN_INCLUDE_DIR}
	${SDL2_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
	${ZLIB_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
	${OGG_INCLUDE_DIR}
	${VORBIS_INCLUDE_DIR}
)

target_link_libraries(Tutorial1 ${Boost_LIBRARIES})
target_link_libraries(Tutorial1 ${FIFE_LIBRARY})
target_link_libraries(Tutorial1 ${FIFECHAN_LIBS})
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
