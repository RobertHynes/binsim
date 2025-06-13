### 2025 Process

1. Install MSYS2 from https://www.msys2.org/. Installation instructions are on the front page.
2. Install GCC from mingw-64 https://www.mingw-w64.org/. This can actually be done within the MSYS2 terminal (MSYS2 UCRT64 in the Start menu) using the pacman package manager:

pacman -S mingw-w64-ucrt-x86_64-gcc

3. Install freeglut as above:

pacman -S mingw-w64-x86_64-freeglut

4. Install libjpeg as above:

pacman -S mingw-w64-x86_64-libjpeg-turbo

COMPILING BINSIM
----------------

I have only tried compiling with Visual C++ 6, so if you are using
something else you may have to make some changes.

Preludes - GLUT
---------------

You will need the GLUT library:

http://www.xmission.com/~nate/glut.html

glut-3.7.5-bin.zip (or later version) contains all you need.  You
should put glut.h in:

\Program Files\Microsoft Visual Studio\VC98\Include\GL

and glut32.lib in: 

\Program Files\Microsoft Visual Studio\VC98\Lib.

To run BinSim you will also need to install the glut32.dll file (see below).

Preludes - Libjpeg [Not applicable]
-----------------------------------

LIBJPEG SUPPORT IS CURRENTLY DISABLED, SO YOU CAN IGNORE THIS SECTION

You will also need libjpeg:

http://gnuwin32.sourceforge.net/packages/libjpeg.htm  

The libjpeg-6b-lib.zip packages contains the headers and libraries
needed to compile.  You will need to put the headers (everything from
include) in:

\Program Files\Microsoft Visual Studio\VC98\Include

and the libraries (everything from lib) in: 

\Program Files\Microsoft Visual Studio\VC98\Lib.

To run BinSim you will also deed the dlls from the libjpeg-6b-bin.zip
package (see below).

Compiling from the makefile
---------------------------

The easiest way to compile BinSim for Windows is to use the makefile
provided.  At the command prompt (MSDOS window) go into the binsim
directory and do:

nmake /f winbinsim.mak

If nmake is not recognised you will need to first run: 

C:\Program Files\Microsoft Visual Studio\VC98\Bin\vcvars32.bat

Compiling within Visual Studio
------------------------------

Create a new empty console application.  Import all the .h files and
all the .cxx ones (except osbinsim.cxx).  Change your
Project...Settings...Link options and add libjpeg.lib at the end.
Then you should be able to build the project.

RUNNING THE EXECUTABLE
----------------------

If you have a precompiled BinSim executable you will probably still
need to install libglut.dll (glut-3.7.5-bin.zip; see above) into your
Windows\System (or WINNT\system32) directory.

Once this is in place you can run BinSim from the command line
(MSDOS prompt) as on Unix platforms:

binsim sample.par

If you have an old version of Windows (Win95, possibly also Win98, but
not Win98SE or later) you may not have OpenGL installed.  I think all
you need is to get opengl.dll from the Microsoft site, but I haven't
tried this.

There is currently no GUI interface to BinSim, even for Windows.  It
is on the to do list, though, and I think a portable Java program to
provide a GUI front end should be fairly easy to put together. 
