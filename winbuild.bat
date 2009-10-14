@echo off

SET MAJOR_NUM=0
SET MINOR_NUM=5
SET REVISION_NUM=0
SET IS_RC=yes

IF "%IS_RC%"=="yes" (
SET RELEASE_VERSION=%MAJOR_NUM%.%MINOR_NUM%rc%REVISION_NUM%
) ELSE (
SET RELEASE_VERSION=%MAJOR_NUM%.%MINOR_NUM%
)
SET WORKDIR=libfreespace
SET ZIPNAME=libfreespace-%RELEASE_VERSION%-win32

REM Create the .rc file

echo // Microsoft Visual C++ generated resource script. > win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo #include "resource.h" >> win32\libfreespace.rc
echo #define APSTUDIO_READONLY_SYMBOLS >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo // Generated from the TEXTINCLUDE 2 resource. >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo #include "afxres.h" >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo #undef APSTUDIO_READONLY_SYMBOLS >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo // English (U.S.) resources >> win32\libfreespace.rc
echo #if !defined(AFX_RESOURCE_DLL) ^|^| defined(AFX_TARG_ENU) >> win32\libfreespace.rc
echo #ifdef _WIN32 >> win32\libfreespace.rc
echo LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US >> win32\libfreespace.rc
echo #pragma code_page(1252) >> win32\libfreespace.rc
echo #endif //_WIN32 >> win32\libfreespace.rc
echo #ifdef APSTUDIO_INVOKED >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo // TEXTINCLUDE >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo 1 TEXTINCLUDE  >> win32\libfreespace.rc
echo BEGIN >> win32\libfreespace.rc
echo     "resource.h\0" >> win32\libfreespace.rc
echo END >> win32\libfreespace.rc
echo 2 TEXTINCLUDE  >> win32\libfreespace.rc
echo BEGIN >> win32\libfreespace.rc
echo     "#include ""afxres.h""\r\n" >> win32\libfreespace.rc
echo     "\0" >> win32\libfreespace.rc
echo END >> win32\libfreespace.rc
echo 3 TEXTINCLUDE >> win32\libfreespace.rc
echo BEGIN >> win32\libfreespace.rc
echo     "\r\n" >> win32\libfreespace.rc
echo     "\0" >> win32\libfreespace.rc
echo END >> win32\libfreespace.rc
echo #endif    // APSTUDIO_INVOKED >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo // Version >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo VS_VERSION_INFO VERSIONINFO >> win32\libfreespace.rc
echo  FILEVERSION %MAJOR_NUM%,%MINOR_NUM%,%REVISION_NUM%,0 >> win32\libfreespace.rc
echo  PRODUCTVERSION %MAJOR_NUM%,%MINOR_NUM%,%REVISION_NUM%,0 >> win32\libfreespace.rc
echo  FILEFLAGSMASK 0x17L >> win32\libfreespace.rc
echo #ifdef _DEBUG >> win32\libfreespace.rc
echo  FILEFLAGS 0x1L >> win32\libfreespace.rc
echo #else >> win32\libfreespace.rc
echo  FILEFLAGS 0x0L >> win32\libfreespace.rc
echo #endif >> win32\libfreespace.rc
echo  FILEOS 0x4L >> win32\libfreespace.rc
echo  FILETYPE 0x2L >> win32\libfreespace.rc
echo  FILESUBTYPE 0x0L >> win32\libfreespace.rc
echo BEGIN >> win32\libfreespace.rc
echo     BLOCK "StringFileInfo" >> win32\libfreespace.rc
echo     BEGIN >> win32\libfreespace.rc
echo         BLOCK "040904b0" >> win32\libfreespace.rc
echo         BEGIN >> win32\libfreespace.rc
echo             VALUE "CompanyName", "Hillcrest Labs, Inc." >> win32\libfreespace.rc
echo             VALUE "FileDescription", "libfreespace DLL" >> win32\libfreespace.rc
echo             VALUE "FileVersion", "%MAJOR_NUM%, %MINOR_NUM%, %REVISION_NUM%, 0" >> win32\libfreespace.rc
echo             VALUE "InternalName", "libfreespace" >> win32\libfreespace.rc
echo             VALUE "LegalCopyright", "Copyright (C) Hillcrest Labs 2009" >> win32\libfreespace.rc
echo             VALUE "OriginalFilename", "libfreespace.dll" >> win32\libfreespace.rc
echo             VALUE "ProductName", "libfreespace" >> win32\libfreespace.rc
echo             VALUE "ProductVersion", "%MAJOR_NUM%, %MINOR_NUM%, %REVISION_NUM%, 0" >> win32\libfreespace.rc
echo         END >> win32\libfreespace.rc
echo     END >> win32\libfreespace.rc
echo     BLOCK "VarFileInfo" >> win32\libfreespace.rc
echo     BEGIN >> win32\libfreespace.rc
echo         VALUE "Translation", 0x409, 1200 >> win32\libfreespace.rc
echo     END >> win32\libfreespace.rc
echo END >> win32\libfreespace.rc
echo #endif    // English (U.S.) resources >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo #ifndef APSTUDIO_INVOKED >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo // Generated from the TEXTINCLUDE 3 resource. >> win32\libfreespace.rc
echo // >> win32\libfreespace.rc
echo ///////////////////////////////////////////////////////////////////////////// >> win32\libfreespace.rc
echo #endif    // not APSTUDIO_INVOKED >> win32\libfreespace.rc

REM Generate config-win32.h and the line below when the version changes.
echo #define VERSION "%RELEASE_VERSION%"> config-win32.h


SET WORKDIR=libfreespace
SET ZIPNAME=libfreespace-%RELEASE_VERSION%-win32

REM This must be run from the Visual Studio 2005 Command Prompt
devenv win32\libfreespace.sln /clean release
devenv win32\libfreespace.sln /clean debug
devenv win32\libfreespace.sln /build release
devenv win32\libfreespace.sln /build debug

echo Cleaning up work directory if any
rd /S /Q %WORKDIR%
echo Creating working directory
mkdir %WORKDIR%
mkdir %WORKDIR%\lib
mkdir %WORKDIR%\include
mkdir %WORKDIR%\include\freespace
mkdir %WORKDIR%\lib\Debug
mkdir %WORKDIR%\lib\Release
echo Copying DLLs and LIBs
copy win32\Debug\libfreespaced.lib %WORKDIR%\lib\Debug
copy win32\Debug\libfreespaced.dll %WORKDIR%\lib\Debug
copy win32\Release\libfreespace.lib %WORKDIR%\lib\Release
copy win32\Release\libfreespace.dll %WORKDIR%\lib\Release
echo Copying header files
copy include\freespace\*.h %WORKDIR%\include\freespace

REM Create ZIP file using Info-ZIP
REM http://www.info-zip.org/Zip.html
echo Zipping - be sure to have zip.exe in your path or you will get an error below
zip -r %ZIPNAME%.zip %WORKDIR%
echo Cleaning up work directory
rd /S /Q %WORKDIR%
echo Done.
