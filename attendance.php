<html>
<body>

<?php


header("Content-Type: text/html;charset=utf-8");
$dbms = 'mysql';
$db_host = 'localhost';
$db_name = 'xinchejian';
$db_user = 'root';
$db_password = 'root';

$dsn="$dbms:host=$db_host;dbname=$db_name";
$dbh = new PDO('mysql:host=localhost;dbname=xinchejian', 'root', 'root');
$staff_php_name = $_GET['staff_name'];
$staff_php_id = $_GET['staff_id'];
$result = $dbh->query("SELECT * FROM staff_authorization WHERE staff_id=$staff_php_id AND NoEntry='0' limit 1");
$row = $result->fetchAll(PDO::FETCH_COLUMN, 1);

if ($row){
	echo "state:success";
	$stmt = $dbh->exec("INSERT INTO staff_attendance (staff_db_id,staff_db_name,staff_db_time) VALUES ('$staff_php_id','$staff_php_name',now())");
}
else{
	echo "state:fail";
}


//echo $_GET['staff_name'];

//echo $_GET['staff_id'];



?>

</body>
</html>