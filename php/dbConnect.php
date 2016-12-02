<?php
	$dbHost = "localhost";
	$dbUserName = "root";
	$dbPassword = ""; //1flyer
	$dbName = "nixieData";

		$link = mysql_connect("$dbHost","$dbUserName","$dbPassword") 
		or die ('Could not connect to MySQL: ' . mysql_error());
		
		mysql_select_db("$dbName", $link) 
		or die ('Could not connect to database: ' . mysql_error());

		echo"Successful Connection";
?>