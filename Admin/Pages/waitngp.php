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
                <th class="column100" id="minor" colspan="9">Waiting Jobs With Priority</th>
               </tr>
              <tr class="row100 head">
                <th class="column100 column1" id="minor" data-column="column1">Priority</th>
                <th class="column100 column2" id="minor" data-column="column2">Client ID</th>
                <th class="column100 column3" id="minor" data-column="column3">Task Type</th>
                <th class="column100 column4" id="minor" data-column="column4">String</th>
              </tr>
            </thead>
            <tbody>
            
              <?php
              $count = 0;
              $line = file("/var/www/html/Recess/waiting.txt");
              $top200 = array_slice(($line),0,100);
              foreach($top200 as $line)
              { 
                ?>
               <tr class="row100">
              <?php
                  $pieces = explode("\t", $line);?>
                  <td class="column100 column1" data-column="column1">
                    <?php echo $pieces[0]; ?>
                  </td>
                  <td class="column100 column2" data-column="column2">
                    <?php echo $pieces[1]; ?>
                  </td>
                  <td class="column100 column3" data-column="column3">
                    <?php echo $pieces[2]; ?>
                  </td>
                  <td class="column100 column4" data-column="column4">
                    <?php echo $pieces[3]; ?>
                  </td>
                  </tr>
              <?php 
                  $count++;
              }
              ?>
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