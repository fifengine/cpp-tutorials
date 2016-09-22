#------------------------------------------------------------------------------
#                          Fifengine C++ Tutorial 1                                        
#------------------------------------------------------------------------------

project(Tutorial1)

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

find_package(Fife REQUIRED)
find_package(FifeChan COMPONENTS opengl sdl REQUIRED)  
find_package(Boost 1.33.1 COMPONENTS system filesystem regex)
find_package(Vorbis REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(Ogg REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(TINYXML REQUIRED)

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
target_link_libraries(Tutorial1 ${OPENAL_LIBRARY})
target_link_libraries(Tutorial1 ${OPENGL_LIBRARY})
target_link_libraries(Tutorial1 ${ZLIB_LIBRARIES})
target_link_libraries(Tutorial1 ${PNG_LIBRARIES})
target_link_libraries(Tutorial1 ${SDL2_LIBRARY})
target_link_libraries(Tutorial1 ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(Tutorial1 ${SDL2_TTF_LIBRARIES})
target_link_libraries(Tutorial1 ${TinyXML_LIBRARIES})
target_link_libraries(Tutorial1 ${OGG_LIBRARIES})
target_link_libraries(Tutorial1 ${VORBIS_LIBRARIES})
target_link_libraries(Tutorial1 ${VORBIS_LIBRARY})
target_link_libraries(Tutorial1 ${VORBISFILE_LIBRARY})
target_link_libraries(Tutorial1 ${FIFECHAN_LIBRARIES})
target_link_libraries(Tutorial1 ${FIFE_LIBRARIES})

#------------------------------------------------------------------------------
#                         Install Tutorial 1                                        
#------------------------------------------------------------------------------

# install the binary/exe into the subfolder "Tutorial1"
install(TARGETS Tutorial1     
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    COMPONENT executable
)  

# set paths to executables
set(APP  ${CMAKE_INSTALL_PREFIX}/Tutorial1/Tutorial1${CMAKE_EXECUTABLE_SUFFIX}) 

# set paths to dependencies
set(DIRS ${PROJECT_SOURCE_DIR}/../../dependencies/bin
         ${PROJECT_SOURCE_DIR}/../../dependencies/lib
         ${PROJECT_SOURCE_DIR}/../../dependencies/lib/x86 
)   

# copy all libraries to the binary folder
# by calling fixup_bundle, when TARGET INSTALL is executed
install(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APP}\"   \"\"   \"${DIRS}\")
   " 
   COMPONENT libraries
)

#------------------------------------------------------------------------------
#                         Package Tutorial 1                                        
#------------------------------------------------------------------------------

# set package type depending on OS
#if(WIN32)
#  set(CPACK_GENERATOR ZIP)
#elseif(APPLE)
#  set(CPACK_GENERATOR DragNDrop)
#else()
#  set(CPACK_GENERATOR TGZ)
#endif()

# include system runtimes (e.g. msvcp140.dll) into "<install_prefix>/bin" folder 
# how can i change/remove the "bin" suffix to install into <install_prefix>?
#include(InstallRequiredSystemLibraries)

#set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")

# tell CPack all of the components to install..
#set(CPACK_COMPONENTS_ALL executable libraries)

# tell cpack to strip debug symbols from files
#set(CPACK_STRIP_FILES           1)

# this must be the last call
#include(CPack)