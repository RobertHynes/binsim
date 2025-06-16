# Configure compiler - binary name and flags

# Use GNU GCC (g++) on Linux, some Unixes
CC = g++

# Some other Unixes may use a commercial compiler, often called CC
#CC = CC

# Use "c++" for MacOS X
#CC = c++

# Define flags for development version
#CFLAGS = -pg -g -Wall -ansi

# Define flags for release version
CFLAGS = -w -O

GLLIBDIR = -L"C:\Windows\System32"


###############################################################################
# Library and include file paths

# Define library and header directories
# My setup with Mesa compiled from source
#GLLIBDIR = -L/usr/local/mesa/lib
#JPEGLIBDIR = 
#X11LIBDIR = -L/usr/X11R6/lib
#GLINCLUDEDIR = -I/usr/local/mesa/include
#JPEGINCLUDEDIR = 
#X11INCLUDEDIR = -I/usr/X11R6/include

# Ubuntu test
GLLIBDIR =
JPEGLIBDIR = 
X11LIBDIR = 
GLINCLUDEDIR =
JPEGINCLUDEDIR = 
X11INCLUDEDIR = 

# Standard Redhat 6.2/7.0/7.1/7.2 setup
#GLLIBDIR =
#JPEGLIBDIR = 
#X11LIBDIR = -L/usr/X11R6/lib
#GLINCLUDEDIR =
#JPEGINCLUDEDIR = 
#X11INCLUDEDIR = -I/usr/X11R6/include

# Example Solaris setup; will need customisation since you will 
# likely have to install Mesa and/or libjpeg yourself
#GLLIBDIR = -L/usr/local/lib
#JPEGLIBDIR =
#X11LIBDIR = 
#GLINCLUDEDIR = -I/usr/local/include
#JPEGINCLUDEDIR =
#X11INCLUDEDIR =

# Mac OS X setup for XFree86, courtesy of Paul Ray (Paul.Ray@nrl.navy.mil)
# Uses XFree86 OpenGL and glut-3.7 from /usr/local (which you must compile)
# Get glut from http://www.opengl.org/developers/documentation/glut/#2
# Hopefully glut will soon be added to fink
#GLLIBDIR = -L/usr/local/lib/
#JPEGLIBDIR = -L/sw/lib/
#X11LIBDIR = -L/usr/X11R6/lib/
#GLINCLUDEDIR = -I/usr/local/include/
#JPEGINCLUDEDIR = -I/sw/include/
#X11INCLUDEDIR = -I/usr/X11R6/include/

# Mac OS X Native Aqua setup, courtesy of Paul Ray (Paul.Ray@nrl.navy.mil)
# Uses the built-in MacOS X GLUT framework
# Requires that you have done the following command in this directory:
#   ln -s /System/Library/Frameworks/GLUT.framework/Headers GL
# and have compiled libjpeg with fink (see http://fink.sourceforge.net)
# This version does not look good because it doesn't handle the NaNs
# that get passed to OpenGL by binsim very well.  Turn off HighQuality.
#GLLIBDIR = 
#JPEGLIBDIR = -L/sw/lib/
#X11LIBDIR = 
#GLINCLUDEDIR = -I./
#JPEGINCLUDEDIR = -I/sw/include/
#X11INCLUDEDIR = 

###############################################################################
# Libraries to link against

# Define libraries for all Unix+X11 setups I have tried (including Mac OS X
# with XFree86)
GLLIBS = -lglut -lGLU -lGL
XLIBS = -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE
JPEGLIBS = -ljpeg

# Define libraries for all Unix+X11 setups I have tried (including Mac OS X
# with XFree86)
#GLLIBS = -lglut -lGLU -lGL
GLLIBS = -lfreeglut -lglu32 -lopengl32
#XLIBS = -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE
XLIBS = 
JPEGLIBS = -ljpeg

## Mac OS X Native GL support
#GLLIBS = -framework OpenGL -framework GLUT -lobjc
#XLIBS = 
#JPEGLIBS = -ljpeg

###############################################################################
# OSMesa linkage

# OpenGL implementation other than Mesa or
# Mesa 3.4.2 or earlier or
# Mesa without OSMesa library 
#OSMESALIB = 
#OSMESAFLAGS = 

# Standard OSMesa for Mesa 3.5 or later
OSMESALIB = -lOSMesa
OSMESAFLAGS = 

# OSMesa with 16 bits per channel (ie 64bpp).  Only for Mesa 3.5 or later
#OSMESALIB = -lOSMesa16
#OSMESAFLAGS = -DBIGBUFFER

###############################################################################
# YOU SHOULD NOT NEED TO MODIFY ANYTHING BELOW HERE
###############################################################################

# Define libraries to use for linking
LIBS = ${GLLIBS} ${XLIBS} ${JPEGLIBS}

# Define include and library directories
INCLUDEDIR = ${GLINCLUDEDIR} ${JPEGINCLUDEDIR} ${X11INCLUDEDIR} 
LIBDIR = ${GLLIBDIR} ${JPEGLIBDIR} ${X11LIBDIR} 

# Define the names of the modules
OBJS = bbcolormodel.o binary3d.o binsim.o corona3d.o disc.o disc3d.o hotspot3d.o image_writer.o jet3d.o keyword.o keyword_translator.o lobe3d.o mathvec.o movie_maker.o object3d.o roche.o starsky.o stream.o stream3d.o stringutil.o transparent_disc3d.o transparent_object3d.o vertex_logger.o

