# BinSim 1.01, Robert I. Hynes, 8 September 2025

### OpenGL-based code for visualizing interacting binary stars

This program is designed to produce pretty pictures of interacting
binaries (cataclysmic variables, X-ray binaries, etc).  In many places
the science has been fudged or ignored, so it is not suitable for
modelling light curves or spectra.  The code uses the OpenGL API to do
the 3D rendering.  As such there are some limitations since this
approach is better suited to rendering polygons (e.g. surface elements
of a star or disc) than to volumetric rendering (e.g. an accretion
stream or disc wind).  Consequently the treatment of optically thin
objects is rather poor.  At present the code will render the mass
donor star, an axisymmetric disc (no superhumps, warps or spirals
yet), the accretion stream and hotspot and a 'corona', which may
represent an accretion disc corona, a disc wind, some other kind of
uncollimated outflow or even a white dwarf!

*This code has been dormant for a long time, but has remained in use. In 
migrating it to GitHub, I've worked on updating the README files and 
checking usability on different platforms. This version is currently known 
to work on Ubuntu 22.04, MacOS Sequoia 15.5, and Windows 11.*

## Usage


To run the program you need a parameter file.  A sample is included in
this distribution.  Just type: 

% ./binsim sample.par

or 

% ./osbinsim sample.par

All options are controlled from the parameter file.

## Output formats

BinSim can produce either JPEG or PPM images.  The type produced is
determined by the extension of the Image_File parameter: for JPEG
images, .jpg, .JPG, .jpeg and .JPEG are recognised.  For PPM then
either .ppm or .PPM are fine.  Any other extensions will be treated as
an error.  If Anim is turned on then only PPM mode as used as JPEG
does not work well with mpeg_encode.

If you do not have the libjpeg library then you can compile with the
-DNOJPEG option and remove the -ljpeg from the Makefile.  Then all
JPEG support is disabled.  This is the default for Windows as JPEG
writing does not work there.

## Off-screen rendering

BinSim can also be built to render off-screen, so that it can be run
from a remote session to produce an image, etc.  This takes
advantage of Mesa's off-screen option (OSMesa) so will probably not
work with a commercial OpenGL library.  Consequently, this is not
built by default.  If you are using Mesa and want to do this then you
can do 'make osbinsim' to make the off-screen binary (osbinsim).  The
usage is the same as for binsim.

## 16 bits per channel

BinSim can also take advantage of the 16 bits per channel (64bpp)
OSMesa mode in Mesa 3.5.  This is not built by default and offers
little if any benefit in the current version of BinSim.  Getting it to
work is rather awkward, so there really isn't much point at present.
See the INSTALL file for details of how to get this working.

## Animation

If the Anim keyword is true then a series of images will be generated
from Low_Phase to High_Phase with steps of Delta_Phase.  If Save is
also true then each image will be saved to Anim_Rootdir (which should
have a lot a free space) and an MPEG movie will also be created.  This
depends on having mpeg_encode installed.  See the INSTALL file for how
to get this.  If Save is not defined then the sequence will only be
rendered to the screen.  

## Tips

As well as lobe overflowing stars, by setting Lobe_Fill to less than
1.0, Disc_Eff_Thick negative and turning off the stream and disc it is
possible to produce a detached system containing a distorted,
irradiated star.  A white dwarf + M dwarf detached binary can be
produced by doing this and making one of the coronae very small with a
very high opacity to represent the white dwarf.

## Parameters

Nb) Old version of renamed parameters continue to be supported via
internal keyword translations.  See notes below.

### New parameters in v0.9

Show_Lobe1, Lobe1_N_Steps, Lobe1_Fill, Lobe1_T_Pole, Lobe1_T_Min,
Lobe1_Granulation, Lobe1_Granulation_Period, Luminosity2

### Renamed parameters in v0.9

Show_Lobe, and all Lobe_* replaced by Show_Lobe2 and Lobe2_*

Luminosity replaced by Luminosity1

### New parameters in v0.8.1

None

### New parameters in v0.8

Disc_R_in, Show_Stellar_Wind, Stellar_Wind_Rad, Stellar_Wind_Red, 
Stellar_Wind_Green, Stellar_Wind_Blue, Stellar_Wind_Opacity, 
Stellar_Wind_Exponent, Show_Thin_Disc, Thin_Disc_Nsteps, Thin_Disc_Rad,
Thin_Disc_R_in, Thin_Disc_Geom_Thick, Thin_Disc_Beta, Thin_Disc_Red, 
Thin_Disc_Green, Thin_Disc_Blue, Thin_Disc_Opacity, Thin_Disc_N_Flare, 
Thin_Disc_Flare_Length, Show_Jet, Jet_Opening_Angle, Jet_Upper_Red, 
Jet_Upper_Green, Jet_Upper_Blue, Jet_Lower_Red, Jet_Lower_Green, 
Jet_Lower_Blue, Jet_Opacity, Jet_Exponent, Jet_Inclination, Jet_Rotation,
Vertex_Log 

