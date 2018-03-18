<?php

if (!empty($_FILES)):

$ext = pathinfo($_FILES['file_upload']['name'], PATHINFO_EXTENSION);
if (!in_array($ext, ['gif', 'png', 'jpg', 'jpeg'])) {
    die('Unsupported filetype uploaded.');
}

$i = new Imagick($_FILES['file_upload']['tmp_name']);
$size = $i->getSize();

echo "Image size is: ";
print_r($size);

else:
?>
<form method="post" enctype="multipart/form-data">
    File: <input type="file" name="file_upload">
    <input type="submit">
</form>
<?php
endif;