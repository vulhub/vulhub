<?php
namespace app\index\controller;

use app\index\model\User;

class Index
{
    public function index()
    {
        $ids = input('ids/a');
        $t = new User();
        $result = $t->where('id', 'in', $ids)->select();
        foreach($result as $row) {
            echo "<p>Hello, {$row['username']}</p>";
        }
    }
}