### New parameters in v0.7.3

HighQuality_AA

### New parameters in v0.7.2

None

### New parameters in v0.7.1

None

### New parameters in v0.7

Anim, Low_Phase, High_Phase, Delta_Phase, Anim_Root, MPEG_File, 
MPEG_Pattern, MPEG_IQScale, MPEG_PQScale, MPEG_BQScale, 
Lobe_Granulation_Period, Hot_Spot_Timescale

### New parameters in v0.6.1

Image_File

## Notes

All boolean (True/False) parameters can also take values of On/Off or
Yes/No.

The HighQuality flag turns on high quality output (antialiasing and
nicer stars).  If the antialiasing causes problems (apparently it does
on Mac OS X) then it can be disabled by setting HighQuality_AA =
False.  If this parameter is not given it defaults to true.

If Vertex_Log is true then a file called vertices.log will be created
containing the coordinates and colours of every vertex in the model.
This will be very large, 30Mb or more is likely!  This is intended
only for debugging and should normally be disabled.  If it is not
specified it will silently default to false.  Don't enable this is
Anim is true - the log file will be HUGE!

Anim_Root should specify the directory (no trailing /) for animation
images.  This should be a directory with a lot of free space.  The
full filename will be <Anim_Root>/binsim_tmp.0001.jpg and so on.

