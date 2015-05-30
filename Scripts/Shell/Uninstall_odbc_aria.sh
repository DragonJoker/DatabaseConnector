@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : Uninstall_odbc_aria.bat
REM
REM Description : Uninstall an ODBC source for accessing to an ARIA database.
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

CALL %~dp0Uninstall_odbc.bat %SOURCE_NAME% %HKEY%

:: ===================================================
:: End of main batch procedure
:: ===================================================
