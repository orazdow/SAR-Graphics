@ECHO off

rem MSVC build bat file
rem run with vscmd: cmd /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

set target=out_w32.exe
set builddir=build\

IF not [%1]==[] (GOTO args)

rem mkdir %builddir%

set includes=/ID:\Libraries\glad\include /ID:\Libraries\glfw\include /ID:\Libraries\SOIL\src

set libdirs=/LIBPATH:D:\Libraries\glfw\lib-vc2017 /LIBPATH:D:\Libraries\SOIL\lib\VC

set libs=SOIL.lib glfw3.lib user32.lib gdi32.lib shell32.lib msvcrt.lib opengl32.lib

rem set copts=/fp:fast
set static=/MT
set define=/DOS_W32
rem set debug=/Zi /DEBUG
set buildopt=/Fo%builddir%

set files=main.cpp D:\Libraries\glad\src\glad.c

set opts= %copts% %static% %define% /Fe%target% %buildopt% %debug%

cl %opts% %includes% %files% /link %libdirs% %libs%

GOTO end

:args
IF [%1]==[clean] (del %builddir%\* del %target%)
IF [%1]==[run] (%target%)

:end