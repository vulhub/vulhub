<?php
if (!empty($_FILES)):

// Check for errors
if($_FILES['file_upload']['error'] > 0){
    die('An error ocurred when uploading.');
}

// Check filesize
if(!is_uploaded_file($_FILES['file_upload']['tmp_name'])) {
    die('File is not uploaded file');
}

$ext = pathinfo($_FILES['file_upload']['name'], PATHINFO_EXTENSION);
if (empty($ext) || in_array($ext, ['php', 'php3', 'php5', 'phtml'])) {
    die('Unsupported filetype uploaded.');
}

$new_name = __DIR__ . '/uploadfiles/' . $_FILES['file_upload']['name'];
if(!move_uploaded_file($_FILES['file_upload']['tmp_name'], $new_name)){
    die('Error uploading file - check destination is writeable.');
}

die('File uploaded successfully: ' . $new_name);

else:
?>
<form method="post" enctype="multipart/form-data">
    File: <input type="file" name="file_upload">
    <input type="submit">
</form>
<?php
endif;