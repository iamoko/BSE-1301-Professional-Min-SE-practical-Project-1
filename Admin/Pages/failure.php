<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<link rel="stylesheet" type="text/css" href="css/util.css">
    <link rel="stylesheet" type="text/css" href="css/main.css"> 
    <link rel="stylesheet" type="text/css" href="css/Another.css">
	<title>Waiting Jobs with priority</title>
	<style type="text/css">
		body{
		  font: 18px normal sans-serif;
		  background: url("bg.jpg");
		  margin: 0;
		  overflow-x:hidden;
		}
    #minor{
  background-color: rgb(30,30,30);
    }
  </style>
</head>
	<body>

		<div id="another">
        <div class="table100 ver2 m-b-110">
        <table data-vertable="ver2">
            <thead>
            <tr class="row100 head">
                <th class="column100" id="minor" colspan="9">Jobs With High Failure Rate</th>
               </tr>
              <tr class="row100 head">
                <th class="column100 column1" id="minor" data-column="column1">Client ID</th>
                <th class="column100 column2" id="minor" data-column="column2">Percentage (%)</th>
              </tr>
            </thead>
            <tbody>
              
              <?php
              //Counts the lines in Busy_list
              $count = 0;
              $line = file("/var/www/html/Recess/ready_jobs.txt");
              $top200 = array_slice(($line),0,1000);
              foreach($top200 as $line)
              { 
                  $count++;
              }

              $total = $count/9;
              //Counts the lines in waiting list

              

              $counter = 0;
              $liner = file("/var/www/html/Recess/waiting.txt");
              $topr200 = array_slice(($liner),0,100);
              foreach($topr200 as $liner)
              {
                  $counter++;
              }

              $number = 0;
              $lines = file("/var/www/html/Recess/busy_list.txt");
              $tops200 = array_slice(($lines),0,8);
              foreach($tops200 as $lines)
              { 
                $pieces = explode("\t", $lines);
                $number++;
              }
              $another1 = $number + $counter;
                ?>
                <tr class="row100">
                  <td class="column100 column1" data-column="column1">
                    <?php echo $pieces[0]; ?>
                  </td>
                  <td class="column100 column2" data-column="column2">
                    <?php echo ($number/$another1) * 100; ?>
                  </td>
                
            </tbody>
          </table>
          </div>

        </div>

	</body>
	<!--===============================================================================================-->  
  <script src="vendor/jquery/jquery-3.2.1.min.js"></script>
<!--===============================================================================================-->
  <script src="vendor/bootstrap/js/popper.js"></script>
  <script src="vendor/bootstrap/js/bootstrap.min.js"></script>
<!--===============================================================================================-->
  <script src="vendor/select2/select2.min.js"></script>
<!--===============================================================================================-->
  <script src="js/main.js"></script>

</html>