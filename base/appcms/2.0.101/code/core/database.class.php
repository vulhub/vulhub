<?php
/**
 * AppCMS Copyright (c) 2012-2013  
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers

 */
class db_mysql {
    public $query_count = 0; //数据库操作次数
    private $conn; //数据库连接对象
    /**
     * 初始化类，根据传入的数据库参数连接数据库
     */
    public function __construct() {
        $this -> conn = mysql_connect(DB_HOST , DB_USERNAME , DB_PASS) or die("do not connect database");
        mysql_select_db(DB_DBNAME , $this -> conn) or die("do not open database");
        mysql_query("set names " . DB_CHARSET);
    } 
    /**
     * 销毁类
     */
    public function __destory() {
        mysql_close();
    } 
    /**
     * 执行查询语句返回结果集
  $sql        查询语句，不包括 order by limit等后缀
     *   $suffix     为where条件完毕之后的sql语句如order by 和 limit 等
  $is_total   用于获取分页数据的总记录数，默认值0为不count，不分页千万不要传递此参数
     */
    public function query($sql, $suffix = '', $is_total = 0) {
        // 判断是否取记录总数,0为不取，1为取
        $_start = helper :: getmicrotime();
        $total = 0;
        $sql_count_time = 0;
        if ($is_total > 0) {
            $count_sql=preg_replace("~select (.*?) from(.*?)~","select count(1) as t from $2",strtolower($sql));
            $countid=mysql_query($count_sql);
            $total_rs=mysql_fetch_assoc($countid);
            $total = $total_rs['t'];
            //$total = mysql_num_rows(mysql_query($sql));
            $this -> query_count++;
            $sql_count_time = helper :: getmicrotime() - $_start;
        } 
        // 查询取得记录列表
        $rs = mysql_query($sql . ' ' . $suffix);
        $this -> query_count++;
        $i = 0;
        $list = array();
        if ($rs) {
            while ($rows = mysql_fetch_assoc($rs)) {
                $list[$i] = $rows;
                $i++;
            } 
        } 
        // 返回该查询的记录总数和记录列表
        $querys = array('sql' => $sql . ' ' . $suffix, // SQL
            'error' => mysql_error(), // SQL报错信息
            'sql_time' => helper :: getmicrotime() - $_start, // 整个SQL完成耗费时间
            'sql_time_count' => $sql_count_time, // 统计行数耗费时间
            'total' => $total, // 记录总数，如果$is_total=0，则该值为0
			
            'list' => $list,
            );//print_r($querys);
        return $querys;
    } 
    /**
     * 执行插入操作
     *   特别返回：返回最后插入的自增ID值
     */
    public function query_insert($sql) {
        $_start = helper :: getmicrotime();
        mysql_query($sql);
        $this -> query_count++;
        $querys = array('sql' => $sql,
            'error' => mysql_error(),
            'sql_time' => helper :: getmicrotime() - $_start, // 整个SQL完成耗费时间
            'autoid' => mysql_insert_id(),
            );
        return $querys;
    } 
    /**
     * 执行更新、删除操作
     */
    public function query_update($sql) {
        $_start = helper :: getmicrotime();
        mysql_query($sql);
        $this -> query_count++;
        $querys = array('sql' => $sql,
            'error' => mysql_error(),
            'sql_time' => helper :: getmicrotime() - $_start, // 整个SQL完成耗费时间
            );
        return $querys;
    } 
    /**
     * 单表插入
     *   $table_name     表名
     *   $fields         字段和表单域对应值，格式如array("fields1"=>val1,"fields2"=>val2....)，插入时候循环遍历
     */
    public function single_insert($table_name, $fields) {
        if (!is_array($fields) || count($fields) == 0) return array('sql' => '', 'error' => '插入失败，插入字段为空', 'sql_time' => 0, 'autoid' => 0);

        $sql_field = "";
        $sql_value = ""; 
        // 遍历字段和值
        foreach($fields as $key => $value) {
            $sql_field .= ",$key";
            $sql_value .= ",'$value'";
        } 

        $sql_field = substr($sql_field, 1);
        $sql_value = substr($sql_value, 1);

        $sql = "insert into $table_name ($sql_field) values ($sql_value)"; //组合SQL
        $result = $this -> query_insert($sql);

        return $result;
    } 
    /**
     * 单表更新
     *   $table_name     表名
     *   $fields         字段和表单域对应值，格式如array("fields1"=>val1,"fields2"=>val2....)，插入时候循环遍历
  $where          更新的条件语句
     */
    public function single_update($table_name, $fields, $where = '') {
        if (!is_array($fields) || count($fields) == 0) return array('sql' => '', 'error' => '更新失败，插入字段为空', 'sql_time' => 0);

        $sql_set = ""; 
        // 遍历字段和值
        foreach($fields as $key => $value) {
            $sql_set .= ",$key='$value'";
        } 

        $sql_set = substr($sql_set, 1);
        if (strlen($where) > 0) $where = "where $where";
        $sql = "update $table_name set $sql_set $where"; //组合SQL
        $result = $this -> query_update($sql);

        return $result;
    } 

