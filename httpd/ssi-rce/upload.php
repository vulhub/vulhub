<?php
if (!empty($_FILES)):
$ext = pathinfo($_FILES['file_upload']['name'], PATHINFO_EXTENSION);
if (in_array($ext, ['php'])) {
    die('Unsupported filetype uploaded.');
}

move_uploaded_file($_FILES['file_upload']['tmp_name'], './' . $_FILES['file_upload']['name']);
echo "<a href='/{$_FILES['file_upload']['name']}'>{$_FILES['file_upload']['name']}</a>";

endif;
?>
<form method="post" enctype="multipart/form-data">
    File: <input type="file" name="file_upload">
    <input type="submit">
</form>
