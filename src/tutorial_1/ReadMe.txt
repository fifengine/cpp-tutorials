
Fife tutorial 1

This is the first tutorial based on the FIFE engine. 

The tutorial will go through the following:
	1) Engine initialization and setup
	2) Setting up user input
	3) Loading a map
	4) Manipulating the camera through input keys and mouse
	5) Moving characters on screen
	6) Auto screen scrolling based on mouse cursor position
	
Directory layout
All code for this tutorial can be found in the src directory.
The art assets used for this demo are located in the assets directory.
The visual studio 2008 solution can be found in the build/msvc2008 directory.

Build
Place this tutorial in the <path_to_fife>/demos folder. To build open the 
visual studio solution in the build/msvc2008 directory and run the build
command. If you do not place this in the demos directory of fife you will
need to modify the project properties to correctly point to where fife is
installed.

Dependencies
This package requires the FIFE engine release 0.3.1 (http://fifengine.net)
The latest win32 installer can be found here (includes all dependencies):
	http://sourceforge.net/projects/fife/files/active/packages/FIFE-0.3.1_installer_win32.exe/download
The FIFE source code can be found here:
	http://sourceforge.net/projects/fife/files/active/src/fife-0.3.1.tar.gz/download
