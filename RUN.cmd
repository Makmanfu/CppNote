
@ECHO OFF
title CppNote 维护
cls
set VCKILL=C
set /p VCKILL=如何操作？(P-打包/C-清理[默认]): 
if %VCKILL%==P ( Goto PACKWORK ) 
if %VCKILL%==p ( Goto PACKWORK ) 
if %VCKILL%==C ( Goto CLEAN ) 
if %VCKILL%==c ( Goto CLEAN ) 
if %VCKILL%==E ( Goto CLEANBUILD ) 
if %VCKILL%==e ( Goto CLEANBUILD ) 
GOTO CLEANBUILD
:CLEANBUILD
cls
del /s /q *.APS *.plg *.ncb *.opt *.obj *.sdf *.pch *.pdb *.tlog *.exp *.log *.manifest *.lastbuildstate *.unsuccessfulbuild *.ilk *.exp *.user *.pro.user
GOTO EXIT
:CLEAN
cls
echo 清理中...
RD /s /q BIN LIB ipch
del /f /s /q /a .\Thumbs.db 
del /s /q /f /A :H *.suo
del /s /q *.APS *.plg *.ncb *.opt *.obj *.sdf *.pch *.pdb *.tlog *.exp *.log *.manifest *.lastbuildstate *.unsuccessfulbuild *.ilk *.exp *.user *.pro.user
for /f "delims=" %%a in ('dir /A=D /s /b debug*') do rd /s /q "%%~a"
for /f "delims=" %%a in ('dir /A=D /s /b build*') do rd /s /q "%%~a"
for /f "delims=" %%a in ('dir /A=D /s /b release*') do rd /s /q "%%~a"
for /f "delims=" %%a in ('dir /A=D /s /b GeneratedFiles*') do rd /s /q "%%~a"
for /f "delims=" %%a in ('dir /A=D /s /b moc*') do rd /s /q "%%~a"
for /f "delims=" %%a in ('dir /A=D /s /b tmp*') do rd /s /q "%%~a"
for /f "delims=" %%a in ('dir /A=D /s /b cbin*') do rd /s /q "%%~a"
cls
GOTO EXIT
:PACKWORK
set YYYYmmdd=%date:~0,4%%date:~5,2%%date:~8,2%
set hhmiss=%time:~0,2%%time:~3,2%%time:~6,2%
set "YYYYmmdd=%YYYYmmdd: =0%"
set "hhmiss=%hhmiss: =0%"
set "PackName=CppNote_%YYYYmmdd%_%hhmiss%.cexe"
cd ..
REM -pHAMIGUA
REM ECHO %PackName%
.\CppNote\Note\TOOL\cab.exe a -t7z -sfx ./%PackName% ./CppNote -pHAMIGUA -mx=9 -ms=200m -mf -mhc -mhcf -mmt 
GOTO EXIT
:EXIT
exit

