IF EXIST ..\..\bin\msvc2008 GOTO BIN
:BIN
xcopy ..\..\..\..\build\win32\binaries\msvc2008\*.dll ..\..\bin\msvc2008 /D /C /R /I /K /Y