The MPEG parameters sound rather obscure.  You can read about them in
the mpeg_encode manual that should have come with your distribution of
the encoder.  For an easy life, try two options:
  1) Best quality, large files:  Use Pattern = I and IQScale = 1
  2) Compression at the cost of quality: Pattern = IBBPBBPBBPBBPBBPB 
     (please don't ask why; the manual recommends it!) and increase
     IQScale, PQScale and BQScale until you get a satisfactory compromise.

Brightness and Contrast only effect optically thick surfaces (the disc
and companion star).  This is a limitation of the code; optically thin
materials are poorly modelled.  Future voxel based optically thin
components will improve on this situation.

Star_Colours determines the range of colours present in a background
starfield.  Setting it to 0.0 will make all stars white (ie shades of
gray according to brightness).  1.0 or more will give a very
colourful, if unrealistic background.  I find 0.5 gives some range
without being overstated.

It is now possible to include two stars.  The companion star is
specified by parameters Lobe2*, and irradiated by luminosity
Luminosity1.  It is shielded by the disk if present.  Optionally a
primary star can be added, with parameters Lobe1* and irradiated by
Luminosity2 (i.e. the companion luminosity).

Lobe_Fill can control how much of the Roche lobe is filled by the
companion star.  1.0 is lobe filling; lower values can give distorted
stars that do not fill their lobes.

T_Min can be used to prevent black L1 points.  Formally, the usual
gravity darkening formulae results in the temperature tending to zero
at the L1 point, which then appears black.  I'm sure this is not
physical.  Setting T_Min to some sensible value, e.g. 0.85, prevents
this.

Granulation_Amplitude should be set to zero for hot stars with
radiative envelopes.

Granulation_Period controls the time taken (as a fraction of the
orbital period) for each point on the companion to complete one minmax
cycle.  This is to create a convective bubbling effect.  If you don't
like the effect set this very large.

Disc_Eff_Thick and Disc_Geom_Thick can be set to different values to
allow more shielding of the mass donor than the disc rim alone could
produce.  This could model absorption of X-rays by the disc atmosphere
or a bulge in the inner disc.

Disc_Rad is definined as the fraction of Eggleton's effective lobe
radius, defined in Eggleton, P. P., 1983, ApJ, 268, 368.  If this is
set too small the stream may not behave well, as it is programmed to
continue until it reaches the disc rim; for small disc radii this will
never happen!

Disc_Temp_Grad is defined by T propto R^grad.  Standard values
are -3/4 for a Shakura-Sunyaev viscously heated disc, -3/7 for an
irradiated disc.

Disc_Beta is the flaring exponent of the disc defined by H/R propto
R^beta.  Standard values are 9/8 for a viscously heated disc or 9/7
for an irradiated disc, although how physical these are is debatable!

Stream_Max_Thick defines a maximum stream thickness.  This prevents
odd looking thick streams for long period systems.

Stream_Open_Angle sets the apparent opening angle of the stream as
seen from the centre of the companion star, in degrees. 10 degrees
looks okay for a typical mass ratio of 0.3.  This needs to be
decreased for large mass ratios (>1) and maybe increased for very
small ones.

Stream_Opacity, Hot_Spot_Opacity and Corona_Opacity are not defined on
any absolute scale, they just give some control over the optical
thicknesses.

Hot_Spot_Timescale controls the timescale (as a fraction of the
orbital period) of flickering at the stream impact point.

Hot_Temp controls the temperature of a heated area of the disc
downstream of the stream impact point.  Set this low to turn thiseffect off.

Corona1_Exponent and Corona2_Exponent control the radial density
opacity of the coronae - 0.0 gives a uniformly shaded circle, without
even fading at the edges due to lower optical depths.  Positive values
will give a centrally condensed corona.  Negative values can produce a
shell-like effect.  This was not a designed effect, and isn't very
satisfactory.

The Stellar_Wind component is identical to a Corona except it is
centred on the companion star.

## Keyword Translations

A new internal module was added in v0.9 which can translate alternate
versions of keywords.  This is intended for:

1. Maintaining backward compatibility.  Where possible, old versions
of parameters names (e.g. Show_Lobe) will be translated to their new
counterparts (Show_Lobe1 in this case) so that old parameter files
will continue to work.

2. Allowing internationalisation of BinSim.  You could write a
complete set of translations from another language, subject to
restrictions which might be imposed by the character set.  If you do
this I would be grateful for a copy to distribute with the package.

3. Allowing other variations in specific keywords, e.g. Disc vs. Disk
or Q vs. Mass_Ratio.

4. Any other personal changes you wish to make - e.g. Corona1_* ->
ADAF_*

It is implemented via a function in the keyword_translator.cxx file.
This is isolated so that internationalised versions of this file can
be distributed.  Simply add as many 

list->translate_keyword("Old_Keyword", "New_Keyword"); 

lines as are necessary.  Note that later translations override earlier
ones.

## Bugs and 'issues'

### Small triangles

If the grid sizes (N_STEPS parameters) are too fine, or the scale is
too small, or the image size is too small, then triangles can occupy a
tiny fraction of a pixel.  Mesa 3.4.2 or earlier software rendering
does not always handle this case well and stars or discs may have
holes or black spots sprinkled over them, generally at the star poles
or disc centre.  This problem was fixed by Brian Paul on 12/06/01 and
incorporated into Mesa from version 3.5 onwards.  If you encounter
this problem either decrease the grid size, increase the image size or
update Mesa to version 3.5 or later.

### Saving only works with software rendering

The code to save an image does not work with Glide fullscreen mode -
the image is blank.  This problem may be common to all hardware
implementations but I have not been able to test any others.

### Crash while encoding MPEGs

In theory mpeg_encode can work with JPEG images, but it often crashes
with segmentation faults.  If I find a solution to this I'll implement
it, otherwise MPEGs are constructed via (large) PPM files.

### Problems with antialiasing on Mac OS X (and others???)

Antialiasing does not work properly under Mac OS X.  The problem may
occur on other platforms as well.  The effect is that colours become
banded, as if the image is produced with a 16 bit color display, or
less.  I don't know what causes it but it may be a rounding problem in
accumulating multiple samples.  From 0.7.3 it is possible to turn off
antialising without disabling high quality stars.  This is done by
setting HighQuality_AA = False in the parameter file.  

### Saving images under Windows

I have major problems with saving images when running on Windows.
This is not fully functional in this version.  PPM output does work, I
think, although it required a kludge which may have other side effects
to get the colours right.  JPEG output crashes.  I don't know why, but
the crash appears to occur within a library call rather than in BinSim
proper, and even a simple image writing application reproduces it, so
it seems to be a libjpeg bug.  For this release, JPEG output has been
disabled on Windows, so PPM is the only option.

### Generation of NaNs

On some platforms (Mac OS X, but not Unix/Linux or Windows?) BinSim
can try to pass NaNs as part of vertex values.  The effect of this is
undefined; you may get corrupted images, you may have the program (or
the X Server!)  crash.  This is being investigated.  If you get odd
looking features in the image, or a crash then turn on vertex logging
(Vertex_Log = True) and look at the resulting vertices.log.  There
should be no NaN or Inf values here.  If there are this is a serious
bug; please tell me.  Ideally send me the parameter file, the
offending lines grepped from vertices.log (not the whole file!) and
let me know what operating system and compiler you have.

## Finally ...

Thanks to many people who have provided me with encouragement to
develop this code.  Also to Brian Paul and other contributors to Mesa,
which is so central to this program, both for writing the code and
responding to problems so efficiently.  Libjpeg is produced by the
Independent JPEG Group.  Thanks to Paul Ray and Torrey Lyons for
testing on Mac OS X.

While this code has been in circulation for a while, there may still be some bugs.
If you find something that doesn't work the way it should, or looks
odd then let me know.  Equally, if you modify the code and you think
others would benefit from the improvements I'll consider merging the
changes into the main code, with suitable acknowledgements.

Whatever, if you find the code useful or interesting please let me
know so I know how widely it is being used and can keep you informed
of developments.  I'd also like to hear from people who have success
(or failure) getting the program to work on other systems.

--
Rob Hynes, Louisiana State University, rhynes@lsu.edu
