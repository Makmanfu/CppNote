@echo off

:build_all
REM ����MSVC������
CALL D:\Develop\Compiler\VC2017\bin\vcvarsall.BAT

REM ��ǰĿ¼������Ŀ
CD %~dp0
RD /S /Q CBIN tmp
MD tmp
CD tmp
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE /C
CD %~dp0
RD /S /Q tmp
GOTO END
:END
rem PAUSE
depends.exe .\CBIN\SQCore.exe


exit
depends.exe .\CBIN\SQCore.exe

.\CBIN\SQCore.exe