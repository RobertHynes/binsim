## Version 1.0, 21 August 2025 ##

-After 20+ years unchanged I think it's time to call this version 1.0!

-20 year update to makefiles, library references, etc

-Migration to GitHub

## Version 0.9, 12 March 2003 ##

 -Added a draft manual in the doc subdirectory
 
 -Added tutorial parameter files (to accompany the manual) in the tutorial 
  subdirectory
  
 -Added sample parameter files in the examples subdirectory
 
 -Added keyword translation capability
 
 -Renamed Lobe parameters to Lobe2, and added the option of having Lobe1.
 
 -Renamed primary and companion irradiating luminosities to Luminosity1 and 
  Luminosity2 respectively
	
## Version 0.8.1, 21 May 2002 ##

 -Reenable the Lobe_T_Min functionality which was accidentally
  disabled in version 0.8.
	
## Version 0.8, 20 December 2001 ##

 -Tried to fix NaN problems on Mac OS X.
 
 -All JPEG support can be removed with -DNOJPEG to allow compilation if
  libjpeg is not installed.
  
 -Output file format (JPEG or PPM) can now be selected by the extension 
  of the filename specified.
  
 -Added option to log all vertexes to a text file for debugging
 
 -Various code tweaks to fix incompatibilities with Visual C++.  BinSim 
  will now compile and run on Windows!
  
 -Renamed .cc files to .cxx to be recognised by both Visual C++ and GCC.
 
 -Added a jet component
 
 -Added an optically thin disc component
 
 -Added a stellar wind component to allow hot stars in high mass X-ray 
  binaries.  Works identically to Corona components. 
  
 -Added Disc_R_in parameter to allow discs with holes in the middle.
	
 ## Version 0.7.3, 11 October 2001 ##

 -Added HighQuality_AA option to allow selective disabling of antialiasing 
  option in high quality mode
  
 -Added Mac OS X options to the Makefile and a README.MacOSX file, 
  courtesy of Paul Ray (Paul.Ray@nrl.navy.mil)
  
 -Simplified dependencies in Makefile removing many redundant entries.

## Version 0.7.2, 31 August 2001 ##

 -Animations (to MPEGs) now work in off-screen mode.  This was omitted from 
  earlier versions (=forgotten!)
	
## Version 0.7.1, 30 July 2001 ##

 -Fixed black spot on pole of companion star in v0.7
 
 -Fixed phase_index bug in v0.7 which can lead to segmentation fault

## Version 0.7, 27 July 2001 ##
	
 -Miscellaneous bug fixes.
	
 -The documentation has been completely reorganised along standard lines.
 
 -PPM output has been reimplemented using Joerg Schmalzl's PPM code from 
  the osdemo.c file in the Mesa distribution.  At the moment this is only
  used in producing MPEGs and is not an option for stills.
  
 -Movies are now supported again, both in real time and as MPEG encoding.  
  The companion star, stream, hot spot and disc all have animation.
  
 -Off screen rendering now supported, via the osbinsim binary.  This includes 
  the new 64bpp option in Mesa 3.5 if available.

## Version 0.6.1, 27 June 2001 ##

 -Some documentation changes and corrections and several minor bug fixes.
 
 -Progress is now logged to the terminal.
 
 -Some parameters have sensible defaults so that the code will run without 
  them, and more thorough value checking is performed of all parameters.
  
 -The output image filename can now be specified by the Image_File keyword.
 
 -SAMPLES parameter handling corrected.  Allowed values are 2
  (2x2 subsamples) or 4 (4x4).
  
 -Corona bug corrected.  In 0.6, both coronae were accidentally
  controlled by Show_Corona1.
  
 -Images will now only be saved once the first time the binary is
  drawn, not at every redraw.
  
 -Should now build under GCC 3.0; version 0.6 will not.

## Version 0.6, 13 June 2001 ##

Mainly this is an attempt to produce a test release based on a subset
of features previously used.  As such no major new features or
components are present.  A number of improvements have been made to
the way some objects are rendered compared to previously released
images.  These include:

 -In high-quality mode background stars are now drawn as fuzzy
  circles.  They now look round rather than squarish as they did
  before.  The circle size also scales with brightness, so the
  apparent difference between bright and faint stars is more dramatic.

 -Granulation on the companion star and structure in the hotspot now
  varies on a scale of roughly fixed solid angle.  Previously it
  varied on a fixed azimuth scale resulting in squashing towards the
  poles with structure appearing vertically streaked.
 
 -Stream now merges sensibly onto the companion star surface and the
  amount of the surface over which it blends can be controlled.

