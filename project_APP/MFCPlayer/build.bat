@echo off

:build_all
REM ����MSVC������
CALL Z:\B\Develop\Compiler\VC2013\BIN\vcvarsall.BAT x64

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
start .\bin\MFCPlayer.exe

