@echo off
set PAR=
set SOURCE=%1
:Bcl
shift
if %1.==. goto :Next
set PAR=%SOURCE% %PAR%
set SOURCE=%1
goto :Bcl

:Next
fsmg -wk stat %PAR% -o tmp.c %SOURCE%
nmake tmp.exe
tmp.exe
del tmp.c
del tmp.obj
del tmp.exe
