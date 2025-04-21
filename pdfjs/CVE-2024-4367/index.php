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
if ($ext !== 'pdf') {
    die('Unsupported filetype uploaded.');
}

$new_name = uniqid() . '.pdf';
if(!move_uploaded_file($_FILES['file_upload']['tmp_name'], __DIR__ . '/upload/' . $new_name)){
    die('Error uploading file - check destination is writeable.');
}

header('Location: /web/viewer.html?file=../upload/' . $new_name, true, 302);
exit;
else:
?>
<form method="post" enctype="multipart/form-data">
    File: <input type="file" name="file_upload">
    <input type="submit">
</form>
<?php
endif;