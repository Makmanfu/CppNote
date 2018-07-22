@echo off
REM ���ñ�����·��(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=B:\Develop\Compiler
REM �������ɳ���
SET EXENAME=TestWin32.exe
REM ��������ƽ̨
SET PLATFORM=x86
REM ���ñ������汾
SET CCNUM=14






SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
:BUILD_ALL
REM ����MSVC������
if %COMPILERDIR%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC64.BAT %CCNUM%
)
REM TMPĿ¼������Ŀ
CD %~dp0 &RD /S /Q CBIN tmp &MD tmp &CD tmp
REM ���빤��
::cmake -G"NMake Makefiles" ..
::NMAKE
cmake  -G "NMake Makefiles JOM"  .. 
jom -j 4
CD %~dp0
RD /S /Q tmp & GOTO RUN
:BUILDGUI
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC64.BAT %CCNUM%
)
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

