@ECHO OFF
REM �������ɳ���
SET PATHEXE=.\CBIN\Qt5Play.exe
if not exist "%PATHEXE%" (GOTO build_all) else (GOTO RUN)

:build_all
REM ����MSVC
CALL D:\Develop\Compiler\VC2013\bin\vcvarsall.BAT
CALL:SETLIBPATH
REM TMPĿ¼������Ŀ
CD %~dp0 & RD /S /Q tmp CBIN & MD tmp & CD tmp
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0 & RD /S /Q tmp & GOTO RUN
:RUN
CALL:SETLIBPATH
START %PATHEXE%
GOTO END
REM --------------------------------------------------------------------
:CLEAN
RD /S /Q tmp CBIN
GOTO END
:SETLIBPATH
REM ���û�������
set QTDIR=D:\Develop\QT\Qt5.8.0\5.8\msvc12
set QTPATH=%QTDIR%
set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%PATH%
GOTO END
:EXIT
EXIT
:END


