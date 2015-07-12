@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : Install_odbc_fdms.bat
REM
REM Description : Install an ODBC source for accessing to an ARIA database.
REM
REM Project     : ARIA
REM
REM *************************************************************************
REM Contributors :
REM    Version 1.00: 06/30/2013 2:44:32 PM, Author: ezeyen, Original hack  
REM
REM *************************************************************************
REM (c) Copyright 2013 AREVA NDE SOLUTIONS - FRANCE - INTERCONTROLE
REM     All Rights Reserved.
REM *********************************<+>*************************************

:: ===================================================
:: Variables
:: ===================================================

SET SOURCE_NAME=%1
SET SERVER=%2
SET PORT=%3
SET DATABASE_NAME=%4
SET USER=%5
SET PASSWORD=%6
SET DRIVER_PATH=C:\Windows\system32\sqlncli11.dll
SET DRIVER_DESCRIPTION=SQL Server Native Client 11.0
SET DESCRIPTION=FDMS
SET HKEY=CURRENT_USER

:: ===================================================
:: End of variables
:: ===================================================

:: ===================================================
:: Main batch procedure
:: ===================================================

:: Determine the the registry key (LOCAL_MACHINE needs administrator rights, CURRENT_USER doesn't)
IF "%HKEY%"=="LOCAL_MACHINE" (
    SET HKEY_REGISTRY=HKEY_LOCAL_MACHINE
) ELSE IF "%HKEY%"=="CURRENT_USER" (
    SET HKEY_REGISTRY=HKEY_CURRENT_USER
) ELSE (
    ECHO "HKEY not valid! Exit."
    GOTO:EOF
)

:: Set common keys
CALL %~dp0InstallOdbc.bat %SOURCE_NAME% "%SERVER%" "%DATABASE_NAME%" "%DRIVER_PATH%" "%DRIVER_DESCRIPTION%" "%DESCRIPTION%" %HKEY_REGISTRY%

:: Set specific keys
REG ADD %HKEY_REGISTRY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v LastUser /t REG_SZ /d %USER%

:: ===================================================
:: End of main batch procedure
:: ===================================================