# Recognised suffixes
.SUFFIXES:
.SUFFIXES: .cxx .o

.cxx.o:; ${CC} -c ${CFLAGS} ${OSMESAFLAGS} ${INCLUDEDIR} -o $@ $<

###############################################################################
# High level targets

binsim: gl_binsim.o ${OBJS}
	${CC} ${CFLAGS} ${LIBDIR} -o $@ gl_binsim.o ${OBJS} ${LIBS}

osbinsim: os_binsim.o ${OBJS}
	${CC} ${CFLAGS} ${LIBDIR} -o $@ os_binsim.o ${OBJS} ${OSMESALIB} ${LIBS}

clean: 
	rm -f binsim osbinsim gl_binsim.o osbinsim.o ${OBJS} *~

###############################################################################
# Object modules

bbcolormodel.o:  bbcolormodel.cxx bbcolormodel.h binsim_stdinc.h constants.h errmsg.h keyword.h mathvec.h
binary3d.o:  binary3d.cxx bbcolormodel.h binary3d.h binsim_stdinc.h constants.h corona3d.h disc3d.h errmsg.h hotspot3d.h jet3d.h keyword.h lobe3d.h mathvec.h object3d.h stream3d.h stream.h transparent_disc3d.h transparent_object3d.h
binsim.o:  binsim.cxx bbcolormodel.h binary3d.h binsim.h binsim_stdinc.h binsim_version.h constants.h corona3d.h disc3d.h errmsg.h hotspot3d.h image_writer.h jet3d.h keyword.h lobe3d.h mathvec.h movie_maker.h object3d.h starsky.h stream3d.h stream.h stringutil.h transparent_disc3d.h transparent_object3d.h vertex_logger.h
corona3d.o:  corona3d.cxx bbcolormodel.h binsim_stdinc.h constants.h corona3d.h disc.h keyword.h mathvec.h object3d.h roche.h stream.h surface.h transparent_object3d.h
disc3d.o:  disc3d.cxx bbcolormodel.h binsim_stdinc.h constants.h disc3d.h disc.h keyword.h mathvec.h object3d.h roche.h stream.h surface.h
disc.o:  disc.cxx binsim_stdinc.h constants.h disc.h mathvec.h roche.h surface.h
gl_binsim.o:  gl_binsim.cxx bbcolormodel.h binary3d.h binsim.h binsim_stdinc.h binsim_version.h constants.h corona3d.h disc3d.h errmsg.h hotspot3d.h image_writer.h jet3d.h keyword.h lobe3d.h mathvec.h movie_maker.h object3d.h starsky.h stream3d.h stream.h transparent_disc3d.h transparent_object3d.h
hotspot3d.o:  hotspot3d.cxx bbcolormodel.h binsim_stdinc.h constants.h hotspot3d.h keyword.h mathvec.h object3d.h stream.h transparent_object3d.h
image_writer.o:  image_writer.cxx binsim_stdinc.h image_writer.h
jet3d.o:  jet3d.cxx bbcolormodel.h binsim_stdinc.h constants.h disc.h jet3d.h keyword.h mathvec.h object3d.h stream.h surface.h transparent_object3d.h
keyword.o:  keyword.cxx binsim_stdinc.h keyword.h stringutil.h
lobe3d.o:  lobe3d.cxx bbcolormodel.h binsim_stdinc.h constants.h keyword.h lobe3d.h mathvec.h object3d.h roche.h surface.h
mathvec.o:  mathvec.cxx binsim_stdinc.h mathvec.h
movie_maker.o:  movie_maker.cxx binsim_stdinc.h errmsg.h keyword.h movie_maker.h
object3d.o:  object3d.cxx binsim_stdinc.h constants.h mathvec.h object3d.h vertex_logger.h
os_binsim.o:  os_binsim.cxx bbcolormodel.h binary3d.h binsim.h binsim_stdinc.h constants.h corona3d.h disc3d.h errmsg.h hotspot3d.h image_writer.h jet3d.h keyword.h lobe3d.h mathvec.h movie_maker.h object3d.h starsky.h stream3d.h stream.h transparent_disc3d.h transparent_object3d.h
roche.o:  roche.cxx binsim_stdinc.h constants.h mathvec.h roche.h surface.h
starsky.o:  starsky.cxx binsim_stdinc.h constants.h errmsg.h keyword.h starsky.h
stream3d.o:  stream3d.cxx binsim_stdinc.h constants.h mathvec.h object3d.h roche.h stream3d.h stream.h surface.h transparent_object3d.h
stream.o:  stream.cxx binsim_stdinc.h constants.h mathvec.h roche.h stream.h surface.h
stringutil.o:  stringutil.cxx binsim_stdinc.h stringutil.h
transparent_disc3d.o:  transparent_disc3d.cxx bbcolormodel.h binsim_stdinc.h constants.h disc.h keyword.h mathvec.h object3d.h roche.h stream.h surface.h transparent_disc3d.h transparent_object3d.h
transparent_object3d.o:  transparent_object3d.cxx binsim_stdinc.h constants.h mathvec.h object3d.h transparent_object3d.h vertex_logger.h
vertex_logger.o:  vertex_logger.cxx binsim_stdinc.h vertex_logger.h
