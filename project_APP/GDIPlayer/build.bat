@ECHO OFF

:build_all
REM ��ǰĿ¼������Ŀ
CD %~dp0
RD /S /Q CBIN tmp
MD tmp
CD tmp
REM ����MSVC������
CALL B:\Develop\Compiler\VC2013\BIN\vcvarsall.BAT
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0
RD /S /Q tmp
GOTO END
:END
start .\CBIN\GDIPlayer.exe

