## Building Binsim on Windows 11

These instructions were written in 2025 for an install on Windows 11. Anticipating most users will be familiar with a Unix-like environment they use MSYS2 and mingw64 to create a Unix-like environment within Windows complete with a root filesystem, many familiar Unix/GNU tools, and the GNU compilers like g++. OpenGL support is based a combination of the OpenGL implementation built in to Windows and the FreeGLUT library.

I'm sure it would be possible to build Binsim with Visual Studio, CMake, etc for those familiar with that environment. I had Visual Studio working 20 years ago, but couldn't get the libraries to work this time around, among issues. Please feel free to create a fork and add instructions and makefiles if that works for you.

### MSYS2/MINGW64 Instructions

1. Install MSYS2 from https://www.msys2.org/. Installation instructions are on the front page. It will open a MSYS2 UCRT64 terminal window. DON'T USE IT. Instead open a mingw64 terminal (you can type mingw64 in the Windows search if you don't have a shortcut). They have different paths set up and the instructions below are for mingw64.

2. Install development tools. This can be done within the MSYS2 terminal using the pacman package manager. Space can probably be saved here, but I just installed all.

   *$ pacman -S --needed base-devel mingw-w64-x86_64-toolchain*

3. Install freeglut as above:

   *$ pacman -S mingw-w64-x86_64-freeglut*

4. Install libjpeg as above:

   *$ pacman -S mingw-w64-x86_64-libjpeg-turbo*

5. Optionally download emacs to edit makefile and parameter files. Or use any other editor.

   *$ pacman -S mingw-w64-x86_64-emacs*

This will have created a Unix-like environment within C:\msys64. Many unix commands will be available (pwd, ls, grep, etc). The packages have been installed to C:\msys64\mingw64 (include and lib directories).

6. Install git as above

   *$ pacman -S git*

7. Download the Binsim repo

   *$ git clone https://github.com/RobertHynes/binsim.git*

8. Change to the main Binsim directory and edit the Makefile. After all the steps above this should be a matter of making sure the compiler is set to g++, uncommenting the Windows section, commenting out sections for other operating systems, and commenting out the OSBinsim section (not supported with this Binsim configuration at least). 

9. Build the package.

   *$ make*

10. Edit parameter files to taste and run

    *$ ./binsim sample.par*






