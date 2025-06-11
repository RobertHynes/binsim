!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\
INTDIR=.\
# Begin Custom Macros
OutDir=.\
# End Custom Macros

ALL : "binsim.exe"

CLEAN :
	-@erase "bbcolormodel.obj"
	-@erase "binary3d.obj"
	-@erase "binsim.obj"
	-@erase "corona3d.obj"
	-@erase "disc.obj"
	-@erase "disc3d.obj"
	-@erase "gl_binsim.obj"
	-@erase "hotspot3d.obj"
	-@erase "image_writer.obj"
	-@erase "jet3d.obj"
	-@erase "keyword.obj"
	-@erase "lobe3d.obj"
	-@erase "mathvec.obj"
	-@erase "movie_maker.obj"
	-@erase "object3d.obj"
	-@erase "roche.obj"
	-@erase "starsky.obj"
	-@erase "stream.obj"
	-@erase "stream3d.obj"
	-@erase "stringutil.obj"
	-@erase "transparent_disc3d.obj"
	-@erase "transparent_object3d.obj"
	-@erase "vertex_logger.obj"
	-@erase "vc60.idb"
	-@erase "binsim.exe"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "NOJPEG" /Fp"binsim.pch" /YX /FD /c 

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"binsim.bsc" 
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libjpeg.lib /nologo /subsystem:console /incremental:no /pdb:"binsim.pdb" /machine:I386 /out:"binsim.exe" 
LINK32_OBJS= \
	"bbcolormodel.obj" \
	"binary3d.obj" \
	"binsim.obj" \
	"corona3d.obj" \
	"disc.obj" \
	"disc3d.obj" \
	"gl_binsim.obj" \
	"hotspot3d.obj" \
	"image_writer.obj" \
	"jet3d.obj" \
	"keyword.obj" \
	"lobe3d.obj" \
	"mathvec.obj" \
	"movie_maker.obj" \
	"object3d.obj" \
	"roche.obj" \
	"starsky.obj" \
	"stream.obj" \
	"stream3d.obj" \
	"stringutil.obj" \
	"transparent_disc3d.obj" \
	"transparent_object3d.obj" \
	"vertex_logger.obj"

"binsim.exe" : $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE=bbcolormodel.cxx
"bbcolormodel.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=binary3d.cxx
"binary3d.obj" : $(SOURCE) 
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=binsim.cxx
"binsim.obj" : $(SOURCE) 
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=corona3d.cxx
"corona3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=disc.cxx
"disc.obj" : $(SOURCE) 
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=disc3d.cxx
"disc3d.obj" : $(SOURCE) 
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=gl_binsim.cxx
"gl_binsim.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=hotspot3d.cxx
"hotspot3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=image_writer.cxx
"image_writer.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=jet3d.cxx
"jet3d.obj" : $(SOURCE) 
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=keyword.cxx
"keyword.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=lobe3d.cxx
"lobe3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=mathvec.cxx
"mathvec.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=movie_maker.cxx
"movie_maker.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=object3d.cxx
"object3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=roche.cxx
"roche.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=starsky.cxx
"starsky.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=stream.cxx
"stream.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=stream3d.cxx
"stream3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=stringutil.cxx
"stringutil.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=transparent_disc3d.cxx
"transparent_disc3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=transparent_object3d.cxx
"transparent_object3d.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

SOURCE=vertex_logger.cxx
"vertex_logger.obj" : $(SOURCE)
	$(CPP) $(CPP_PROJ) $(SOURCE)

