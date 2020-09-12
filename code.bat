@echo off

set THISDIR=%~dp0

set ELECTRON_NO_ATTACH_CONSOLE=true

set OPENDIR=""

if "%1"=="." (set OPENDIR=".")

start vs_code_editor.exe %OPENDIR% --user-data-dir "%THISDIR%/userdata" --extensions-dir "%THISDIR%/extensions" &