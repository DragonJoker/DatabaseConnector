@ECHO OFF
REM *******************************<+>***************************************
REM
REM File        : InstallStkUnit.bat
REM
REM Description : Stk unit tests installation
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
SET stkUnitDir=Utility\stk_unit\sql
SET sqlScriptsDir=DataBase\SQLScripts
SET unitTestsDir=DataBase\SQLScripts\UnitTests
SET batchDir=DataBase\BatchFiles

CD ..\..\

ECHO Install Stk Unit
CALL :EXECUTE_QUERY_FROM_SOURCE %user% %pwd% %stkUnitDir%\stk_unit.sql

ECHO Install Aria Test Case Attributes Sp
CALL :EXECUTE_QUERY_FROM_SOURCE %user% %pwd% %unitTestsDir%\case\ariaTest_Attributes_Sp.sql

ECHO Install Aria Test Suite
CALL :EXECUTE_QUERY_FROM_SOURCE %user% %pwd% %unitTestsDir%\suite\ariaUnitTests.sql

CD %batchDir%

PAUSE

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
mysql --user=%1 --password=%2 --database=%3 < %3.SQL
IF errorlevel 1 (@ECHO error)
GOTO:EOF

:: ===================================================
:: End Of Subroutines
:: ===================================================

:END