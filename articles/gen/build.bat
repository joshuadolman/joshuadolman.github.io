@echo off
set FILE=webgen.cpp
set EXE=webgen

set OPTIMISER_LEVEL=d
set OS=OS_WIN32
REM OS_WIN32 / OS_LINUX /

set DEFINES=-DCRT=%CRT% -D%OS% -DUNINCLUDE

REM =========================
REM --- Disabled Warnings ---
REM =========================
set DISABLEDWARNINGS=-wd4100 -wd4189 -wd4710 -wd4668 -wd4711 -wd4820 -wd4505 -wd4211 -wd4201 -wd4996 -wd4005 -wd4838
REM 4100 - "Unreferenced formal parameter" (Unused function parameter)
REM 4189 - "Local Variable is Initialized but not referenced" (Unused Variables?)
REM 4711 - "xx selected for automatic inline expansion" (Function auto-inlined?)
REM 4820 - "xx bytes padding added after data member xx" (Struct padding warning)
REM 4505 - "xx unreferenced local function has been removed" (Unused 'internal' function?)
REM 4211 - redefined extern to static (thinks we have an internal function that is supposed to be external)
REM 4201 - "nonstandard extension used: nameless struct/union" (anonymous struct + union may not be supported on every c++ compiler)
REM 4996 - Decprecated stuff
REM 4005 - Macro Redefinition
REM 4838 - conversion from unsigned int to INT requires a narrowing conversion (for xnamath.h ugggh)

set STACKSIZE=10485760
set COMPILERFLAGS= -F%STACKSIZE% -MT -O%OPTIMISER_LEVEL% -GS- -Gs9999999 -Gm- -GR- -EHa -utf-8 -Oi -Zi -nologo -FC -W4 -WX %DISABLEDWARNINGS% -I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include"

set LIBS=user32.lib
set LINKERFLAGS=-incremental:no -opt:ref -subsystem:console -stack:%STACKSIZE%,%STACKSIZE% %LIBS% -LIBPATH:"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64"

cl %DEFINES% %COMPILERFLAGS% -Fm:%EXEFILE%.map -Fe:%EXE%.exe %FILE% /link %LINKERFLAGS%

del *.obj 2> nul