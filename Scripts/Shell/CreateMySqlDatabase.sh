@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : CreateDatabase.bat
REM
REM Description : Database installation helper
REM
REM Project     : ARIA
REM
REM *************************************************************************
REM Contributors :
REM    Version 1.00: 12/06/2013 15:20:51 AM, Author: flenouail, Original hack  
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

CD %~dp0..\SQLScripts\

ECHO Drop %db% database
CALL :EXECUTE_QUERY %user% %pwd% "drop schema if exists %db%"

ECHO Creating %db% database
CALL :EXECUTE_QUERY %user% %pwd% "create database if not exists %db%"
CALL :EXECUTE_QUERY %user% %pwd% "use mysql"

ECHO Create users
CALL :EXECUTE_QUERY %user% %pwd% "GRANT ALL PRIVILEGES ON %db%.* TO 'ariaUser'@'localhost' IDENTIFIED BY 'ariaPwd' WITH GRANT OPTION"
CALL :EXECUTE_QUERY %user% %pwd% "GRANT ALL PRIVILEGES ON %db%.* TO 'ariaUser'@'%' IDENTIFIED BY 'ariaPwd' WITH GRANT OPTION"

ECHO Create tables
CALL :EXECUTE_DATABASE_QUERY_FROM_SOURCE %user% %pwd% %db% ariaCreateTables

ECHO Create store procedures
CALL :EXECUTE_DATABASE_QUERY_FROM_SOURCE %user% %pwd% %db% ariaCreateStoreProcedures

CD %~dp0

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
ECHO         Install
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