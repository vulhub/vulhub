<?php
header("Content-Type: text/html; charset=utf-8");
echo "Hello, \n";
echo "Your name is <strong>" . (isset($_GET['name']) ? $_GET['name'] : 'Vulhub') . '</strong>';