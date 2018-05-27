@ECHO OFF

:build_all
REM 当前目录生成项目
CD %~dp0
RD /S /Q CBIN tmp
MD tmp
CD tmp
REM 调用MSVC编译器
CALL D:\Develop\Compiler\VC2017\BIN\vcvarsall.BAT
REM 编译工程
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0
RD /S /Q tmp
GOTO END
:build_gui_all
REM 调用MSVC编译器
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT
cmake-GUI
:END
start .\CBIN\TestWin32.exe