    /**
     * 单表查询
     *   $params
     *   -> $table_name     表名 必填
     *   -> $fields         字段 field1,field2,...
     *   -> $where          更新的条件语句
  -> $suffix         order by , limit 语句
     *   -> $count          计算总量 0=不计算 1=计算
     *   -> $pagesize       分页大小
     */

    public function single_query($params) {
        // 初始化
        $table_name = isset($params['table_name'])?$params['table_name']:'';
        $fields = isset($params['fields'])?$params['fields']:'*';
        $where = isset($params['where'])?$params['where']:'';
        $suffix = isset($params['suffix'])?$params['suffix']:'';
        $count = isset($params['count'])?$params['count']:0;
        $pagesize = isset($params['pagesize'])?$params['pagesize']:10;
        if (strlen($where) > 0) $where = "where $where";

        $sql = "select $fields from $table_name $where";
        $result = $this -> query($sql, $suffix, $count);
        $result['pagebar'] = helper :: pagebar(array('total' => $result['total'], 'pagesize' => $pagesize, 'rewrite' => 2));
        return $result;
    } 
    /**
     * 单表删除
     * 
     *   -> $table_name     表名 必填
     *   -> $where         删除的限制条件，必填
     */
    public function single_del($table_name, $where) {
        // 判断限制
        if (empty($table_name) || empty($where)) return false;
        if (strlen($where) > 0) $where = "where $where";

        $sql = " delete from $table_name $where";
        $result = $this -> query_update($sql);
        return $result;
    } 

    /**
     * 返回limit语句
     *   $pagesize   分页大小
     *   $p          当前页码
     */
    public function get_limit_sql($pagesize = 10, $p = 1) {
        return "limit " . ($p-1) * $pagesize . ",$pagesize";
    } 

    /**
     * 单表查询
     *   $params
     *   -> $table_name     表名 必填
     *   -> $fields         字段 field1,field2,...
     *   -> $where          更新的条件语句
  -> $suffix         order by , limit 语句
     *   -> $count          计算总量 0=不计算 1=计算
     *   -> $pagesize       分页大小
     * $params=array('table1'=>'cate','table2'=>'info','joinon'=>'cate_id#last_cate_id','fields'=>'a.field,b.felde'......)
     */

    public function join_query($params) {
        // 初始化
        $table1 = isset($params['table1'])?$params['table1']:'';
        $table2 = isset($params['table2'])?$params['table2']:''; 
        // 关联字段 cate_id#last_cate_id 以#分隔
        $joinon = isset($params['joinon'])?$params['joinon']:'';
        $joinon = explode("#", $joinon);
        $fields = isset($params['fields'])?$params['fields']:'*';
        $where = isset($params['where'])?$params['where']:'';
        $suffix = isset($params['suffix'])?$params['suffix']:'';
        $count = isset($params['count'])?$params['count']:0;
        $pagesize = isset($params['pagesize'])?$params['pagesize']:10;
        if (strlen($where) > 0) $where = "where $where";
        $sql = "select $fields from $table1 as a left join $table2 as b on a." . $joinon[0] . "=b." . $joinon[0] . " $where";
        $result = $this -> query($sql, $suffix, $count);
        $result['pagebar'] = helper :: pagebar(array('total' => $result['total'], 'pagesize' => $pagesize, 'rewrite' => 2));
        return $result;
    } 
    /**
     */
    public function scalar($sql, $suffix = '') {
        $res = $this -> query($sql, $suffix); 
        // if (!empty($res['error'])) return false;
        if (empty($res['list'])) return null;
        return $res['list'][0];
    }
	/**
	 *检查表是否存在
	 * $params
	 * ->$table 表名 必填
     */
	public function check_exists_table($table) {
		$res = $this -> query("SHOW TABLES");
		$havbz = false;
		foreach ($res['list'] as $tablename) {
			$val = array_values($tablename);
			if ($val[0] == $table) {
				$havbz = true;
				break;
			}
		}
		return $havbz;
	}
} 

?>