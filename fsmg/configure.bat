@echo off
rem This batch file assumes a unix-type "sed" program

echo # *** generate by configure.bat >Makefile

if %1.==. goto :default
REM if %1.==icc. goto :VAC
goto default

:VAC
sed -f config.vac Makefile.in >> Makefile
echo config.status >config.status
goto :end

:default
copy conf.ms conf.h
sed -f config.ms Makefile.in >> Makefile
echo config.status >config.status
:end