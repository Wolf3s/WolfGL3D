# Microsoft Developer Studio Project File - Name="WolfGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WolfGL - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WolfGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WolfGL.mak" CFG="WolfGL - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WolfGL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "WolfGL - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/", aaaaaaaa"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WolfGL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Ox /Ot /Og /Op /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib opengl32.lib glu32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib glaux.lib /nologo /subsystem:windows /map /machine:I386 /out:"Release/WolfGL3D.exe"
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "WolfGL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WolfGL___Win32_Debug"
# PROP BASE Intermediate_Dir "WolfGL___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WolfGL___Win32_Debug"
# PROP Intermediate_Dir "WolfGL___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Ox /Ot /Og /Op /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /ZI /Ot /Op /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT BASE MTL /Oicf
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib opengl32.lib glu32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib glaux.lib /nologo /subsystem:windows /map /machine:I386 /out:"Release/WolfGL3D.exe"
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib opengl32.lib glu32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib glaux.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"Release/WolfGL3D.exe"

!ENDIF 

# Begin Target

# Name "WolfGL - Win32 Release"
# Name "WolfGL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Docs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\amp11lib.txt
# End Source File
# Begin Source File

SOURCE=.\Description.txt
# End Source File
# Begin Source File

SOURCE=.\GNU.txt
# End Source File
# Begin Source File

SOURCE=.\MapObjects.txt
# End Source File
# Begin Source File

SOURCE=.\Notes.txt
# End Source File
# Begin Source File

SOURCE=.\Objects.txt
# End Source File
# End Group
# Begin Group "OpenGL"

# PROP Default_Filter ""
# Begin Group "MD3"

# PROP Default_Filter ""
# Begin Group "PNG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\png\array_texture.cpp
# End Source File
# Begin Source File

SOURCE=.\png\data_path.cpp
# End Source File
# Begin Source File

SOURCE=.\png\nv_png.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Md3.cpp
# End Source File
# Begin Source File

SOURCE=.\MD3_Man.cpp
# End Source File
# Begin Source File

SOURCE=.\md3auxilia.cpp
# End Source File
# Begin Source File

SOURCE=.\MD3Shaders.cpp
# End Source File
# End Group
# Begin Group "Map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\automap.c
# End Source File
# Begin Source File

SOURCE=.\Level.c
# End Source File
# Begin Source File

SOURCE=.\map.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\_OpenGL.txt
# End Source File
# Begin Source File

SOURCE=.\FontMan.c
# End Source File
# Begin Source File

SOURCE=.\gl_draw.c
# End Source File
# Begin Source File

SOURCE=.\Menu.c
# End Source File
# Begin Source File

SOURCE=.\OpenGL.c
# End Source File
# Begin Source File

SOURCE=.\raycast.c
# End Source File
# Begin Source File

SOURCE=.\render.c
# End Source File
# Begin Source File

SOURCE=.\sprites.c
# End Source File
# Begin Source File

SOURCE=.\TexMan.c
# End Source File
# Begin Source File

SOURCE=.\Video.c
# End Source File
# Begin Source File

SOURCE=.\wl_text.c
# End Source File
# End Group
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_AI.txt
# End Source File
# Begin Source File

SOURCE=.\actor_ai.c
# End Source File
# Begin Source File

SOURCE=.\Actors.c
# End Source File
# Begin Source File

SOURCE=.\Ai_com.c
# End Source File
# End Group
# Begin Group "OpenAL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_OpenAL.txt
# End Source File
# Begin Source File

SOURCE=.\fmopl.c
# End Source File
# Begin Source File

SOURCE=.\Sound.c
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_Math.txt
# End Source File
# Begin Source File

SOURCE=.\math.c
# End Source File
# Begin Source File

SOURCE=.\Q_fns.c
# End Source File
# End Group
# Begin Group "Input"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_Input.txt
# End Source File
# Begin Source File

SOURCE=.\cl_input.c
# End Source File
# Begin Source File

SOURCE=.\input.c
# End Source File
# Begin Source File

SOURCE=.\keys.c
# End Source File
# End Group
# Begin Group "File Access"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\_File Access.txt"
# End Source File
# Begin Source File

SOURCE=.\Fileio.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\areas.c
# End Source File
# Begin Source File

SOURCE=.\Cl_main.c
# End Source File
# Begin Source File

SOURCE=.\cmd.c
# End Source File
# Begin Source File

SOURCE=.\console.c
# End Source File
# Begin Source File

