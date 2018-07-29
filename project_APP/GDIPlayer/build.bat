@ECHO OFF
REM 设置编译器路径(B:\Develop\Compiler  %~dp0..\.. )
SET COMPILERDIR=B:\Develop\Compiler
REM 设置生成程序
SET EXENAME=GDIPlayer.exe
REM 设置生成平台
SET PLATFORM=x86
REM 设置编译器版本
SET CCNUM=12






















SET PATHEXE=.\CBIN\%EXENAME%
if not exist "%PATHEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
GOTO END
:BUILD_ALL
REM 调用MSVC编译器CALL vcvarsall.BAT x86
if %COMPILERDIR%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC.BAT %CCNUM% x64
)
REM TMP目录生成项目
CD %~dp0 &RD /S /Q CBIN tmp &MD tmp &CD tmp
REM 编译工程
::cmake -G"NMake Makefiles" ..
::NMAKE
:: cmake  -G "NMake Makefiles JOM" -DCMAKE_MAKE_PROGRAM=d:\jom-1.1.2\jom.exe .
cmake  -G "NMake Makefiles JOM"  .. 
jom -j 4
CD %~dp0 &RD /S /Q tmp & GOTO RUN
GOTO END
:BUILDGUI
if %PLATFORM%==x86 (
    CALL %COMPILERDIR%\VC.BAT %CCNUM%
) else (
    CALL %COMPILERDIR%\VC.BAT %CCNUM% x64
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







