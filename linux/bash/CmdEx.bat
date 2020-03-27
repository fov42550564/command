@echo off

set /a isFile=0

(2>nul pushd %1)||(set /a isFile=1)

if %isFile% EQU 0 (
set pa=%~dp1
) else (
set pa=%1
)
cmd.exe /s /k pushd "%pa%"
