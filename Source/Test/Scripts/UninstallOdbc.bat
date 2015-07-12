@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : Uninstall_odbc.bat
REM
REM Description : Uninstall an ODBC source on the machine which executes this script.
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

:: Check existing key
REG QUERY "%HKEY_REGISTRY%\Software\ODBC\ODBC.INI\ODBC Data Sources" /v %SOURCE_NAME%
IF %ERRORLEVEL% EQU 0 (
    :: Unregister all keys
    REG DELETE "%HKEY_REGISTRY%\Software\ODBC\ODBC.INI\%SOURCE_NAME%" /f
    REG DELETE "%HKEY_REGISTRY%\Software\ODBC\ODBC.INI\ODBC Data Sources" /v %SOURCE_NAME% /f
)

:: Reset ERRORLEVEL
ver > nul

:: ===================================================
:: End of main batch procedure
:: ===================================================
