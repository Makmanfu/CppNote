@echo off

:build_all
REM 调用MSVC编译器
CALL Z:\B\Develop\Compiler\VC2013\BIN\vcvarsall.BAT x64

REM 当前目录生成项目
CD %~dp0
RD /S /Q BIN tmp
MD tmp
CD tmp
REM 编译工程
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0
RD /S /Q tmp
GOTO END
:END
start .\bin\MFCPlayer.exe

