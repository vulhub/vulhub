<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: huliangliang
 * Editor: huliangliang
 */
/**
 * * 加载核心文件类和公用方法函数*
 */
require_once(dirname(__FILE__) . "/../core/init.php");
$time_start = helper :: getmicrotime(); //开始时间
//分配内存
ini_set("memory_limit","128M");
set_time_limit(0);
$dbm = new db_mysql(); //数据库类实例
$c = new common($dbm);
//数据库备份类
//$dbkdir = dirname(dirname(__FILE__)).'/backup/appcms_'.date("Y-m-d H:i:s").'/';
$dbkdir =  '.././backup/';


$page['get'] = $_GET; //get参数的 m 和 ajax 参数是默认占用的，一个用来执行动作函数，一个用来判断是否启用模板还是直接输出JSON格式数据
$page['post'] = $_POST;

check_admin(); //判断用户是否登录

/**
 * 页面动作 model 分支选择  
 *          动作函数写在文件末尾，全部以前缀 m__ 开头
 */
$page['get']['m'] = isset($_GET['m'])?$_GET['m']:'list';

if (function_exists("m__" . $page['get']['m'])) {
    call_user_func("m__" . $page['get']['m']);
}

$time_end = helper :: getmicrotime(); //主程序执行时间，如果要包含模板的输出时间，则可以调用该静态时间方法单独计算
$page['data_fill_time'] = $time_end - $time_start;
/**
 * 模板载入选择
 *          模板页面为PHP+HTML混合编写，如果模板页面中也有区块函数，模板函数以 tpl__ 为前缀
 */
if (!isset($page['get']['ajax']) || $page['get']['ajax'] == null || $page['get']['ajax'] == '') {
    $tpl_filename = str_replace('\\', '', str_replace(dirname(__FILE__), '', __FILE__));
    $tpl_filename = str_replace('/', '', $tpl_filename);
    require(dirname(__FILE__) . '/templates/tpl_' . $tpl_filename);
} else {
    if ($page['get']['ajax'] == 'json') {
        echo json_encode($page);
    } 
} 
//获取备份文件
function m__list() {
	global $page;
	$page['list'] = get_tables_name();	
	$page['rows'] = get_hander();
}
//获取备份文件
function get_hander(){
	global $dbkdir;
	$filedb=array();
	if (is_dir($dbkdir) && file_exists($dbkdir) && dirsize($dbkdir) > 0) {
		$filedb['version'] = VERSION;
		$filedb['time'] = date("Y-m-d H:i:s");
		$filedb['dbsize'] = num_bitunit(dirsize($dbkdir));
	}

	return $filedb;
}
/*
*根据传入的目录名计算目录下的文件大小
*/
function dirsize($dirname) {
	$dirsize=0;
	if ($dir_handle=opendir($dirname)) {
		while ($filename = readdir($dir_handle)) {
			$subFile = $dirname.DIRECTORY_SEPARATOR.$filename;
			if ($filename == '.' || $filename == '..') {
				continue;
			} else if (is_dir($subFile)) {
				$dirsize += dirsize($subFile);
			} else if (is_file($subFile)) {
				$dirsize += filesize($subFile);
			}
		}
		closedir($dir_handle);
	}
	return $dirsize;
}

//转换字节单位
function num_bitunit($num) {
	$bitunit=array(' B',' KB',' MB',' GB');
	for($key=0;$key<count($bitunit);$key++){
		if($num>=pow(2,10*$key)-1){ //1023B 会显示为 1KB
		  $num_bitunit_str=(ceil($num/pow(2,10*$key)*100)/100)." $bitunit[$key]";
		}
	}
	return $num_bitunit_str;
}

//读取整个文件内容
function read_msg($file) {
	if ($handle = @fopen($file,"rb")) {
		$file_data = fread($handle,filesize($file));
		fclose($handle);
	}
	return $file_data;
}