SOURCE=.\cvar.c
# End Source File
# Begin Source File

SOURCE=.\Doors.c
# End Source File
# Begin Source File

SOURCE=.\Player.c
# End Source File
# Begin Source File

SOURCE=.\powerups.c
# End Source File
# Begin Source File

SOURCE=.\Pwalls.c
# End Source File
# Begin Source File

SOURCE=.\sv_main.c
# End Source File
# Begin Source File

SOURCE=.\sys_win32.c
# End Source File
# Begin Source File

SOURCE=.\version.c
# End Source File
# Begin Source File

SOURCE=.\Window.c
# End Source File
# Begin Source File

SOURCE=.\WolfGL.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "OpenGL Headers"

# PROP Default_Filter ""
# Begin Group "MD3 Headers"

# PROP Default_Filter ""
# Begin Group "PNG Heafers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\png\array_texture.h
# End Source File
# Begin Source File

SOURCE=.\png\data_path.h
# End Source File
# Begin Source File

SOURCE=.\png\Nv_png.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Md3.h
# End Source File
# Begin Source File

SOURCE=.\MD3_Man.h
# End Source File
# Begin Source File

SOURCE=.\md3auxilia.h
# End Source File
# Begin Source File

SOURCE=.\refresh.h
# End Source File
# End Group
# Begin Group "Map Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\automap.h
# End Source File
# Begin Source File

SOURCE=.\level.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\FontMan.h
# End Source File
# Begin Source File

SOURCE=.\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\OpenGL.h
# End Source File
# Begin Source File

SOURCE=.\raycast.h
# End Source File
# Begin Source File

SOURCE=.\render.h
# End Source File
# Begin Source File

SOURCE=.\sprites.h
# End Source File
# Begin Source File

SOURCE=.\TexMan.h
# End Source File
# Begin Source File

SOURCE=.\vgapics.h
# End Source File
# Begin Source File

SOURCE=.\video.h
# End Source File
# Begin Source File

SOURCE=.\wl_text.h
# End Source File
# End Group
# Begin Group "OpenAL Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Audiowl6.h
# End Source File
# Begin Source File

SOURCE=.\fmopl.h
# End Source File
# Begin Source File

SOURCE=.\Sound.h
# End Source File
# End Group
# Begin Group "AI Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Act_stat.h
# End Source File
# Begin Source File

SOURCE=.\actor_ai.h
# End Source File
# Begin Source File

SOURCE=.\Actors.h
# End Source File
# Begin Source File

SOURCE=.\ai_com.h
# End Source File
# End Group
# Begin Group "Math Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\math.h
# End Source File
# Begin Source File

SOURCE=.\Q_fns.h
# End Source File
# End Group
# Begin Group "Input Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\input.h
# End Source File
# Begin Source File

SOURCE=.\keys.h
# End Source File
# End Group
# Begin Group "File Access Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileIO.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\areas.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\cmd.h
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\cvar.h
# End Source File
# Begin Source File

SOURCE=.\Doors.h
# End Source File
# Begin Source File

SOURCE=.\MenuDef.h
# End Source File
# Begin Source File

SOURCE=.\plain_C_is_bullshit.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\powerups.h
# End Source File
# Begin Source File

SOURCE=.\Pwalls.h
# End Source File
# Begin Source File

SOURCE=.\Server.h
# End Source File
# Begin Source File

SOURCE=.\sprt_def.h
# End Source File
# Begin Source File

SOURCE=.\system.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\window.h
# End Source File
# Begin Source File

SOURCE=.\WolfDef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# Begin Source File

SOURCE=.\WolfGL.ico
# End Source File
# End Group
# Begin Group "OldHeaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileIOo.h
# End Source File
# Begin Source File

SOURCE=.\tmp.h
# End Source File
# Begin Source File

SOURCE=.\Wl_2Ddrw.h
# End Source File
# Begin Source File

SOURCE=.\Wl_Agent.h
# End Source File
# Begin Source File

SOURCE=.\Wl_AI.h
# End Source File
# Begin Source File

SOURCE=.\Wl_Game.h
# End Source File
# End Group
# Begin Group "Game Defs"

# PROP Default_Filter "def"
# Begin Source File

SOURCE=.\wl6.def

!IF  "$(CFG)" == "WolfGL - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "WolfGL - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WolfGL.h
# End Source File
# End Group
# Begin Group "Configs"

# PROP Default_Filter "cfg"
# End Group
# End Target
# End Project
