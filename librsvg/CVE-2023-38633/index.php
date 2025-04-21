<?php
$newname = '';
if (!empty($_FILES)) {
    $ext = pathinfo($_FILES['file_upload']['name'], PATHINFO_EXTENSION);
    if (!in_array($ext, ['svg'])) {
        die('Unsupported filetype uploaded.');
    }

    $newname = uniqid() . '.png';
    shell_exec("rsvg-convert {$_FILES['file_upload']['tmp_name']} > ./{$newname}");
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
        <img src="./<?=$newname?>">
    </a>
</p>
<?php
endif;