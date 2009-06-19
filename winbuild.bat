@echo off

SET RELEASE_VERSION=0.3rc2
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
echo Zipping
.\tools\zip -r %ZIPNAME%.zip %WORKDIR%
echo Cleaning up work directory
rd /S /Q %WORKDIR%
echo Done.