//导入备份文件
function m__backin() {
	global $dbm,$dbkdir;
	m__list();
	$act = isset($_GET['act']) ? trim($_GET['act']) : 'recover';
	if ($act == 'recover') {
		if(file_exists($dbkdir . DB_DBNAME . ".sql")) {
			$sql = read_msg($dbkdir . DB_DBNAME . ".sql");
			recover_data($sql);
			$table = get_tables_name();
			$tables = implode(',', $table);
			$selfurl = "dbbak.php?m=backin&act=recover_data&table=" . $tables;
			helper :: redirect($selfurl, 2, '数据表结构已经恢复，正在恢复数据信息！');
		} else {
			die('数据表结构文件不存在!');
		}
	} else if($act == 'recover_data') {
		if(!isset($_GET['table']) && empty($_GET['table'])) die('没有恢复的数据表');
		$table = trim($_GET['table']);
		//每次跳转接收所有的表名
		$tables = $table;
		$table = explode(',',$table);
		//表数组中索引，默认为0
		$tableid = isset($_GET["tableid"]) ? intval($_GET["tableid"]) : 0;
		//分卷id号，默认为0
		$pageid = isset($_GET["pageid"]) ? intval($_GET["pageid"]) : 0;
		//统计表个数
		$table_count = count($table);
		//当前表名在数组中索引值小于表个数
		if ($tableid < $table_count) {
			//检查当前表分卷备份文件是否存在,如果不存在，则跳转到下一个表,否则，导入数据库
			if (!file_exists($dbkdir.$table[$tableid]."_".$pageid.".sql")) {
				$selfurl = "dbbak.php?m=backin&act=recover_data&table=".$tables."&tableid=".($tableid+1);
				$msg = "数据表 <font color='red'>".$table[$tableid]."</font> 信息不存在或未曾备份！";
				helper :: redirect($selfurl, 2, $msg);
			}
			//读取当前分卷备份文件的内容
			$sql = read_msg($dbkdir.$table[$tableid]."_".$pageid.".sql");
			//将当前分卷备份文件的insert语句导入数据库
			if ($sql) recover_data($sql);
			//检查当前表下一个分卷备份文件是否存在,如果存在，则跳转到当前表下下一个分卷备份,否则，跳转到下个表
			if (file_exists($dbkdir.$table[$tableid]."_".($pageid+1).".sql")) {
				$selfurl ="dbbak.php?m=backin&act=recover_data&table=".$tables."&tableid=".$tableid."&pageid=".($pageid+1);
				$msg = "正在恢复数据表 <font color='red'>".$table[$tableid]."</font> 信息";
				helper :: redirect($selfurl, 2, $msg);
			} else {
				$selfurl ="dbbak.php?m=backin&act=recover_data&table=".$tables."&tableid=".($tableid+1);
				$msg = "已经恢复数据表 <font color='red'>".$table[$tableid]."</font> 信息，将恢复下一个数据表信息！";
				helper :: redirect($selfurl, 2, $msg);
			}
		} else {
			//删除缓存，解决数据还原后，页面及即时更新
			if ( is_dir('../cache') && dirsize("../cache") > 0) {
				del_dir('../cache', 1);
			}
			die("<font color='red'>数据信息均已经恢复完毕！</font>");
			
		}
		
	}
}

//删除备份文件
function m__del() {
	global $dbkdir;
	foreach (glob($dbkdir . "*.sql") as $filename) {
		unlink($dbkdir . $filename);
	}
	die('{"code":"0","msg":"备份文件删除成功"}');	
}

/*备份原理：
 *一表一文件
 *单表文件大于2048K时，自动创建新文件！
 *首次生成数据结构表！该表起总纲作用！
 */
//备份文件
function m__save() {
    global $dbm, $page, $dbkdir; 
	$table = @trim($_GET['table']);
	if(!isset($table) && empty($table)){
		die('{"code":"100","msg":"请选择要备份数据表"}');
	}
	helper :: mkdirs($dbkdir); 
	//判断备份目录是否有可写的权限
	if (is_dir($dbkdir)) {
		$iswrite = @fopen($dbkdir . 'dd.txt', 'w');
		if ($iswrite) {
			$is_write = 1;
			fclose($iswrite);
			unlink($dbkdir . 'dd.txt');
		} else {
			$is_write = 0;
		}
		if(!$is_write) die('{"code":"200","msg":"目录没有写的权限"}');
	}
	$tables = strpos($table,',')!== false ? explode(',', $table) : array($table);
	if(isset($_GET['first']) && $_GET['first'] == 1) {
		foreach (glob($dbkdir . "*.sql") as $filename) {
			unlink($dbkdir . $filename);
		}
	}
	//开始备份数据库
	backup_action($tables);	
}

//创建目录
function mkdirs($dir, $mode = 0777) {
	if (is_dir($dir) || @mkdir($dir, $mode)) return TRUE;
	if (!mkdirs(dirname($dir), $mode)) return FALSE;
	return @mkdir($dir, $mode);
}

//获取创建表的语句
function bakuptable($table) {
	global $dbm;
	$creattable = "DROP TABLE IF EXISTS $table;\n";
	$res = $dbm->query("SHOW CREATE TABLE $table");
	$creattable .= $res['list'][0]['Create Table'].";\n\n";
	return $creattable;
}

//分割insert语句执行
function recover_data($sql) {
	global $dbm;
	$sql=str_replace("\r","\n",$sql);
	$sql_array=explode(";\n",$sql);
	foreach($sql_array as $key=>$value){
		$value = trim($value);
		if($value == "#" || $value == "--"){
			$queryy = explode("\n",$value);
			$value = '';
			foreach($queryy as $v2){
				if($v2[0]!='#') $value.=$v2;
			}
		}
		if($value){
			$value=trim(str_replace("\n","",$value));
			$dbm->query_update($value);	
		}
	}
	return true;
}

//获取数据库所有的表名
function get_tables_name() {
	global $dbm;
	$tablename = array();
	$table = $dbm->query("SHOW TABLES");
	foreach ($table['list'] as $val) {
		$tb = array_values($val);
		$tablename[] = $tb[0];
	}
	return $tablename;
}

