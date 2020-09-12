@echo off

if "%~1"=="" (
	echo Requires one argument to use as the commit message
) else (
	git add --all
	git commit -m%1
	git push
)


