@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : Install_odbc.bat
REM
REM Description : Install an ODBC source on the machine which executes this script.
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
SET DATABASE_NAME=%3
SET DRIVER_PATH=%4
SET DRIVER_DESCRIPTION=%5
SET DESCRIPTION=%6
SET HKEY=%7

:: ===================================================
:: End of variables
:: ===================================================

:: ===================================================
:: Main batch procedure
:: ===================================================

:: Register all keys
REG ADD %HKEY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v Driver /t REG_SZ /d %DRIVER_PATH%
REG ADD %HKEY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v DESCRIPTION /t REG_SZ /d %DESCRIPTION%
REG ADD %HKEY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v SERVER /t REG_SZ /d %SERVER%
REG ADD %HKEY%\SOFTWARE\ODBC\ODBC.INI\%SOURCE_NAME% /f /v DATABASE /t REG_SZ /d %DATABASE_NAME%
REG ADD "%HKEY%\SOFTWARE\ODBC\ODBC.INI\ODBC Data Sources" /f /v %SOURCE_NAME% /t REG_SZ /d %DRIVER_DESCRIPTION%

:: ===================================================
:: End of main batch procedure
:: ===================================================

