<!DOCTYPE html>
<html>
<head>
	<title>Read and store data in Database</title>
	<script type="text/javascript">
		setTimeout(function() {
		  location.reload();
		}, 60000);
	</script> 
</head>
<body>

<?php

$ht = "localhost"; 
$u = "root"; 
$pwd = "4826";
$database="recess";

$connection = mysqli_connect($ht,$u, $pwd);

$count = 0;
$line = file("ready_jobs.txt");
$top200 = array_slice(($line),0,9);
foreach($top200 as $line)
{	
    echo $line . "<br />";
    $count++;
   	if($count == 1){
       	$ID = $line;
    }else if($count == 2){
        $Timer = $line;
    }else if($count == 3){
        $Date = $line;
    }else if($count == 4){
        $Job_Id = $line;
    }else if($count == 5){
        $Result = $line;
    }else if($count == 6){
        $Type = $line;
    }else if($count == 7){
    	$strings = $line;
    }else if($count == 8){
    	$Duration = $line;
    }else if($count == 9){
    	$priority = $line;
    	break;
    }
}
if(@mysqli_connect($ht, $u, $pwd)){
	mysqli_select_db($connection, $database);
	if(!empty($ID) && !empty($Date)){
		$sql = "INSERT INTO `Entry`(`ID`, `Time`, `Date`, `Job ID`, `Task Type`, `String`, `Result`, `Processing Duration`, `Priority`) VALUES ('$ID',
		'$Timer',
		'$Date',
		'$Job_Id',
		'$Result',
		'$Type',
		'$strings',
		'$Duration',
		'$priority')";

		mysqli_query($connection, $sql);
		echo "<h4>Data Has been Successfully Uploaded to the Database</h4>";
		
		
		$lines = file("ready_jobs.txt");
		$first_line = "";
		$lines = array_slice($lines,9);
		$lines = array_merge(array($first_line, ""), $lines);

		// Write to file
		$file = fopen("ready_jobs.txt", 'w');
		fwrite($file, implode('', $lines));
		fclose($file);


		
	}else{
		echo "The File is Empty";
	}

}else{
	echo "Failed to connect";
}
mysqli_close($connection);
?>

</body>
</html>
