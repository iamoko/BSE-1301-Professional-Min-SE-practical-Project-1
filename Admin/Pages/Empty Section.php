<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Waiting Jobs with priority</title>
	<style type="text/css">
		body{
	  font: 18px normal sans-serif;
	  background: url("bg.jpg");
		}.des{
			position: absolute;
			top: 40px;
			left: 40px;
			border: 1px solid #000;
			background-color: rgba(40,40,40,0.6);
			height: 150px;
			width: 390px;
			padding-left: 15px;
			color: #fff;
			letter-spacing: 1px;
			border-radius: 5px;
		}.des1{
			position: absolute;
			top: 40px;
			left: 490px;
			border: 1px solid #000;
			background-color: rgba(40,40,40,0.6);
			height: 150px;
			width: 210px;
			padding-left: 15px;
			color: #fff;
			letter-spacing: 1px;
			border-radius: 5px;
		}.des2{
			position: absolute;
			top: 40px;
			left: 760px;
			border: 1px solid #000;
			background-color: rgba(40,40,40,0.6);
			height: 150px;
			width: 190px;
			padding-left: 15px;
			color: #fff;
			letter-spacing: 1px;
			border-radius: 5px;
		}.low{
			width: 110px;
			height: 350px;
			position: absolute;
			left: 100px;
			background: rgba(0,0,256,0.3);
		}.high{
			width: 140px;
			height: 70px;
			position: absolute;
			left: 210px;
			background: rgba(0,0,256,0.3);
		}.high1{
			width: 140px;
			height: 70px;
			position: absolute;
			left: 210px;
			top: 280px;
			background: rgba(0,0,256,0.3);
		}.main{
			position: absolute;
			top: 230px;
			left: 15%;
			border-radius: 8px;
		}h1{
			position: absolute;
			left: 220px;
			color: #fff;
			top: 80px;
			font-size: 50px;
		}
  </style>
</head>
	<body>
	<div class="des"> 
		<h3>Average Rate at which Server Processes Jobs:</h3>
	<?php 
        $connection = mysqli_connect("localhost", "root", "4826");
        mysqli_select_db($connection, "recess");
        $sql = "SELECT sum(`Processing Duration`)/count(`Processing Duration`) as average FROM `Entry`";
        $nuumber = mysqli_query($connection, $sql);
        while($record = mysqli_fetch_array($nuumber)){
        	echo $record['average']."<br>Seconds";
        }
        
	?>
	</div>
	<div class="des1">
		<h3>Processed Jobs</h3><br>
		<?php
		$sql1 = "SELECT count(`Processing Duration`) as total FROM `Entry`";
        $nuumber1 = mysqli_query($connection, $sql1);
        while($record1 = mysqli_fetch_array($nuumber1)){
        	echo $record1['total']."<br>Jobs";
        }

		?>
	</div>

	<div class="des2">
		<h3>Unprocessed Jobs</h3>
		<?php

		$linecount = -1;
		$handle = fopen("/var/www/html/Recess/waiting.txt" , "r");
		while(!feof($handle)){
			$line = fgets($handle);
			$linecount++;
		}
		fclose($handle);
		$linecount1 = -1;
		$handle1 = fopen("/var/www/html/Recess/busy_list.txt" , "r");
		while(!feof($handle1)){
			$line1 = fgets($handle1);
			$linecount1++;
		}
		fclose($handle1);
		echo $linecount1 + $linecount."<br>Jobs";
		?>
	</div>
	<br><br>
	<div class="main">
		<div class="low"></div>
		<div class="high"></div>
		<div class="high1"></div>
		<div><h1>Welcome <br>Administrator</h1></div>
	</div>
	
	</body>
	

</html>