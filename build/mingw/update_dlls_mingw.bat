IF EXIST ..\..\bin\mingw GOTO BIN
:BIN
xcopy ..\..\..\..\build\win32\binaries\mingw\*.dll ..\..\bin\mingw /D /C /R /I /K /Y