//备份数据库
function backup_action($table) {
	global $dbm,$dbkdir;
	$tables = implode(',',$table);
	$array = array();
	if (!is_array($table) && empty($table)) return $array;
		if(!file_exists($dbkdir . DB_DBNAME . ".sql")) {
			for ($i=0;$i<count($table);$i++) {
				$array[$i]=bakuptable($table[$i]);
			}
			$table_struct = implode("\n", $array);
			$handle = fopen($dbkdir . DB_DBNAME . ".sql","wb");
			fputs($handle, $table_struct);
			fclose($handle);
			unset($table_struct );//释放内存
	}
	//表ID下标
	$tableid = isset($_GET["tableid"]) ? intval($_GET["tableid"]) : 0;
	//limit起始值
	$startid = isset($_GET["startid"]) ? intval($_GET["startid"]) : 0;
	//分卷编号
	$pageid  = isset($_GET["pageid"]) ? intval($_GET["pageid"]) : 0;
	
	//开始内容备份
	if ($tableid < count($table)) {
		$tablename = $table[$tableid];
		//统计当前表的记录总数
		$ret = $dbm -> query("select count(1) as c from " . $tablename);
		$count = $ret['list'][0]['c'];
		$per_size = 1000;//最大数不超过1000
		if ($count < $per_size) $per_size = $count;
		if ($count && $startid < $count) {
			$res = $dbm -> query("select * from " . $tablename . " limit " . $startid . "," . $per_size);
			$bakupdata = '';
			foreach ($res['list'] as $k => $v) {
				$tablefields = implode("`,`", array_keys($v));
				$tablevalues = preg_replace('~(\n)|(\r)~', '',implode("','", array_values($v)));
				$bakupdata .= "INSERT INTO $tablename (`" . $tablefields . "`) ";
				$bakupdata .= "VALUES ('" . $tablevalues . "');\n";
				$startid++;
				if (strlen($bakupdata) > (2048*1024)) {
					$handle = fopen($dbkdir . $tablename . "_" . $pageid . ".sql.","wb");
					fputs($handle, $bakupdata);
					fclose($handle);
					unset($bakupdata);
					$selfurl = "dbbak.php?m=save&tableid=".$tableid."&startid=".($startid)."&pageid=".($pageid+1)."&table=$tables";
					echo ('<script type="text/javascript">setTimeout(function(){window.location.href="' . $selfurl . '";},1000);</script>');
					echo '<br />';
					echo "正在备份数据表 <font color='red'>".$tablename."</font> 信息，当前已经写入第 ".($pageid+1)." 页，即将写入第 ".($pageid+2)." 页信息";
					die();
				}
			}
			if ($bakupdata) {
				if ($pageid > 0) {
					$data = file_get_contents($dbkdir . $tablename . '_'  . ($pageid-1) . ".sql");
					if (strlen($data) < (2048*1024)) {
						$handle = fopen($dbkdir . $tablename . '_'  . ($pageid-1).".sql","ab");
						fputs($handle,$bakupdata);
						fclose($handle);
						unset($bakupdata,$data);
						$newpageid = $pageid;
					} else {
						$handle = fopen($dbkdir . $tablename."_".$pageid.".sql","wb");
						fputs($handle,$bakupdata);
						fclose($handle);
						unset($bakupdata);
						$newpageid = $pageid + 1;
					}
				} else {
					$handle = fopen($dbkdir . $tablename."_".$pageid.".sql","wb");
					fputs($handle,$bakupdata);
					fclose($handle);
					unset($bakupdata);
					$newpageid = $pageid + 1;
				}
			}
			if ($startid<$count) {
				$selfurl = "dbbak.php?m=save&tableid=".$tableid."&startid=".($startid)."&pageid=".$newpageid."&table=$tables";
				echo ('<script type="text/javascript">setTimeout(function(){window.location.href="' . $selfurl . '";},1000);document.write("正在备份数据表 '.$tablename.' 信息。");</script>');
				die();
			} else {
				$selfurl = "dbbak.php?m=save&tableid=".($tableid+1)."&table=$tables";
				echo ('<script type="text/javascript">setTimeout(function(){window.location.href="' . $selfurl . '";},1000);document.write("数据表 '.$tablename.' 信息已经备份完毕，将开始备份下一个数据表！");</script>');
				die();
			}
		} else {
			$selfurl = "dbbak.php?m=save&tableid=".($tableid+1)."&table=$tables";
			echo ('<script type="text/javascript">setTimeout(function(){window.location.href="' . $selfurl . '";},1000);document.write("数据表 '.$tablename.' 信息为空，将开始下一个数据表信息备份");</script>');
			die();
		}
	} else {
		//die('{"code":"100","msg":"数据已经备份完毕！"}');
		die("<font color='red'>数据已经备份完毕！</font>");
	}	
}

//备份数据库文件名赋加随机数
function num_rand($lenth) {
	$randval = '';
	mt_srand((double)microtime() * 1000000);
	for ($i=0;$i<$lenth;$i++) {
		$randval .= mt_rand(0,9);
	}
	$randval=substr(md5($randval),mt_rand(0,32-$lenth),$lenth);
	return $randval;
}


?>