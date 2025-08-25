# Installation Instructions for Linux #

See README_MacOS.md and README_Windows.md for other platforms.

## COMPILATION ##

These instructions and the makefile were tested on Ubuntu 22.04

To compile simply untar the source code, uncomment the relevant block
within the makefile and type make.  Depending on your environment you 
may need to further edit the Makefile to point to Mesa and libjpeg 
libraries and headers.  The only file required is binsim (and 
optionally osbinsim), so if you wish you can copy this to your
preferred bin directory.

## REQUIRED LIBRARIES ##

### 3D library ###

This program requires an OpenGL library or equivalent to be present.
I have used the free Mesa library (https://mesa3d.org/). Commercial OpenGL
implementations should work as well; BinSim has been found to work
with the Apple licensed OpenGL included with MacOS X (see
README_MacOS) and on Windows (see README_Windows).  The GL Utility
Toolkit, GLUT, is also required but this is included with Mesa.  If you 
are installing Mesa yourself, then get both the MesaLib and MesaDemos 
packages (or the whole CVS tree) as GLUT is contained in the latter.

If you are not sure if you have an OpenGL compatible library what you
need to look for are:

1) Headers.  There should be a subdirectory GL in one of the header
   directories, i.e. /usr/include/GL, /usr/local/include/GL,
   /usr/X11R6/include/GL.

2) Libraries.  The libraries libGL.so, libGLU.so and libglut.so and/or
   .a counterparts should be in a library directory - /usr/lib,
   /usr/local/lib, /usr/X11R6/lib.

This is not an exhaustive list of places to look.  Some installations
may not be in standard directories, e.g. you may have
/usr/local/Mesa-XXXX/include and /usr/local/Mesa-XXXX/lib.  You may have
to resort to a global search to find them.

### Mesa 16 bit per channel support ###

Note: This section is old and has not been tested recently across
multiple platforms yet.

16 bits per channel appears to be a linux only option.  To enable it
you need to edit the ${MESADIR}/src/config.h file and change #define
CHAN_BITS 8 to #define CHAN_BITS 16.  Then make Mesa as normal (make
linux) then do make linux-osmesa16 to make the additional 16 bit
library.  Then you need to edit the BinSim Makefile and comment out
the normal OSMesa definitions and uncomment the 16 bit definitions.
Then rebuild, make sure your LD_LIBRARY_PATH points to the right
directory and run it.  Unfortunately, with Mesa compiled in this way,
ordinary binsim does not work, so I think you need two Mesa
installations, one compiled as normal for binsim and one compiled with
16 bits per channel if you want the 16 bit version of osbinsim.  Since
this offers no benefit at present I don't see any point in going to
the trouble, but the option is there.  If anyone finds a way that the
two modes can coexist more easily, please let me know.

### Image libraries ###

For normal use, the program requires libjpeg or libjpeg-turbo (recommended).
Under Ubuntu Linux this is provided by the libjpeg8 and libjpeg8-devel 
packages (or libjpeg8-turbo and libjpeg8-turbo-dev), included in the
distribution.  Sources are available from https://libjpeg-turbo.org/.  If you
can't, or don't want to install libjpeg, then remove the -ljpeg from
the Makefile and add a CFLAGS option -DNOJPEG, and BinSim should
compile happily.

PPM output is also available.  This produces very large files (5-10x
larger than a JPEG with a quality of 100).  Compilation is now no
problem because BinSim now incorporates Joerg Schmalzl's PPM code from
the osdemo.c file in the Mesa distribution rather than using an
external library.  Because of the file size PPM output is only
recommended for making MPEGs, because JPEGs don't seem to work for
this.

## SUPPORTING PROGRAMS ##

Note: This section is old. mpeg_encode is still available in various 
places but it has not been tested recently.

To create MPEG movies (Anim + Save keywords both true) requires
mpeg_encode.  This is not necessary to compile BinSim, just to make
MPEGs.  



