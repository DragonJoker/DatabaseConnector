@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : DeleteDatabase.bat
REM
REM Description : Database uninstallation helper
REM
REM Project     : ARIA
REM
REM *************************************************************************
REM Contributors :
REM    Version 1.00: 07/02/2013 15:20:51 AM, Author: smichelizza, Original hack  
REM
REM *************************************************************************
REM (c) Copyright 2013 AREVA NDE SOLUTIONS - FRANCE - INTERCONTROLE
REM     All Rights Reserved.
REM *********************************<+>*************************************

:: Initialize variables
SET user=root
SET pwd=JDB
IF "%1" NEQ "" (
    SET db=%1
) ELSE (
    SET db=db_aria
)

ECHO Drop %db% database
CALL :EXECUTE_QUERY %user% %pwd% "drop schema if exists %db%"

ECHO.
ECHO.
ECHO ------------------------------------------------------
ECHO ########## Databases Uninstallation finished! ##########
ECHO ------------------------------------------------------
ECHO.
GOTO END

:USAGE
ECHO -------------------------------------------------------
ECHO Missing argument
ECHO.
ECHO USAGE:
ECHO         Uninstall
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

:EXECUTE_QUERY
mysql --user=%1 --password=%2 --execute=%3
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:EXECUTE_QUERY_FROM_SOURCE
mysql --user=%1 --password=%2 < %3
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:EXECUTE_DATABASE_QUERY_FROM_SOURCE
mysql --user=%1 --password=%2 --database=%3 < %4.SQL
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:: ===================================================
:: End Of Subroutines
:: ===================================================

:END