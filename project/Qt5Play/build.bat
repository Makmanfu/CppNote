@ECHO OFF
REM 设置编译器路径(B:\Develop\Compiler  %~dp0..\.. )
SET CSHELL=B:\Develop\Compiler
REM 设置生成程序
SET EXENAME=Qt5Play
REM 设置生成平台
SET PLATFORM=x86
REM 设置编译器版本
SET CCNUM=12


















:START
REM 调用MSVC编译器
if %CSHELL%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL:SETLIBPATH
    CALL %CSHELL%\VC.BAT %CCNUM% x86
    SET RUNEXE=%EXENAME%.exe
) else (
    CALL:SETLIBPATH64
    CALL %CSHELL%\VC.BAT %CCNUM% x64
    SET RUNEXE=%EXENAME%64.exe
)
if not exist ".\CBIN\%RUNEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
GOTO EOF
:BUILD_ALL
CALL:TMPDODIR
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
REM 编译工程
::cmake -G"NMake Makefiles" ..
::NMAKE
cmake  -G "NMake Makefiles JOM"  .. 
jom -j 4
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CALL:RUN
GOTO EOF
:TMPDODIR
CD %~dp0&RD /S /Q CBIN tmp&MD tmp&CD tmp
GOTO EOF
:RUN
CD %~dp0 
RD /S /Q tmp
CD CBIN
START %~dp0CBIN\%RUNEXE%
GOTO EOF
:SETLIBPATH
REM 设置环境变量
set QTDIR=%CSHELL%\..\QT\Qt5.8.0\5.8\msvc12
set QTPATH=%QTDIR%
set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%QTDIR%\plugins;%PATH%
GOTO EOF
:SETLIBPATH64
REM 设置环境变量
set QTDIR=%CSHELL%\..\QT\Qt5.8.0\5.8\msvc12_64
set QTPATH=%QTDIR%
set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%QTDIR%\plugins;%PATH%
GOTO EOF
:EXIT
EXIT
:EOF







