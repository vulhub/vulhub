<?php

require __DIR__ . '/vendor/autoload.php';

use GuzzleHttp\Client;

header('Content-Type: application/json; charset=utf-8');

$client = new Client([
    // Base URI is used with relative requests
    'base_uri' => 'http://httpbin.org',
    // You can set any number of default request options.
    'timeout'  => 2.0,
]);

$response = $client->get('http://httpbin.org/get');

$body = $response->getBody();

echo $body;