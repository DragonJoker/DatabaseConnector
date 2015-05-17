@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : CreateDatabase.bat
REM
REM Description : Database installation helper
REM
REM *********************************<+>*************************************

:: Initialize variables
SET user=DragonJoker
SET pwd=b1cr0sS

IF "%1" NEQ "" (
    SET db=%1
) ELSE (
    SET db=db_test
)

CD %~dp0..\SQL\

ECHO Drop %db% database
CALL :EXECUTE_QUERY %user% %pwd% "drop database if exists %db%"

ECHO Creating %db% database
CALL :EXECUTE_QUERY %user% %pwd% "create database if not exists %db%"
CALL :EXECUTE_QUERY %user% %pwd% "use %db%"

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
osql -U=%1 --password=%2 --execute=%3
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