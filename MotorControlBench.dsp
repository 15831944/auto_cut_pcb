# Microsoft Developer Studio Project File - Name="MotorControlBench" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MotorControlBench - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MotorControlBench.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MotorControlBench.mak" CFG="MotorControlBench - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MotorControlBench - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MotorControlBench - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MotorControlBench - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Gz /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "MotorControlBench - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Gz /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GT400.lib PEGRP32C.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MotorControlBench - Win32 Release"
# Name "MotorControlBench - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ACMotorCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\BufPtr.cpp
# End Source File
# Begin Source File

SOURCE=.\CardInitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\CIniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CNC_c.cpp
# End Source File
# Begin Source File

SOURCE=.\CNCData.cpp
# End Source File
# Begin Source File

SOURCE=.\DCMotrCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\digitaldisp.cpp
# End Source File
# Begin Source File

SOURCE=.\DY_MATH.CPP
# End Source File
# Begin Source File

SOURCE=.\FixedSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FunctionBar.cpp
# End Source File
# Begin Source File

SOURCE=.\GCodeCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\GCodeDrawView.cpp
# End Source File
# Begin Source File

SOURCE=.\GCodeFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\GTCARD.cpp
# End Source File
# Begin Source File

SOURCE=.\InterpExpCtrlWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\InterpExpDrawView.cpp
# End Source File
# Begin Source File

SOURCE=.\InterpExpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Interpolation.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ModeSelTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MotorControlBench.cpp
# End Source File
# Begin Source File

SOURCE=.\MotorControlBench.rc
# End Source File
# Begin Source File

SOURCE=.\MotorControlBenchDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\MotorControlBenchView.cpp
# End Source File
# Begin Source File

SOURCE=.\PageBasePara.cpp
# End Source File
# Begin Source File

SOURCE=.\PageFeedSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\PageGearMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PageJogOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\PagePlaneSel.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSysPara.cpp
# End Source File
# Begin Source File

SOURCE=.\PageTMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PageVelMode.cpp
# End Source File
# Begin Source File

SOURCE=.\PageZoomOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\ParaSetDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PasWordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Position.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TabSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewSelTabCtrl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ACMotorCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\BufPtr.h
# End Source File
# Begin Source File

SOURCE=.\CardInitDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChartStatic.h
# End Source File
# Begin Source File

SOURCE=.\CIniFile.h
# End Source File
# Begin Source File

SOURCE=.\Cnc_api.h
# End Source File
# Begin Source File

SOURCE=.\CNC_c.h
# End Source File
# Begin Source File

SOURCE=.\CNCData.h
# End Source File
# Begin Source File

SOURCE=.\DCMotrCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\DebugDlg.h
# End Source File
# Begin Source File

SOURCE=.\digitaldisp.h
# End Source File
# Begin Source File

SOURCE=.\DY_MATH.H
# End Source File
# Begin Source File

SOURCE=.\FixedSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\FunctionBar.h
# End Source File
# Begin Source File

SOURCE=.\GCodeCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\GCodeDrawView.h
# End Source File
# Begin Source File

SOURCE=.\GCodeFrame.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=..\..\GMD_IMPROVE\GMD_Demo_SOR_1.0\GT400.H
# End Source File
# Begin Source File

SOURCE=.\GTCARD.h
# End Source File
# Begin Source File

SOURCE=.\GTCNCLib.h
# End Source File
# Begin Source File

SOURCE=.\InterpExpCtrlWnd.h
# End Source File
# Begin Source File

SOURCE=.\InterpExpDrawView.h
# End Source File
# Begin Source File

SOURCE=.\InterpExpFrame.h
# End Source File
# Begin Source File

SOURCE=.\Interpolation.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ModeSelTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MotorControlBench.h
# End Source File
# Begin Source File

SOURCE=.\MotorControlBenchDoc.h
# End Source File
# Begin Source File

SOURCE=.\MotorControlBenchView.h
# End Source File
# Begin Source File

SOURCE=.\MyDataType.h
# End Source File
# Begin Source File

SOURCE=.\PageBasePara.h
# End Source File
# Begin Source File

SOURCE=.\PageFeedSetting.h
# End Source File
# Begin Source File

SOURCE=.\PageGearMode.h
# End Source File
# Begin Source File

SOURCE=.\PageJogOperate.h
# End Source File
# Begin Source File

SOURCE=.\PagePlaneSel.h
# End Source File
# Begin Source File

SOURCE=.\PageSMode.h
# End Source File
# Begin Source File

SOURCE=.\PageSysPara.h
# End Source File
# Begin Source File

SOURCE=.\PageTMode.h
# End Source File
# Begin Source File

SOURCE=.\PageVelMode.h
# End Source File
# Begin Source File

SOURCE=.\PageZoomOperate.h
# End Source File
# Begin Source File

SOURCE=.\ParaSetDlg.h
# End Source File
# Begin Source File

SOURCE=.\PasWordDlg.h
# End Source File
# Begin Source File

SOURCE=.\Pegrpapi.h
# End Source File
# Begin Source File

SOURCE=.\Position.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TabSheet.h
# End Source File
# Begin Source File

SOURCE=.\ViewSelTabCtrl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AddRec.ICO
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CONTROL.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Degree.ico
# End Source File
# Begin Source File

SOURCE=.\res\gcode.ico
# End Source File
# Begin Source File

