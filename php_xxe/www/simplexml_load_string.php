<?php
$data = file_get_contents('php://input');
$xml = simplexml_load_string($data);

print_r($xml);