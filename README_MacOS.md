## Building Binsim on Mac OS

These instructions were written in 2025 for an install on Mac OS Sequoia 15.3.1. 
They are adapted from one of three options in an earlier Mac port by Paul Ray 
(Paul.Ray@nrl.navy.mil).

They are written with the Mac OS X System OpenGL Framework in mind which includes 
GLUT. You still edit the sample.par file and run binsim from the command line, 
but a native (Aqua) window will pop up and render the image using hardware-accelerated 
OpenGL.

### Prerequisites

1. You must have the Mac OS X Developer Tools (XCode) installed. You can download XCode
   from the App Store.

2. You will need some version of libjpeg. The Makefile is written assuming you use libjpeg-turbo 
(https://libjpeg-turbo.org/) and install this with Homebrew
(brew install libjpeg-turbo). If you use another installer like fink or compile from source
the headers and libraries will be in a different place and you will need to modify the 
Makefile.

### Build

Edit the Makefile and comment out all the sections dealing with operating systems except for
"MacOS Native Aqua". Then just type "make" and it should build. Run it with 
"./binsim sample.par" and it should work fine.

Depending on your OS version GLUT apparently could be in either /opt/X11/include/GL or /opt/X11/include/GLUT.

## Alternative Build Instructions (OLD)

These instructions were provided in the 2000's and will likely need modification and updates if
wish to go this path. Please let me know if you succeed!

### ProjectBuilder

BinSim can also be compiled using ProjectBuilder.  These directions
are from memory, but should get you started.  It will also use the
native Mac OS X windowing system (Aqua)

* Install libjpeg using fink as above.

* Start ProjectBuilder and start a New Project.  Select "C++
Tool" as the Project type.

* Move the .cxx and .h files into the project directory (all EXCEPT
os_binsim.cxx)

* Drag the files into Project Builder.  I put them all in the "Source"
group.

* Drag GLUT.framework into the "External Frameworks" group.  It can be
found in /System/Library/Frameworks

* Go to "Edit Active Target" under the Project menu.
In the Files & Build Phases pane, verify the /usr/lib/libstdc++.a and
/System/Library/Frameworks/GLUT.framework show up under "Frameworks &
Libraries" 
In the "Build Settings" pane, add these to Other Linker Flags
-lGL -lGLU -lobjc -ljpeg
Add "/sw/include" to Search Paths / Headers
Add "/sw/lib" to Search Paths / Libraries

* Then just click "Build" to build it.

* You should still run it from the command line.  When I run it
within ProjectBuilder, it can't find sample.par.  Probably I am doing
something wrong.

### XFree86

Finally, BinSim can also be compiled against XFree86 or another X11
distribution for OS X (such as Tenon's XTools or eXodus).

* You must install XFree86 to use this version!
  This version crashes some earlier releases of XFree86 because of the 
  NaNs that get sent to OpenGL from binsim.  This has been fixed in
  later releases by Torrey Lyons (see http://www.mrcla.com/XonX)

* You also need to compile GLUT-3.7 and put the includes in /usr/local/include
and the libglut.a into /usr/local/lib. Get glut from
http://www.opengl.org/developers/documentation/glut/#2
Hopefully, glut will soon be added as a fink package.

* You still need libjpeg from fink but this version has no dependence


