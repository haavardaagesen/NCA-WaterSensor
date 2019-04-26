<?php
$servername = "";
$username = "";
$password = "";
$dbname = "";

$uid = $_GET["uid"];
$flow = $_GET["f"];
$lat = $_GET["la"];
$long = $_GET["lo"];
$locName = $_GET["na"];
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}


$sql = "INSERT INTO waterflow(uid,lat,locName,flow,lo) VALUES ('$uid','$lat','$locName','$flow','$long')";

if ($conn->query($sql) === TRUE){
	echo "New record successfull";
}
else{
	echo "Error:".$sql."<br>".$conn->error;
}
$conn->close();
?>

