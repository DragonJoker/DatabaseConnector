#!/bin/sh
# *******************************<+>***************************************
#
# File        : DeleteDatabase.bat
#
# Description : Database uninstallation helper
#
# *************************************************************************
# Contributors :
#    Version 1.00: 07/02/2013 15:20:51 AM, Author: smichelizza, Original hack  
#
# *********************************<+>*************************************

execute_query()
{
	mysql --user=$1 --password=$2 --execute=$3
}

execute_query_from_source()
{
	mysql --user=$1 --password=$2 < $3
}

execute_database_query_from_source()
{
	mysql --user=$1 --password=$2 --database=$3 < $4.SQL
}

# Initialize variables
if ("$1" <> "")
	db=$1
else
	db=db_test_odbc_mysql
fi

if ("$2" <> "")
	user=$2
else
	user=test_user
fi

if ("$3" <> "")
	pwd=$3
else
	pwd=test_pwd
fi

echo Drop $db database
execute_query $user $pwd "DROP DATABASE IF EXISTS $db"

echo
echo
echo ------------------------------------------------------
echo ########## Databases Uninstallation finished! ##########
echo ------------------------------------------------------
echo
