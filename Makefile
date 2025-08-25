# Configure compiler - binary name and flags

# Use GNU GCC (g++). This is now widespread but you may need an alternative
CC = g++

# Define flags for development version
#CFLAGS = -pg -g -Wall -ansi

# Define flags for release version
CFLAGS = -w -O

###############################################################################
# Operating system specific configurations
###############################################################################

# Windows 11 setup under MSYS2 with mingw64. Uses OpenGL built into Windows 
# together with libjpeg and freeglut. X11 libraries are not relevant to this
# configuration

# Library and include paths
#GLLIBDIR = -L"C:\Windows\System32"
#JPEGLIBDIR = 
#X11LIBDIR = 
#GLINCLUDEDIR =
#JPEGINCLUDEDIR = 
#X11INCLUDEDIR = 

# Libraries to link against
#GLLIBS = -lfreeglut -lglu32 -lopengl32
#XLIBS = 
#JPEGLIBS = -ljpeg

###############################################################################

# Ubuntu Linux (tested on 22.04). Template for 'out of the box' Linux build.

# Library and include paths
#GLLIBDIR =
JPEGLIBDIR = 
X11LIBDIR = 
GLINCLUDEDIR =
JPEGINCLUDEDIR = 
X11INCLUDEDIR = 

# Libraries to link against
GLLIBS = -lglut -lGLU -lGL
XLIBS = -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE
JPEGLIBS = -ljpeg

###############################################################################

# MacOS Native Aqua setup
# Uses the built-in MacOS X GLUT framework
# Requires libjpeg-turb

# Library and include paths
#GLLIBDIR = 
#JPEGLIBDIR =  -L/opt/homebrew/opt/jpeg-turbo/lib
#X11LIBDIR = 
#GLINCLUDEDIR = -I/opt/X11/include
#JPEGINCLUDEDIR =  -I/opt/homebrew/opt/jpeg-turbo/include
#X11INCLUDEDIR = 

# Libraries to link against
#GLLIBS = -framework OpenGL -framework GLUT -lobjc
#XLIBS = 
#JPEGLIBS = -ljpeg

###############################################################################

# Mac OS X setup for XFree86, courtesy of Paul Ray (Paul.Ray@nrl.navy.mil)
# Uses XFree86 OpenGL and glut-3.7 from /usr/local (which you must compile)
# Get glut from http://www.opengl.org/developers/documentation/glut/#2
# Hopefully glut will soon be added to fink

# Library and include paths
#GLLIBDIR = -L/usr/local/lib/
#JPEGLIBDIR = -L/sw/lib/
#X11LIBDIR = -L/usr/X11R6/lib/
#GLINCLUDEDIR = -I/usr/local/include/
#JPEGINCLUDEDIR = -I/sw/include/
#X11INCLUDEDIR = -I/usr/X11R6/include/

# Libraries to link against
#GLLIBS = -framework OpenGL -framework GLUT -lobjc
#XLIBS = 
#JPEGLIBS = -ljpeg

###############################################################################

# Generic Linux setup with Mesa compiled from source [Old]

# Library and include paths
#GLLIBDIR = -L/usr/local/mesa/lib
#JPEGLIBDIR = 
#X11LIBDIR = -L/usr/X11R6/lib
#GLINCLUDEDIR = -I/usr/local/mesa/include
#JPEGINCLUDEDIR = 
#X11INCLUDEDIR = -I/usr/X11R6/include

# Libraries to link against
#GLLIBS = -lglut -lGLU -lGL
#XLIBS = -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE
#JPEGLIBS = -ljpeg

###############################################################################

# Standard Redhat 6.2/7.0/7.1/7.2 setup [Now very old, I haven't used Redhat in a long time]

# Library and include paths
#GLLIBDIR =
#JPEGLIBDIR = 
#X11LIBDIR = -L/usr/X11R6/lib
#GLINCLUDEDIR =
#JPEGINCLUDEDIR = 
#X11INCLUDEDIR = -I/usr/X11R6/include

# Libraries to link against
#GLLIBS = -lglut -lGLU -lGL
#XLIBS = -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE
#JPEGLIBS = -ljpeg

###############################################################################

# Example Solaris setup; will need customisation since you will 
# likely have to install Mesa and/or libjpeg yourself [Very old]

# Library and include paths
#GLLIBDIR = -L/usr/local/lib
#JPEGLIBDIR =
#X11LIBDIR = 
#GLINCLUDEDIR = -I/usr/local/include
#JPEGINCLUDEDIR =
#X11INCLUDEDIR =

# Libraries to link against
#GLLIBS = -lglut -lGLU -lGL
#XLIBS = -lX11 -lXext -lXmu -lXt -lXi -lSM -lICE
#JPEGLIBS = -ljpeg

###############################################################################
# OSMesa linkage

# OpenGL implementation other than Mesa or
# Mesa 3.4.2 or earlier or
# Mesa without OSMesa library 
OSMESALIB = 
OSMESAFLAGS = 

# Standard OSMesa for Mesa 3.5 or later
#OSMESALIB = -lOSMesa
#OSMESAFLAGS = 

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
