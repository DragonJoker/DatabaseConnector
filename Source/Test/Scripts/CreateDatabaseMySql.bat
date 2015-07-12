@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : CreateDatabase.bat
REM
REM Description : Database installation helper
REM
REM *************************************************************************
REM Contributors :
REM    Version 1.00: 12/06/2013 15:20:51 AM, Author: flenouail, Original hack  
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
CALL :EXECUTE_QUERY %user% %pwd% "DROP DATABASE IF EXISTS %db%"

ECHO Creating %db% database
CALL :EXECUTE_QUERY %user% %pwd% "CREATE DATABASE IF NOT EXISTS %db% CHARACTER SET utf8 COLLATE utf8_general_ci"

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