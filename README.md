Introduction
The instructions below will guide you through downloading and setting up the fife engine as well as the fife-tutorials.

Windows Install
Download the FIFE 0.3.2 win32 installer from sourceforge and run the installer
Checkout the fife-tutorials trunk to the location <path-to-fife>/tutorials
Using Visual Studio
Execute the script <path-to-fife>/build/win32/update_project_files.bat
Navigate to <path-to-fife>/build/win32/build_environments/visual_studio_# where # is the version of visual studio you are using
Open the solution fife_engine.sln
Go to the build configurations menu within visual studio and select either the "Debug_static" or "Release_static" configuration
Build the solution and make sure no errors occurred
Navigate to <path-to-fife>/tutorials/build/ and choose the directory based on which version of visual studio you are using
Open the solution (.sln) file within the directory
Set the build configuration within visual studio to either "Debug" or "Release" to correspond to how FIFE was built
Build solution and make sure no errors occurred

Using Mingw
TBD

Linux Install
TBD
