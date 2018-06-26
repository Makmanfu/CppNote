@echo off
REM ���ñ�����·��(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=%~dp0..\..
REM �������ɳ���
SET EXENAME=TestMFC.exe
REM ��������ƽ̨
SET PLATFORM=x64
REM ���ñ������汾
SET CCNUM=10



















SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
:BUILD_ALL
REM ����MSVC������CALL vcvarsall.BAT x86
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
CALL %COMPILERDIR%\VC2013\bin\vcvarsall.BAT %CCNUM%
cmake-GUI
GOTO END
:CLEAN
RD /S /Q tmp CBIN
GOTO END
:RUN
CD CBIN & START %EXENAME%
GOTO END
:EXIT
EXIT
:END


