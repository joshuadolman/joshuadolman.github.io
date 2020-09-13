@echo off
set UNITY_FILE=webgen/src/main.cpp
set EXE=webgen

REM d - debug / 2 - release
set OPTIMISER_LEVEL=d

REM OS_WIN32 / OS_LINUX /
set OS=OS_WIN32

if "%OS%"=="OS_WIN32" (
	REM Only used on windows, set's the executable mode to "console" or "windows"
	set WIN32_EXEC_MODE=console
)

REM COMPILER_MSVC / COMPILER_GCC / COMPILER_CLANG
set COMPILER=COMPILER_MSVC

REM CRT_DISABLED - disables the crt
set CRT=

REM RELEASEMODE_INTERNAL - for dev only
set RELEASEMODE=RELEASEMODE_INTERNAL

set DEFINES=-D%OS% -D%COMPILER% -D%CRT% -D%RELEASEMODE%
REM -DUNINCLUDE
if "%OPTIMISER_LEVEL%"=="d" (
    set DEFINES=%DEFINES% -DJPD_ASSERTS
)
REM =========================
REM --- Disabled Warnings ---
REM =========================
set DISABLEDWARNINGS=-wd4100 -wd4189 -wd4710 -wd4668 -wd4711 -wd4820 -wd4505 -wd4211 -wd4201 -wd4996 -wd4005
REM 4100 - "Unreferenced formal parameter" (Unused function parameter)
REM 4189 - "Local Variable is Initialized but not referenced" (Unused Variables?)
REM 4711 - "xx selected for automatic inline expansion" (Function auto-inlined?)
REM 4820 - "xx bytes padding added after data member xx" (Struct padding warning)
REM 4505 - "xx unreferenced local function has been removed" (Unused 'internal' function?)
REM 4211 - redefined extern to static (thinks we have an internal function that is supposed to be external)
REM 4201 - "nonstandard extension used: nameless struct/union" (anonymous struct + union may not be supported on every c++ compiler)
REM 4996 - Decprecated stuff
REM 4005 - Macro Redefinition
REM  -wd4838
REM 4838 - conversion from unsigned int to INT requires a narrowing conversion (for xnamath.h ugggh)

set INLCUDE_PATH_LIST="W:/cpp/shared/"
set LIB_PATH_LIST=

setlocal EnableDelayedExpansion
set INCLUDE_PATHS=
set LIB_PATHS=

(for %%a in (%INLCUDE_PATH_LIST%) do (
	set INCLUDE_PATHS=!INCLUDE_PATHS! -I%%a
))

(for %%a in (%LIB_PATH_LIST%) do (
	set LIB_PATHS=!LIB_PATHS! -LIBPATH:%%a
))
setlocal DisableDelayedExpansion

set MULTITHREAD_FLAG=
set DISABLE_CRT=
if "%CRT%"=="CRT_DISABLED" (
	if "%OPTIMISER_LEVEL%"=="d" (
		set MULTITHREAD_FLAG=-MTd
	) else (
		set MULTITHREAD_FLAG=-MT
	)
	set DISABLE_CRT=-nodefaultlib
) else (
	if "%OPTIMISER_LEVEL%"=="d" (
		set MULTITHREAD_FLAG=-MDd
	) else (
		set MULTITHREAD_FLAG=-MD
	)
)

set STACKSIZE=20485760
set COMPILERFLAGS= -F%STACKSIZE% %MULTITHREAD_FLAG% -O%OPTIMISER_LEVEL% -GS- -Gs9999999 -Gm- -GR- -EHa -utf-8 -Oi -Zi -nologo -FC -W4 -WX %DISABLEDWARNINGS% %INCLUDE_PATHS%

set LIBS=kernel32.lib user32.lib
set LINKERFLAGS= %DISABLE_CRT% -incremental:no -opt:ref -subsystem:%WIN32_EXEC_MODE% -stack:%STACKSIZE%,%STACKSIZE% %LIBS% %LIB_PATHS%

if "%1"=="-v" (
	@echo on
)
cl %DEFINES% %COMPILERFLAGS% -Fm:%EXEFILE%.map -Fe:%EXE%.exe %UNITY_FILE% /link %LINKERFLAGS%
@echo off

del *.obj 2> nul
