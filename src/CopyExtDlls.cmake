# used to copy external dlls to the proper location on windows
IF (WIN32)
	FILE(GLOB DLLS ${EXT_DLL_DIR}/*.dll)
	FILE(COPY ${DLLS} DESTINATION ${DEST_DIR})
ENDIF()