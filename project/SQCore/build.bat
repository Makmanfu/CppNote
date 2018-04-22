@echo off

:build_all
REM 调用MSVC编译器
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT

REM 当前目录生成项目
CD %~dp0
RD /S /Q BIN tmp
MD tmp
CD tmp
REM 编译工程
cmake -G"NMake Makefiles" ..
NMAKE /C
CD %~dp0
RD /S /Q tmp
GOTO END
:END
PAUSE
depends.exe .\bin\SQCore.exe

