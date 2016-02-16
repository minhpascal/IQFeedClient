SETLOCAL
ECHO off

set QTDIR=C:\Qt\Qt5.5.0\5.5\msvc2013_64

PATH=%QTDIR%\bin;%PATH%

if not exist Build\win64\Debug mkdir Build\win64\Debug
cd Build\win64\Debug
"c:\Program Files (x86)\CMake\bin\cmake.exe" -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 12 2013 Win64" ..\..\..\

cd %~dp0

start "" DEVENV.exe Build\win64\Debug\IQFeedClient.sln

ENDLOCAL
