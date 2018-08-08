@ECHO OFF
REM ���ñ�����·��(B:\Develop\Compiler  %~dp0..\.. )
SET CSHELL=B:\Develop\Compiler
REM �������ɳ���
SET EXENAME=GLGPU
REM ��������ƽ̨
SET PLATFORM=x64
REM ���ñ������汾
SET CCNUM=12


















:START
REM ����MSVC������
if %CSHELL%=="" GOTO EXIT
if %PLATFORM%==x86 (
    CALL %CSHELL%\VC.BAT %CCNUM% x86
    SET RUNEXE=%EXENAME%.exe
) else (
    CALL %CSHELL%\VC.BAT %CCNUM% x64
    SET RUNEXE=%EXENAME%64.exe
)
if not exist ".\CBIN\%RUNEXE%" (GOTO BUILD_ALL) else (GOTO RUN)
GOTO EOF
:BUILD_ALL
CALL:TMPDODIR
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
REM ���빤��
::cmake -G"NMake Makefiles" ..
::NMAKE
:: cmake  -G "NMake Makefiles JOM" -DCMAKE_MAKE_PROGRAM=d:\jom-1.1.2\jom.exe .
cmake  -G "NMake Makefiles JOM"  .. 
jom -j 4
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
CALL:RUN
GOTO EOF
:TMPDODIR
CD %~dp0&RD /S /Q CBIN tmp&MD tmp&CD tmp
GOTO EOF
:RUN
CD %~dp0 
RD /S /Q tmp
CD CBIN
START %~dp0CBIN\%RUNEXE%
GOTO EOF
:EXIT
EXIT
:EOF

