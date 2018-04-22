@ECHO OFF
REM 设置生成程序
SET PATHEXE=.\bin\QtRouterMang.exe
if not exist "%PATHEXE%" (GOTO build_all) else (GOTO RUN)

:build_all
REM 调用MSVC
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT
CALL:SETLIBPATH
REM TMP目录生成项目
CD %~dp0 & RD /S /Q tmp BIN & MD tmp & CD tmp
REM 编译工程
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0 & RD /S /Q tmp & GOTO RUN
:RUN
CALL:SETLIBPATH
START %PATHEXE%
GOTO END
:buildGUI
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT
cmake-GUI
GOTO END
REM --------------------------------------------------------------------
:CLEAN
RD /S /Q tmp BIN
GOTO END
:SETLIBPATH
REM 设置环境变量
set QTDIR=D:\Develop\QT\Qt4.8.6\4.8\msvc12
set QTPATH=%QTDIR%
REM set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%PATH%
GOTO END
:EXIT
EXIT
:END


