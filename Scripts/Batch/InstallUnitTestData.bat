@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : InstallUnitTestData.bat
REM
REM Description : Database installation helper
REM
REM Project     : ARIA
REM
REM *************************************************************************
REM Contributors :
REM    Version 1.00: 17/06/2013 15:20:51 AM, Author: ezeyen, Original hack  
REM
REM *************************************************************************
REM (c) Copyright 2013 AREVA NDE SOLUTIONS - FRANCE - INTERCONTROLE
REM     All Rights Reserved.
REM *********************************<+>*************************************

:: Initialize variables
SET user=root
SET pwd=JDB

if not [%2]==[] (
    goto :USAGE
)

if [%1]==[] (
    goto :USAGE
)

SET script=%1

ECHO Executing %script% script
CALL :EXECUTE_QUERY_FROM_SOURCE %user% %pwd% %script%

ECHO.
ECHO.
ECHO ------------------------------------------------------
ECHO ########## Databases Installation finished! ##########
ECHO ------------------------------------------------------
ECHO.
GOTO END

:USAGE
ECHO -------------------------------------------------------
ECHO Missing argument
ECHO.
ECHO USAGE:
ECHO         Install Script_name
ECHO -------------------------------------------------------
GOTO END

:ERROR
ECHO.
ECHO ------------------------------------------------------
ECHO                !!! ERROR occured. !!!
ECHO ------------------------------------------------------
ECHO.
GOTO END

:: ===================================================
:: End of main batch procedure
:: ===================================================

:: ===================================================
:: Subroutines
:: ===================================================

:EXECUTE_QUERY_FROM_SOURCE
dir %3.sql > NUL
IF errorlevel 1 (
EXIT 1
)
mysql --user=%1 --password=%2 < %3.sql
IF errorlevel 1 (
EXIT 1
)
GOTO:EOF

:: ===================================================
:: End Of Subroutines
:: ===================================================

:END