@echo off

:build_all
REM ����MSVC������
CALL D:\Develop\Compiler\VC2017\BIN\vcvarsall.BAT

REM ��ǰĿ¼������Ŀ
CD %~dp0
RD /S /Q BIN tmp
MD tmp
CD tmp
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0
RD /S /Q tmp
GOTO END
:END
start .\bin\TestMFC.exe

