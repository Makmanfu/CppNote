@ECHO OFF

:build_all
REM ��ǰĿ¼������Ŀ
CD %~dp0
RD /S /Q CBIN tmp
MD tmp
CD tmp
REM ����MSVC������
CALL D:\Develop\Compiler\VC2017\BIN\vcvarsall.BAT
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0
RD /S /Q tmp
GOTO END
:build_gui_all
REM ����MSVC������
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT
cmake-GUI
:END
start .\CBIN\TestWin32.exe

