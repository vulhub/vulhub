<?php
$data = file_get_contents('php://input');
$xml = simplexml_load_string($data);

echo $xml->name;