@echo off

set configuration=Release

:: configure

d:\cmake\bin\cmake src/CMakeLists.txt -B./build -DCMAKE_INSTALL_PREFIX=./tutorials

:: make

d:\cmake\bin\cmake --build ./build --target ALL_BUILD --config %configuration% -- /nologo

:: install

d:\cmake\bin\cmake --build ./build --target INSTALL --config %configuration% -- /nologo

:: package (with cpack)

: d:\cmake\bin\cmake --build ./build --target PACKAGE --config %configuration% -- /nologo

pause