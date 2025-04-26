<?php
$newname = '';
if (!empty($_FILES)) {
    $ext = pathinfo($_FILES['file_upload']['name'], PATHINFO_EXTENSION);
    if (!in_array($ext, ['gif', 'png', 'jpg', 'jpeg'])) {
        die('Unsupported filetype uploaded.');
    }

    $newname = uniqid() . '.png';
    shell_exec("convert -resize 50x50 {$_FILES['file_upload']['tmp_name']} ./{$newname}");
}
?>
<form method="post" enctype="multipart/form-data">
    File: <input type="file" name="file_upload">
    <input type="submit">
</form>
<br>
<?php
if ($newname):
?>
<h1>Your image:</h1>
<p>
    <a href="./<?=$newname?>" target="_blank">
        <img src="./<?=$newname?>" width="50" height="50">
    </a>
</p>
<?php
endif;