@echo off
REM 设置编译器路径(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=B:\Develop\Compiler
REM 设置生成程序
SET EXENAME=SQSock.exe
REM 设置生成平台
SET PLATFORM=x86
REM 设置编译器版本
SET CCNUM=12


















SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
:BUILD_ALL
REM 调用MSVC编译器
if %COMPILERDIR%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC64.BAT %CCNUM%
)
REM TMP目录生成项目
CD %~dp0 &RD /S /Q CBIN tmp &MD tmp &CD tmp
REM 编译工程
cmake -G"NMake Makefiles JOM" ..
jom -j 4
::NMAKE
CD %~dp0 &RD /S /Q tmp & GOTO RUN
:BUILDGUI
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC64.BAT %CCNUM%
)
cmake-GUI
GOTO END
:CLEAN
RD /S /Q tmp CBIN
GOTO END
:RUN
CD CBIN & START %EXENAME%
GOTO END
:EXIT
EXIT
:END

