@echo off
REM ���ñ�����·��(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=B:\Develop\Compiler
REM �������ɳ���
SET EXENAME=SQSock.exe
REM ��������ƽ̨
SET PLATFORM=x86
REM ���ñ������汾
SET CCNUM=12


















SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
:BUILD_ALL
REM ����MSVC������CALL B:\Develop\Compiler\VC2013\bin\vcvarsall.BAT x86
if %COMPILERDIR%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC64.BAT %CCNUM%
)
REM TMPĿ¼������Ŀ
CD %~dp0 &RD /S /Q CBIN tmp &MD tmp &CD tmp
REM ���빤��
cmake -G"NMake Makefiles" ..
NMAKE
CD %~dp0 &RD /S /Q tmp & GOTO RUN
:BUILDGUI
CALL B:\Develop\Compiler\VC2013\bin\vcvarsall.BAT
cmake-GUI
GOTO END
:CLEAN
RD /S /Q tmp CBIN
GOTO END
:RUN
pause
CD CBIN & START %EXENAME%
GOTO END
:EXIT
EXIT
:END
