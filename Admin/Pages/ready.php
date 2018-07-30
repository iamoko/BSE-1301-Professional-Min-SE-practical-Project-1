<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<link rel="stylesheet" type="text/css" href="css/util.css">
    <link rel="stylesheet" type="text/css" href="css/main.css"> 
    <link rel="stylesheet" type="text/css" href="css/Another.css">
	<title>Ready Jobs</title>
	<style type="text/css">

	body{
	  font: 18px normal sans-serif;
	  background: url("bg.jpg");
  }
    #minor{
  background-color: rgb(30,30,30);
    }
  </style>
</head>
	<body>


           <div class="table100 ver2 m-b-110">
          <table data-vertable="ver2">
            <thead>
              <tr class="row100 head">
                <th class="column100" id="minor" colspan="9">Ready Jobs</th>
               </tr>
              <tr class="row100 head">
                <th class="column100 column1" id="minor" data-column="column1">Client ID</th>
                <th class="column100 column2" id="minor" data-column="column2">Time</th>
                <th class="column100 column3" id="minor" data-column="column3">Date</th>
                <th class="column100 column4" id="minor" data-column="column4">Job ID</th>
                <th class="column100 column5" id="minor" data-column="column5">Processing Duration</th>
                <th class="column100 column6" id="minor" data-column="column6">Task Type</th>
                <th class="column100 column7" id="minor" data-column="column7">String</th>
                <th class="column100 column8" id="minor" data-column="column8">Result</th>
                <th class="column100 column9" id="minor" data-column="column9">Priority</th>
              </tr>
            </thead>
            <tbody>
              
              <?php 
                $connection = mysqli_connect("localhost", "root", "4826");
                mysqli_select_db($connection, "recess");
                $command = "SELECT * FROM Entry;";
                if(@$nuumber = mysqli_query($connection, $command)){
                  
                }else{
                  echo "one";
                }
                $counter = 0;
                while($record = mysqli_fetch_array($nuumber)){
               
              ?>
              <tr class="row100">
                <td class="column100 column1" data-column="column1"> 
                <?php  echo $record['ID']; ?></td>
                <td class="column100 column2" data-column="column2">
                  <?php  echo $record['Time']; ?>
                </td>
                <td class="column100 column3" data-column="column3">
                  <?php  echo $record['Date']; ?>
                </td>
                <td class="column100 column4" data-column="column4">
                  <?php  echo $record['Job ID']; ?>
                </td>
                <td class="column100 column5" data-column="column5">
                  <?php  echo $record['Processing Duration']; ?>
                </td>
                <td class="column100 column6" data-column="column6">
                  <?php  echo $record['Task Type']; ?>
                </td>
                <td class="column100 column7" data-column="column7">
                  <?php  echo $record['String']; ?>
                </td>
                <td class="column100 column8" data-column="column8">
                  <?php  echo $record['Result']; ?>
                </td>
                <td class="column100 column9" data-column="column9">
                  <?php  echo $record['Priority']; ?>
                </td>
              </tr>
              <?php
              $counter++;
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