SOURCE=.\res\GO.ICO
# End Source File
# Begin Source File

SOURCE=.\res\GOOGOL.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Green2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Hand_X_Right.ico
# End Source File
# Begin Source File

SOURCE=".\res\Headband(500x354).bmp"
# End Source File
# Begin Source File

SOURCE=.\res\ICON_OK.ICO
# End Source File
# Begin Source File

SOURCE=".\res\interface 01.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\interface 02.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\MISC01.ICO
# End Source File
# Begin Source File

SOURCE=.\res\MotorControlBench.ico
# End Source File
# Begin Source File

SOURCE=.\res\MotorControlBench.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MotorControlBenchDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\OOFL.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Red2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TOOLS.ICO
# End Source File
# Begin Source File

SOURCE=.\res\VideoSourceSet.ICO
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\G.AVI
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\GT400.LIB
# End Source File
# Begin Source File

SOURCE=.\PEGRP32C.lib
# End Source File
# End Target
# End Project
# Section MotorControlBench : {C216ADCA-ED74-4E58-A102-458379D8CBAC}
# 	2:12:TabSheet.cpp:TabSheet.cpp
# 	2:10:TabSheet.h:TabSheet.h
# 	2:19:Application Include:motorcontrolbench.h
# 	2:16:CLASS: CTabSheet:CTabSheet
# End Section
# Section MotorControlBench : {458C62A4-2009-4235-A6EC-419C88DB3D00}
# 	1:17:IDD_T_MODE_DIALOG:111
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_T_MODE_DIALOG:IDD_T_MODE_DIALOG
# 	2:10:TMODEDLG.h:TMODEDLG.h
# 	2:15:CLASS: TMODEDLG:TMODEDLG
# 	2:12:TMODEDLG.cpp:TMODEDLG.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:motorcontrolbench.h
# End Section
# Section MotorControlBench : {AD630327-F224-11D6-8120-0050FC78FDEC}
# 	2:21:DefaultSinkHeaderFile:digitaldisp.h
# 	2:16:DefaultSinkClass:CDigitalDisp
# End Section
# Section MotorControlBench : {9FDD39AC-BD0C-49B6-8063-2937B1545569}
# 	1:20:IDD_GEAR_MODE_DIALOG:113
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:20:IDD_GEAR_MODE_DIALOG:IDD_GEAR_MODE_DIALOG
# 	2:15:GEARMODEDLG.cpp:GEARMODEDLG.cpp
# 	2:13:GEARMODEDLG.h:GEARMODEDLG.h
# 	2:18:CLASS: GEARMODEDLG:GEARMODEDLG
# 	2:19:Application Include:motorcontrolbench.h
# End Section
# Section MotorControlBench : {270F23C2-7126-452A-8993-6CF4646AD778}
# 	2:21:CLASS: CInterpolation:CInterpolation
# 	2:19:Application Include:motorcontrolbench.h
# 	2:17:Interpolation.cpp:Interpolation.cpp
# 	2:15:Interpolation.h:Interpolation.h
# End Section
# Section MotorControlBench : {E96BB00E-9FB2-49D7-9F51-2C40E4F6E626}
# 	1:19:IDD_VEL_MODE_DIALOG:110
# 	2:16:Resource Include:resource.h
# 	2:14:VELMODEDLG.cpp:VELMODEDLG.cpp
# 	2:19:IDD_VEL_MODE_DIALOG:IDD_VEL_MODE_DIALOG
# 	2:10:ENUM: enum:enum
# 	2:12:VELMODEDLG.h:VELMODEDLG.h
# 	2:17:CLASS: VELMODEDLG:VELMODEDLG
# 	2:19:Application Include:motorcontrolbench.h
# End Section
# Section MotorControlBench : {EFC08EE6-4920-4A32-8CB6-177DD2146630}
# 	2:13:CLASS: CMemDC:CMemDC
# 	2:10:ENUM: enum:enum
# 	2:16:CLASS: CButtonST:CButtonST
# 	2:9:BtnST.cpp:BtnST.cpp
# 	2:19:Application Include:MotorControlBench.h
# 	2:7:BtnST.h:BtnST.h
# End Section
# Section MotorControlBench : {AD630325-F224-11D6-8120-0050FC78FDEC}
# 	2:5:Class:CDigitalDisp
# 	2:10:HeaderFile:digitaldisp.h
# 	2:8:ImplFile:digitaldisp.cpp
# End Section
# Section MotorControlBench : {A6D193FA-E6C7-4AC2-94C0-711A811EB8A4}
# 	1:17:IDD_S_MODE_DIALOG:112
# 	2:15:CLASS: SMODEDLG:SMODEDLG
# 	2:16:Resource Include:resource.h
# 	2:12:SMODEDLG.cpp:SMODEDLG.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:IDD_S_MODE_DIALOG:IDD_S_MODE_DIALOG
# 	2:10:SMODEDLG.h:SMODEDLG.h
# 	2:19:Application Include:motorcontrolbench.h
# End Section
# Section MotorControlBench : {0C7C983F-7C05-491D-B25C-5B97865BA270}
# 	2:11:MyTabCtrl.h:MyTabCtrl.h
# 	2:17:CLASS: CMyTabCtrl:CMyTabCtrl
# 	2:13:MyTabCtrl.cpp:MyTabCtrl.cpp
# 	2:19:Application Include:motorcontrolbench.h
# End Section
