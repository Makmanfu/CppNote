@ECHO OFF

:build_all
REM ��ǰĿ¼������Ŀ
CD %~dp0
RD /S /Q BIN tmp
MD tmp
CD tmp
REM ����MSVC������
CALL D:\Develop\Compiler\VC2013\BIN\vcvarsall.BAT
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0
RD /S /Q tmp
GOTO END
:END
start .\bin\GDIPlayer.exe

