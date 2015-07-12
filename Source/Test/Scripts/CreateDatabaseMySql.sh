#!/bin/sh
# *******************************<+>***************************************
#
# File        : CreateDatabase.bat
#
# Description : Database installation helper
#
# *************************************************************************
# Contributors :
#    Version 1.00: 12/06/2013 15:20:51 AM, Author: flenouail, Original hack  
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
if "$1" <> ""
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

echo Creating $db database
execute_query $user $pwd "CREATE DATABASE IF NOT EXISTS $db CHARACTER SET utf8 COLLATE utf8_general_ci"

echo
echo
echo ------------------------------------------------------
echo ########## Databases Installation finished! ##########
echo ------------------------------------------------------
echo
