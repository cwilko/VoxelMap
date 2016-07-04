# Microsoft Developer Studio Project File - Name="Work" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Work - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Work.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Work.mak" CFG="Work - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Work - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Work - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Work___Win32_Release"
# PROP BASE Intermediate_Dir "Work___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Work___Win32_Release"
# PROP Intermediate_Dir "Work___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"Work___Win32_Release/Work2.exe"

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib ddraw.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Work - Win32 Release"
# Name "Work - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DirectX.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FirstTest.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GameMain.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\init.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\InitWindows.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ScreenSave.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\version2.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\version3.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\version4.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\version5.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\version6.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\version7.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\version8.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\version9.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Voxelwn1.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="..\..\..\Windows\Desktop\Old Files\Temp\Voxelwn2.cpp"

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Winmain.cpp

!IF  "$(CFG)" == "Work - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Work - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Winmain2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GameMain.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Work___Win32_Release\chrissy.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\Heightc1.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\Heightd1.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\Heightd2.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\lassen.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\texture2.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\texture3.bmp
# End Source File
# Begin Source File

SOURCE=.\Work___Win32_Release\try.bmp
# End Source File
# End Group
# End Target
# End Project
