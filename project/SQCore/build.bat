@echo off

:build_all
REM ����MSVC������
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT

REM ��ǰĿ¼������Ŀ
CD %~dp0
RD /S /Q BIN tmp
MD tmp
CD tmp
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE /C
CD %~dp0
RD /S /Q tmp
GOTO END
:END
PAUSE
depends.exe .\bin\SQCore.exe

