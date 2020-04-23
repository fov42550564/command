@echo off

set root=c:\cygwin
set _pa_=%1
set _pa_=%_pa_:~1,-1%
start %root%\bin\mintty.exe -i /Cygwin-Terminal.ico -t "%_pa_%" -e /bin/bash --login
