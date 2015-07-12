#!/bin/sh
# *******************************<+>***************************************
#
# File        : Install_odbc_aria.bat
#
# Description : Install an ODBC source for accessing to an ARIA database.
#
# *************************************************************************
# Contributors :
#    Version 1.00: 06/30/2013 2:44:32 PM, Author: ezeyen, Original hack  
#
# *********************************<+>*************************************

# ===================================================
# Variables
# ===================================================

SOURCE_NAME=$1
SERVER=127.0.0.1
DATABASE_NAME=$1
PORT=3306
USER=test_user
PASSWORD=test_pwd
DRIVER_PATH=C:/Program Files/MySQL/Connector ODBC 5.3/myodbc5w.dll
DRIVER_DESCRIPTION=MySQL ODBC 5.3 Unicode Driver
DESCRIPTION=$1
HKEY=CURRENT_USER

# ===================================================
# End of variables
# ===================================================

# ===================================================
# Main batch procedure
# ===================================================

apply_creation()
{
	# Set common keys
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v Driver /t REG_SZ /d $DRIVER_PATH
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v DESCRIPTION /t REG_SZ /d $DESCRIPTION
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v SERVER /t REG_SZ /d $SERVER
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v DATABASE /t REG_SZ /d $DATABASE_NAME
	REG ADD "$1\\SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources" /f /v $SOURCE_NAME /t REG_SZ /d $DRIVER_DESCRIPTION

	# Set specific keys
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v UID /t REG_SZ /d $USER
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v PWD /t REG_SZ /d $PASSWORD
	REG ADD $1\\SOFTWARE\\ODBC\\ODBC.INI\\$SOURCE_NAME /f /v PORT /t REG_SZ /d $PORT
}

# Determine the the registry key (LOCAL_MACHINE needs administrator rights, CURRENT_USER doesn't)
if "$HKEY%"=="LOCAL_MACHINE"
    apply_creation HKEY_LOCAL_MACHINE
elif "$HKEY"=="CURRENT_USER"
    apply_creation HKEY_CURRENT_USER
else
    echo "HKEY not valid! Exit."
fi

# ===================================================
# End of main batch procedure
# ===================================================
