@ECHO OFF
REM ���ñ�����·��(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=B:\Develop\Compiler
REM �������ɳ���
SET EXENAME=QtTomato.exe
REM ��������ƽ̨
SET PLATFORM=x86
REM ���ñ������汾
SET CCNUM=12





SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
:BUILD_ALL
REM ����MSVC������
if %COMPILERDIR%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL:SETLIBPATH
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL:SETLIBPATH64
    CALL %COMPILERDIR%\VC64.BAT %CCNUM%
)
REM TMPĿ¼������Ŀ
CD %~dp0 &RD /S /Q CBIN tmp &MD tmp &CD tmp
REM ���빤��
cmake -G"NMake Makefiles JOM" ..
jom -j 4
::NMAKE
CD %~dp0 & RD /S /Q tmp & GOTO RUN
:BUILDGUI
if %PLATFORM%==x86 (
    CALL:SETLIBPATH
    CALL %COMPILERDIR%\VC2013\bin\vcvarsall.BAT x86
) else (
    CALL:SETLIBPATH64
    CALL %COMPILERDIR%\VC2013\bin\vcvarsall.BAT x64
)
cmake-GUI
GOTO END
:CLEAN
RD /S /Q tmp CBIN
GOTO END
:SETLIBPATH
REM ���û�������
set QTDIR=%COMPILERDIR%\..\QT\Qt4.8.6\4.8\msvc12
set QTPATH=%QTDIR%
REM set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%PATH%
GOTO END
:SETLIBPATH64
REM ���û�������
set QTDIR=%COMPILERDIR%\..\QT\Qt4.8.6\4.8\msvc12_64
set QTPATH=%QTDIR%
REM set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%PATH%
GOTO END
:RUN
IF "%QTDIR%"=="" (
    if %PLATFORM%==x86 (
        CALL:SETLIBPATH
    ) else (
        CALL:SETLIBPATH64
    )
)
START %PATHEXE%
GOTO END
:EXIT
EXIT
:END





