@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : Install_odbc_aria.bat
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
SET SERVER=127.0.0.1
SET DATABASE_NAME=%1
SET DRIVER_PATH=C:\Program Files (x86)\MySQL\Connector ODBC 5.3\myodbc5w.dll
SET DRIVER_DESCRIPTION=MySQL ODBC 5.3 Unicode Driver
SET DESCRIPTION=%1

SET PORT=3306
SET USER=test_user
SET PASSWORD=test_pwd
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
REG ADD %HKEY_REGISTRY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v UID /t REG_SZ /d %USER%
REG ADD %HKEY_REGISTRY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v PWD /t REG_SZ /d %PASSWORD%
REG ADD %HKEY_REGISTRY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v PORT /t REG_SZ /d %PORT%

:: ===================================================
:: End of main batch procedure
:: ===================================================
