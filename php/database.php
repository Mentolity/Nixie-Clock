<?php
	$dbHost = "localhost";
	$dbUserName = "root";
	$dbPassword = ""; //1flyer
	$dbName = "nixieData";

		@mysql_connect("$dbHost","$dbUserName","$dbPassword") or die ("Could not connect to MySQL");
		@mysql_connect("$dbName") or die ("No database");
		echo"Successful Connection";
?>