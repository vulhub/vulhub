<?php

if (!empty($_FILES)):

// Check for errors
if($_FILES['file_upload']['error'] > 0){
    die('An error ocurred when uploading.');
}

if(!getimagesize($_FILES['file_upload']['tmp_name'])){
    die('Please ensure you are uploading an image.');
}

// Check filetype
if(stripos($_FILES['file_upload']['type'], 'image/') !== 0){
    die('Unsupported filetype uploaded.');
}

// Check filesize
if($_FILES['file_upload']['size'] > 500000){
    die('File uploaded exceeds maximum upload size.');
}

// Check filesize
if(!is_uploaded_file($_FILES['file_upload']['tmp_name'])) {
    die('File is not uploaded file');
}

$ext = pathinfo($_FILES['file_upload']['name'], PATHINFO_EXTENSION);
if (!in_array($ext, ['gif', 'png', 'jpg', 'jpeg'])) {
    die('Unsupported filetype uploaded.');
}


$new_name = __DIR__ . '/uploadfiles/' . md5($_FILES['file_upload']['name']) . ".{$ext}";
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