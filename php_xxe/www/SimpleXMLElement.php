<?php
$data = file_get_contents('php://input');
$xml = new SimpleXMLElement($data);

print_r($xml);