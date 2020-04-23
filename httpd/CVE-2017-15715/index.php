<?php
if(isset($_FILES['file'])) {
    $name = basename($_POST['name']);
    $ext = pathinfo($name,PATHINFO_EXTENSION);
    if(in_array($ext, ['php', 'php3', 'php4', 'php5', 'phtml', 'pht'])) {
        exit('bad file');
    }
    move_uploaded_file($_FILES['file']['tmp_name'], './' . $name);
} else {

?>

<!DOCTYPE html>
<html>
<head>
	<title>Upload</title>
</head>
<body>
<form method="POST" enctype="multipart/form-data">
	<p>
		<label>file:<input type="file" name="file"></label>
	</p>
	<p>
		<label>filename:<input type="text" name="name" value="evil.php"></label>
	</p>
	<input type="submit">
</form>
</body>
</html>

<?php
}
?>
