<?php
if(!empty($_FILES)) {
    $filename = escapeshellarg($_FILES['file']['tmp_name']);
    $newname = '/tmp/' . uniqid() . '.mp4';
    shell_exec("ffmpeg -i $filename $newname");

    $data = base64_encode(file_get_contents($newname));
}
?>
<html>
 <head>
  <meta charset="utf-8">
  <title>Video Player</title>
 </head>
 <body>
     <?php if(!empty($_FILES)): ?>
     <div>
        <video src="data:video/mp4;base64,<?=$data?>" controls="controls" width="640" height="480"></video>
     </div>
     <?php endif; ?>
     <form method="post" enctype="multipart/form-data">
       <input type="file" name="file">
       <input type="submit" value="Submit">
     </form>
 </body>
</html>