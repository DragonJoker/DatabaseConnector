@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : DeleteDatabase.bat
REM
REM Description : Database uninstallation helper
REM
REM *************************************************************************
REM Contributors :
REM    Version 1.00: 07/02/2013 15:20:51 AM, Author: smichelizza, Original hack  
REM
REM *********************************<+>*************************************

:: Initialize variables
IF "%1" NEQ "" (
	SET mysql_command=%1
) ELSE (
	SET mysql_command=mysql
)

IF "%2" NEQ "" (
	SET db=%2
) ELSE (
	SET db=db_test_odbc_mysql
)

IF "%3" NEQ "" (
	SET user=%3
) ELSE (
	SET user=test_user
)

IF "%4" NEQ "" (
	SET pwd=%4
) ELSE (
	SET pwd=test_pwd
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
%mysql_command% --user=%1 --password=%2 --execute=%3
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:EXECUTE_QUERY_FROM_SOURCE
%mysql_command% --user=%1 --password=%2 < %3
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:EXECUTE_DATABASE_QUERY_FROM_SOURCE
%mysql_command% --user=%1 --password=%2 --database=%3 < %4.SQL
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:: ===================================================
:: End Of Subroutines
:: ===================================================

:END