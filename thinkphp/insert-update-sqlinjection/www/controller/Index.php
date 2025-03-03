<?php
namespace app\index\controller;

class Index
{
	public function index()
	{
        $username = input('get.username/a');
        $res = db('user')->where(['id'=> 1])->insert(['username'=>$username]);
        echo '添加用户成功!'."\n";
        var_dump($res);
	}
}
