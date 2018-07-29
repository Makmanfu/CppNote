@ECHO OFF
REM ���ñ�����·��(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=B:\Develop\Compiler
REM �������ɳ���
SET EXENAME=Qt5Play.exe
REM ��������ƽ̨
SET PLATFORM=x86
REM ���ñ������汾
SET CCNUM=12






















SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
GOTO END
:BUILD_ALL
REM ����MSVC������CALL vcvarsall.BAT x86
if %COMPILERDIR%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL:SETLIBPATH
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL:SETLIBPATH64
    CALL %COMPILERDIR%\VC.BAT %CCNUM% x64
)
REM TMPĿ¼������Ŀ
CD %~dp0 &RD /S /Q CBIN tmp &MD tmp &CD tmp
REM ���빤��
::cmake -G"NMake Makefiles" ..
::NMAKE
:: cmake  -G "NMake Makefiles JOM" -DCMAKE_MAKE_PROGRAM=d:\jom-1.1.2\jom.exe .
cmake  -G "NMake Makefiles JOM"  .. 
jom -j 4
CD %~dp0 &RD /S /Q tmp & GOTO RUN
GOTO END
:BUILDGUI
CALL %COMPILERDIR%\VC2013\bin\vcvarsall.BAT %CCNUM%
cmake-GUI
GOTO END
:CLEAN
RD /S /Q tmp CBIN
GOTO END
:SETLIBPATH
REM ���û�������
set QTDIR=%COMPILERDIR%\..\QT\Qt5.8.0\5.8\msvc12
set QTPATH=%QTDIR%
set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%QTDIR%\plugins;%PATH%
GOTO END
:SETLIBPATH64
REM ���û�������
set QTDIR=%COMPILERDIR%\..\QT\Qt5.8.0\5.8\msvc12_64
set QTPATH=%QTDIR%
set CMAKE_PREFIX_PATH=%QTDIR%\lib\cmake
SET PATH=%QTDIR%\BIN;%QTDIR%\plugins;%PATH%
GOTO END
:RUN
if %PLATFORM%==x86 (
echo "x86 run"
    CALL:SETLIBPATH
) else (
echo "x64 run"
    CALL:SETLIBPATH64
)
START %PATHEXE%
GOTO END
:EXIT
EXIT
:END




