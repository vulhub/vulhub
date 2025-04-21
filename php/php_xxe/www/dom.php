<?php
$data = file_get_contents('php://input');

$dom = new DOMDocument();
$dom->loadXML($data);

print_r($dom);