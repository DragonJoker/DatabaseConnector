#!/bin/sh
# *******************************<+>***************************************
#
# File        : Uninstall_odbc_aria.bat
#
# Description : Uninstall an ODBC source for accessing to an ARIA database.
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
HKEY=CURRENT_USER

# ===================================================
# End of variables
# ===================================================

# ===================================================
# Main batch procedure
# ===================================================

apply_deletion()
{
	# Check existing key
	REG QUERY "$HKEY\\Software\\ODBC\\ODBC.INI\\ODBC Data Sources" /v $SOURCE_NAME
	# Unregister all keys
	REG DELETE "$HKEY\\Software\\ODBC\\ODBC.INI\\$SOURCE_NAME" /f
	REG DELETE "$HKEY\\Software\\ODBC\\ODBC.INI\\ODBC Data Sources" /v $SOURCE_NAME /f
}

# Determine the the registry key (LOCAL_MACHINE needs administrator rights, CURRENT_USER doesn't)
if '$HKEY'=='LOCAL_MACHINE'
	apply_deletion HKEY_LOCAL_MACHINE
elif '$HKEY'=='CURRENT_USER'
	apply_deletion HKEY_CURRENT_USER
else
    echo "HKEY not valid! Exit."
fi

# ===================================================
# End of main batch procedure
# ===================================================
