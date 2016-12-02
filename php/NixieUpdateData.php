<?php
	$dbHost = "localhost";
	$dbUserName = "root";
	$dbPassword = ""; //1flyer
	$dbName = "nixieData";

		$link = mysql_connect("$dbHost","$dbUserName","$dbPassword") or die ('Could not connect to MySQL: ' . mysql_error());
		mysql_select_db("$dbName", $link) or die ('Could not connect to database: ' . mysql_error());
		
		$value=$_POST['input1'];
		
		mysql_query("INSERT INTO functions (whichEnable) VALUES ('$value')") 
		or die ('Could not send query: ' . mysql_error());
		
		mysql_query("UPDATE functions SET whichEnable='$whichEnable', timeSet='$timeSet', update='1' WHERE id='1'")
		or die ('Could not send query: ' . mysql_error());
		
		echo"Successful Connection";
		mysql_close();
?>