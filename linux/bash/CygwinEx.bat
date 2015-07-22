@echo off

set root=c:\linux\linux
set /a isFile=0

(2>nul pushd %1)||(set /a isFile=1)
for /f %%a in ('%root%\bin\cygpath.exe %1') do (
	set pa=%%a
)
if %isFile% EQU 0 (
start %root%\bin\mintty.exe -i /Cygwin-Terminal.ico -t "mintty" -e /bin/bash --login -c "cd '%pa%';exec bash --rcfile ~/.bashrc"
) else (
start %root%\bin\mintty.exe -i /Cygwin-Terminal.ico -t "mintty" -e /bin/bash --login -c "cd $(dirname %pa%);export FILE=$(basename %pa%);exec bash --rcfile ~/.bashrc"
)