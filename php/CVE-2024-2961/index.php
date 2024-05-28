<?php    
$data = file_get_contents($_POST['file']);
echo "File contents: $data";
