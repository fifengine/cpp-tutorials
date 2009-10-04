IF EXIST ..\..\bin\msvc2005 GOTO BIN
:BIN
xcopy ..\..\..\..\build\win32\binaries\msvc2005\*.dll ..\..\bin\msvc2005 /D /C /R /I /K /Y



