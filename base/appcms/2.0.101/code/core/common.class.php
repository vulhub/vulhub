<?php
/**
 * AppCMS Copyright (c) 2012-2013
 * AppCMS is a free open source mobile phone APP application download website content management system.
 * Custom development, production BUG report template, please contact loyjers@qq.com
 * Author: loyjers
 * Editor: loyjers,crane
 */

/**
 * 前后台公用方法类
 * 全部分类数据获取 $c->categories;
 */
class common
{
    public $categories = array(); //全部分类数组，已经更改下标为对应的cate_id
    public $url = null; //伪静态对象
    public $url_config = array(); //伪静态规则数组;
    public $dbm; //数据库对象
    public $cache_time = 3600; //缓存时间
    public $cache_path = 'cache'; //缓存路径

    /**
     * 初始化
     */
    public function __construct($dbm)
    {
        $this->dbm = $dbm; //初始化数据库类
        // 伪静态对象初始化
        $a = helper :: file_cache('url_config', '', $this->cache_time, SITE_PATH . $this->cache_path);
        if ($a == 'timeout') $a = helper :: file_cache('url_config', $this->url_config(), $this->cache_time, SITE_PATH . $this->cache_path);
        $this->url_config = $a;
        $this->url = new url_rewrite($this->url_config);
        $this->url->rewrite = SITE_REWRITEURL;
        $this->url->root_path = SITE_PATH;
        // 获取分类
        $a = helper :: file_cache('categories', '', $this->cache_time, SITE_PATH . $this->cache_path);
        if ($a == 'timeout') $a = helper :: file_cache('categories', $this->categories(), $this->cache_time, SITE_PATH . $this->cache_path);
        $this->categories = $a;
    }

    /**
     * 获取伪静态规则
     */
    public function url_config()
    {
        $sql = "select * from " . TB_PREFIX . "url_rewrite";
        $a = $this->dbm->query($sql);
        $b = array();
        foreach ($a['list'] as $key => $value) {
            $b[$value['url_title']] = array('url' => $value['url_true'],
                'rule' => $value['url_rule'],
            );
        }
        return $b;
    }

    /**
     * 更新伪静态规则缓存
     */
    public function update_url_config()
    {
        $this->url_config = helper :: file_cache('url_config', $this->url_config(), $this->cache_time, SITE_PATH . $this->cache_path);
    }

    /**
     * 更新分类缓存
     */
    public function update_cate()
    {
        $this->categories = helper :: file_cache('categories', $this->categories(), $this->cache_time, SITE_PATH . $this->cache_path);
    }

    public function get_scroll_list($page = '', $num = 10)
    {
        $page = isset($page) ? (int)$page : 0;
        $num = isset($num) ? (int)$num : 10;
        $startNum = $page * $num;

        $sql = "select * from " . TB_PREFIX . "app_list limit $startNum,$num";
        $rs = $dbm->query($sql);
        $data = array();
        foreach ($rs['list'] as $k => $v) {
            $data[] = $v;
        }
        return $data;
    }

    /**
     * 获取热门关键字
     */
    public function get_keyword($limit = 5)
    {
        $content = array();
        $limit = isset($limit) && is_numeric($limit) ? $limit : 5;
        $sql = "select q from " . TB_PREFIX . "search_keyword order by qorder desc  limit $limit";
        $keyword_sql = $this->dbm->query($sql);
        if (count($keyword_sql['list']) > 0) {
            $keyword = $keyword_sql['list'];
            foreach ($keyword as $v) {
                $content[] = $v['q'];
            }
        } else {
            $content = array('大富翁', '神庙', 'UC', '超级玛丽', '视频播放器');
        }
        return $content;
    }

    /**
     * 统计应用和资讯数量（当天，总数）
     *
     * @param  $ 需要统计的项目
     */
    public function get_app_info_count($statis = array('app_total', 'app_today', 'info_total', 'info_today'))
    {
        $ret = array();
        $today = strtotime(date('Y-m-d', time()));
        foreach ($statis as $a) {
            if ($a == 'app_total') {
                $b = $this->app_info_count("select sum(cdata) as total from " . TB_PREFIX . "category where parent_id = 0 and type = 0");
                $ret[$a] = $b['list'][0]['total'];
            }
            if ($a == 'app_today') {
                $b = $this->app_info_count("select count(1) as c from " . TB_PREFIX . "app_list where app_update_time>=" . $today);
                $ret[$a] = $b['list'][0]['c'];
            }
            if ($a == 'info_total') {
                $b = $this->app_info_count("select sum(cdata) as total from " . TB_PREFIX . "category where parent_id = 0 and type = 1");
                $ret[$a] = $b['list'][0]['total'];
            }
            if ($a == 'info_today') {
                $b = $this->app_info_count("select count(1) as c from " . TB_PREFIX . "info_list where info_update_time>=" . $today);
                $ret[$a] = $b['list'][0]['c'];
            }
        }
        return $ret;
    }

    // 统计SQL
    public function app_info_count($sql)
    {
        if (defined('CACHE_TIME') && CACHE_TIME != '' && CACHE_TIME != 0) {
            $cache_file = md5($sql);
            $cache_path = SITE_PATH . $this->cache_path . "/count";
            $a = helper :: file_cache($cache_file, '', $this->cache_time, $cache_path);
            if ($a == 'timeout') $a = helper :: file_cache($cache_file, $this->dbm->query($sql), $this->cache_time, $cache_path);
        } else {
            $a = $this->dbm->query($sql);
        }
        return $a;
    }

    /**
     * 获取所有分类
     */
    public function categories()
    {
        $sql = "select * from " . TB_PREFIX . "category order by corder ";
        $a = $this->dbm->query($sql);
        $tmp_categories = array();
        foreach ($a['list'] as $c) {
            if ($this->url != null) {
                if ($c['type'] == 0) {
                    $tbname = 'app';
                } else {
                    $tbname = 'info';
                }
                if ($c['cname_py'] == '') {
                    $c['surl'] = $this->url->encode('list_' . $tbname . '_index', array('host' => SITE_PATH, 'cid' => $c['cate_id'], 'p' => 1));
                } else {
                    $c['surl'] = $this->url->encode('cpy_list_' . $tbname . '_index', array('host' => SITE_PATH, 'cpy' => $c['cname_py'], 'p' => 1));
                }
            }
            $c['son'] = $this->cate_last($c['cate_id'], $a['list']);
            $tmp_categories[$c['cate_id']] = $c;
        }
        return $tmp_categories;
    }

    /**
     * 根据拼音取回分类ID
     *
     * @param  $cpy 分类拼音
     */
    public function cate_cpy($cpy)
    {
        foreach ($this->categories as $a) {
            if ($a['cname_py'] == $cpy) return $a['cate_id'];
        }
        return 0;
    }

    /**
     * 检测来源是手机用户
     */
    public function from_mobile()
    {
        $regex_match = "/(nokia|iphone|android|motorola|^mot\-|softbank|foma|docomo|kddi|up\.browser|up\.link|";
        $regex_match .= "htc|dopod|blazer|netfront|helio|hosin|huawei|novarra|CoolPad|webos|techfaith|palmsource|meizu|miui|ucweb";
        $regex_match .= "blackberry|alcatel|amoi|ktouch|nexian|samsung|^sam\-|s[cg]h|^lge|ericsson|philips|sagem|wellcom|bunjalloo|maui|";
        $regex_match .= "symbian|smartphone|midp|wap|phone|windows ce|iemobile|^spice|^bird|^zte\-|longcos|pantech|gionee|^sie\-|portalmmm|";
        $regex_match .= "jig\s browser|hiptop|^ucweb|^benq|haier|^lct|opera\s*mobi|opera\*mini|320x320|240x320|176x220";
        $regex_match .= ")/i";

        if (isset($_SERVER['HTTP_X_WAP_PROFILE']) || isset($_SERVER['HTTP_PROFILE']) || (isset($_SERVER['HTTP_USER_AGENT']) && preg_match($regex_match, strtolower($_SERVER['HTTP_USER_AGENT'])))) {
            return true;
        }

        return false;
    }

    /**
     * 取分类的父分类，返回数组，平面结构
     *
     * @param  $cate_id 分类ID
     */
    public function cate_father($cate_id)
    { // echo($cate_id.'<br>');
        $top_cate = array();
        if (count($this->categories) == 0) return array();
        array_push($top_cate, $this->categories[$cate_id]);
        $parent_id = $this->categories[$cate_id]['parent_id'];
        if (intval($parent_id) > 0) {
            $top_cate = $this->cate_father($parent_id);
            array_push($top_cate, $this->categories[$cate_id]);
        }
        return $top_cate;
    }

    /**
     * 取分类的子分类，返回数组，树形结构
     *
     * @params $cate_id 分类ID
     * 返回值为树形，因为子分类有可能是多分支的
     */
    public function cate_son($cate_id)
    {
        $ret = array();
        foreach ($this->categories as $c) {
            if ($c['parent_id'] == $cate_id) {
                $c['son'] = $this->cate_son($c['cate_id']);
                array_push($ret, $c);
            }
        }
        $ret = helper :: array_sort($ret, 'corder');
        return $ret;
    }

    /**
     * 判断分类是否为终极分类
     *
     * @param  $cate_id 分类ID
     * @param  $categories 树形目录
     */
    public function cate_last($cate_id, $categories = '')
    {
        $a = ($categories == '') ? $this->categories : $categories;
        foreach ($a as $b) {
            if ($b['parent_id'] == $cate_id) return 1;
        }
        return 0;
    }

    /**
     * 取分类的平级分类，返回数组，树形结构，第一级为平级
     *
     * @param cate_id $ 分类ID
     */
    public function cate_brother($cate_id)
    {
        $parent_id = $this->categories[$cate_id]['parent_id'];
        $brother = array();
        foreach ($this->categories as $c) {
            if ($c['parent_id'] == $parent_id) array_push($brother, $c);
        }
        return $brother;
    }

    /**
     * 根据分类ID返回完整树形
     *
     * @param  $type 为空输出全部分类，为0,1则返回各自类型的树
     * @param  $cate_id 分类ID，为0则返回全部树
     */
    public function cate_tree($type = '', $cate_id = 0)
    {
        $tree = array();
        if ($cate_id == 0) {
            foreach ($this->categories as $c) {
                if ($type == '') {
                    if ($c['parent_id'] == 0) {
                        array_push($tree, $c);
                        $tree[count($tree) - 1]['son'] = $this->cate_son($c['cate_id']);
                    }
                } else {
                    if ($c['parent_id'] == 0 && $c['type'] == $type) {
                        array_push($tree, $c);
                        $tree[count($tree) - 1]['son'] = $this->cate_son($c['cate_id']);
                    }
                }
            }
        } else {
            $tree = $this->cate_son($cate_id);
        }

        return $tree;
    }

    /**
     * 添加删除信息时候，根据last_cate_id重算分类信息总量
     *
     * @param  $type ='add' 加量 ='delete' 减量
     * @param  $last_cate_id 终极分类ID
     */
    public function cate_restore_cdata($last_cate_id, $type = 'add')
    {
        $parent = $this->cate_father($last_cate_id);
        // 遍历父类更改，包括自身
        foreach ($parent as $c) {
            if ($type == 'add') {
                $sql = "update " . TB_PREFIX . "category set cdata=cdata+1 where cate_id='" . $c['cate_id'] . "'";
            } else {
                $sql = "update " . TB_PREFIX . "category set cdata=cdata-1 where cate_id='" . $c['cate_id'] . "'";
            }
            $this->dbm->query_update($sql);
        }
    }

    /**
     * 根据last_cate_id插入分类关系
     *
     * @param  $type =0 应用 1 资讯
     * @param  $id 资讯或者应用ID
     * @param  $last_cate_id 终极分类ID
     */
    public function cate_insert_relation($last_cate_id, $id, $type = 0, $action = 'add')
    {
        $parent = $this->cate_father($last_cate_id);
        // 遍历父类更改，包括自身
        if ($action == 'add') {
            foreach ($parent as $c) {
                $this->dbm->single_insert(TB_PREFIX . 'cate_relation', array('cate_id' => $c['cate_id'], 'id' => $id));
            }
        } else {
            $where = " id='" . $id . "' ";
            $this->dbm->single_del(TB_PREFIX . 'cate_relation', $where);
        }
    }

    /**
     * 根据树形分类数组输出select下拉框
     *
     * @param  $tree 树形分类数组
     * @param  $i 层级计数，调用时为0即可
     * @param  $disabled = ' disabled="disabled" '
     * @param  $styles =' style="background:pink;margin-top:1px;"'
     */
    public function html_cate_select($tree = array(), $i = 0, $disabled = ' disabled="disabled" ', $styles = ' style="background:pink;margin-top:1px;"')
    {
        $i++;
        foreach ($tree as $t) {
            $flag = '';
            for ($j = 1; $j < $i; $j++) {
                $flag .= '　';
            }
            if (count($t['son']) > 0) {
                $flag .= '┗';
            } else {
                $flag .= '┗';
            }
            $readonly = '';
            $style = '';
            if (count($t['son']) > 0) $readonly = $disabled;
            if (count($t['son']) == 0) $style = $styles;
            echo('<option value="' . $t['cate_id'] . '"' . $readonly . $style . '>' . $flag . $t['cname'] . '</option>');
            $this->html_cate_select($t['son'], $i, $disabled, $styles);
        }
    }

    /**
     * 取资讯或者应用列表
     *
     * @params 为数组格式
     * @param  $ =>type    0 应用,1 资讯 默认为 0
     * @param  $ =>cate_id         分类ID
     * @param  $ =>where          更新的条件语句
     * @param  $ =>order          排序
     * @param  $ =>pagesize       分页大小
     * @param  $ =>p              当前页码
     * @param  $ =>rewrite        URL重写方式 0,1,2，默认为2
     * @param  $ =>count          是否计算总数 0 不计算 1 计算
     * @param  $ =>resource       是否同时取出资源 0 不取 1 取
     * @param  $ =>history   是否同时取出版本 0 不取 1 取
     * @param  $ =>fields     查询的字段
     */

    public function get_list($params, $no_cache = 0)
    {
        $type = isset($params['type']) ? $params['type'] : 0;
        if ($type == 0) {
            $tbname = 'app';
        } else {
            $tbname = 'info';
        }
        $cate_id = isset($params['cate_id']) ? $params['cate_id'] : '';
        $where = isset($params['where']) ? $params['where'] : '';
        $order = isset($params['order']) ? $params['order'] : ' order by ' . $tbname . '_update_time desc';
        $pagesize = isset($params['pagesize']) ? $params['pagesize'] : PAGESIZE;
        $p = isset($params['p']) ? $params['p'] : 1;
        $count = isset($params['count']) ? $params['count'] : 0;
        $rewrite = isset($params['rewrite']) ? $params['rewrite'] : 2;
        $resource = isset($params['resource']) ? $params['resource'] : 0;
        $history = isset($params['history']) ? $params['history'] : 0;
        $fields = isset($params['fields']) ? $params['fields'] : '';
        $node = isset($_GET['tpl']) ? $_GET['tpl'] : 'index';
        $strip_tags_desc = isset($params['strip_tags_desc']) ? $params['strip_tags_desc'] : 1;
        if (isset($_GET['cpy'])) $node = 'cpy_' . $node;

        if ($fields == '') {
            $fields = '*';
        } else {
            $fields_arr = explode(',', $fields);
            $fields = '';
            for ($i = 0; $i < count($fields_arr); $i++) {
                $fields .= ',' . trim($fields_arr[$i]);
            }
            $fields = substr($fields, 1);
        }

        $total = 0; //总数                                  
        // 拼接SQL语句
        // 分类ID为空，或者为终极分类，无需联表查询
        if ($cate_id == '' || (isset($this->categories[$cate_id]) && $this->categories[$cate_id]['son'] == 0)) {
            if ($cate_id == '') { // 分类ID为空
                $sql = "select $fields from " . TB_PREFIX . $tbname . "_list as b";
                if (strlen($where) > 0) {
                    $sql .= " where $where";
                }
                if ($count == 1) $count = 1;
            } else { // 分类ID不为空
                $sql = "select $fields from " . TB_PREFIX . $tbname . "_list as b  where last_cate_id='$cate_id'";
                if (strlen($where) > 0) {
                    $sql .= " and $where";
                    if ($count == 1) $count = 1;
                } else {
                    $total = $this->categories[$cate_id]['cdata'];
                    $count = 0;
                }
            }
        } else { // 分类ID不为空,也不是终极分类
            $order = preg_replace('~' . $tbname . '_~', 'id_', $order);
            $user_index = ''; //echo($order);
            if (strpos($order, 'id_update_time') > 0 && strpos($order, 'id_order') <= 0) $user_index = ' use index(id_update_time) ';
            if (strpos($order, 'id_down') > 0 && strpos($order, 'id_order') <= 0) $user_index = ' use index(id_down) ';
            $sql = "select * from " . TB_PREFIX . "cate_relation as a $user_index left join " . TB_PREFIX . $tbname . "_list as b on a.id=b." . $tbname . "_id where cate_id='$cate_id' ";
            if (strlen($where) > 0) {
                $sql .= " and $where";
                if ($count == 1) $count = 1;
            } else {
                $total = $this->categories[$cate_id]['cdata'];
                $count = 0;
            }
        }
        // 拼接排序
        $suffix = $order;
        // 拼接分页数据
        $suffix .= " limit " . ($p - 1) * $pagesize . ",$pagesize";
        if ($no_cache == 0) {
            // 缓存开启，获取缓存内容
            if (defined('CACHE_TIME') && CACHE_TIME != '' && CACHE_TIME != 0) {
                // 缓存文件名
                $cache_file = md5($sql . $suffix);
                $cache_pre = substr($cache_file, 0, 2);
                $cache_path = SITE_PATH . $this->cache_path . "/list/" . $cache_pre;
                // 读取缓存
                $result = helper :: file_cache($cache_file, '', CACHE_TIME, $cache_path);
                if ($result == 'timeout') {
                    $result = $this->dbm->query($sql, $suffix, $count);
                    for ($i = 0; $i < count($result['list']); $i++) {
                        unset($result['list'][$i]['uid']);
                        unset($result['list'][$i]['uname']);
                    }
                    // 写入缓存
                    $result = helper :: file_cache($cache_file, $result, CACHE_TIME, $cache_path);
                }
            } else {
                $result = $this->dbm->query($sql, $suffix, $count);
            }
        } else {
            $result = $this->dbm->query($sql, $suffix, $count);
        }
        // print_r($result['sql']);echo('<br>');print_r($result['sql_time']);
        if ($count == 1) {
            $total = $result['total'];
        } else {
            $result['total'] = $total;
        }
        // 重写返回数组
        for ($i = 0; $i < count($result['list']); $i++) {
            // 分类名
            if ($cate_id == '') {
                // print_r($result['list'][$i]['last_cate_id']);
                // print_r($this -> categories[$result['list'][$i]['last_cate_id']]);
                $result['list'][$i]['cate'] = $this->categories[$result['list'][$i]['last_cate_id']];
            } else {
                $result['list'][$i]['cate'] = $this->categories[$cate_id];
            }
            // 取资源列表
            if ($resource == 1) {
                $result['list'][$i]['resource'] = $this->get_resource($result['list'][$i][$tbname . '_id'], $type);
                $result['list'][$i]['resource_total'] = count($result['list'][$i]['resource']);
            }
            // 取历史版本
            if ($history == 1) {
                $result['list'][$i]['history'] = $this->get_history($result['list'][$i][$tbname . '_id']);
                $result['list'][$i]['history_total'] = count($result['list'][$i]['history']);
            }
            // 缩略图
            if (isset($result['list'][$i]['info_img'])) $result['list'][$i]['thumb'] = $this->thumb_url($result['list'][$i]['info_img']);
            if (isset($result['list'][$i]['app_logo'])) {
                $result['list'][$i]['thumb'] = $this->thumb_url($result['list'][$i]['app_logo']);
                //开启图片地址伪静态
                if (defined('IMG_REWRITE') && IMG_REWRITE) {
                    if (substr($result['list'][$i]['app_logo'], 0, 7) == 'http://') {
                        if (strpos($result['list'][$i]['app_logo'], '322.com.cn') > 0 || strpos($result['list'][$i]['app_logo'], 'appcms.cc') > 0) {
                            //图片地址伪静态
                            $shffix = substr($result['list'][$i]['app_logo'], strrpos($result['list'][$i]['app_logo'], '.'));
                            $result['list'][$i]['app_logo'] = SITE_PATH . 'image/' . base64_encode($result['list'][$i]['app_logo']) . $shffix;
                        }
                    }
                }

            }
            /**
             * if(strpos($result['list'][$i]['app_logo'],'322.com.cn')>0 || strpos($result['list'][$i]['app_logo'],'appcms.cc')>0){
             * $result['list'][$i]['app_logo'] = SITE_PATH."img.php?url=".$result['list'][$i]['app_logo'];
             * }
             */
            // URL
            $result['list'][$i]['surl'] = $this->url->encode('content_' . $tbname, array('host' => SITE_PATH, 'id' => $result['list'][$i][$tbname . '_id'], 'rewrite_tag' => @$result['list'][$i]['rewrite_tag']));
            //应用简介
            if ($type == 0 && isset($result['list'][$i]['app_desc']) && $result['list'][$i]['app_desc'] != '' && $strip_tags_desc == 1) {
                $result['list'][$i]['app_desc'] = strip_tags($result['list'][$i]['app_desc']);
            }
            // 资讯简介
            if ($type == 1 && $result['list'][$i]['info_desc'] == '') {
                $result['list'][$i]['info_desc'] = helper :: utf8_substr(strip_tags($result['list'][$i]['info_body']), 0, 220);
            }

        }
        // 分页码HTML
        if ($rewrite == 0 || $rewrite == 1) {
            // 前台使用动态地址或者伪静态地址
            if ($this->url != null && $params['count'] == '1') {
                $cpy = '';
                $q = isset($_GET['q']) ? $_GET['q'] : '';
                if (intval($cate_id) > 0) $cpy = $this->categories[$cate_id]['cname_py'];
                $result['pagebar'] = helper :: pagebar(
                    array('total' => $total, 'pagesize' => $pagesize, 'rewrite' => $rewrite,
                        'rule' =>
                            array('node' => $node,
                                'obj' => $this->url,
                                'params' =>
                                    array('host' => SITE_PATH,
                                        'cid' => $cate_id,
                                        'cpy' => $cpy,
                                        'q' => $q,
                                        'p' => $p
                                    ),
                            )
                    )
                );
            } else {
                $result['pagebar'] = array('pagecode' => '');
            }
        } else {
            $result['pagebar'] = helper :: pagebar(array('total' => $total, 'pagesize' => $pagesize, 'rewrite' => $rewrite));
        }

        return $result;
    }

    /**
     * 取资讯或者应用内容
     *
     * @param  $id 资讯或者应用ID
     * @param  $type 0 应用,1 资讯 默认为 0
     * @param  $data 是否用服务端app_id查询，默认为0，否则为1
     */

    public function get_content($id, $type = 0, $data = 0, $no_cache = 0, $is_page = 1)
    {
        $content = array();
        if (!is_numeric($id)) return $content;
        if (!is_numeric($type)) return $content;

        if ($type == 0) {
            $tbname = 'app';
        } else {
            $tbname = 'info';
        }

        $sql = "select * from " . TB_PREFIX . $tbname . "_list where " . $tbname . "_id='$id' limit 1";
        // 如果是应用，并且指定了以服务端app_id查询
        if ($data == 1 && $type == 0) {
            $sql = "select * from " . TB_PREFIX . $tbname . "_list where data_app_id='$id' limit 1";
        }
        // 缓存开启，获取缓存内容
        if ($no_cache == 0) {
            if (defined('CACHE_TIME') && CACHE_TIME != '' && CACHE_TIME != 0) {
                $cache_file = md5($sql);
                $cache_pre = substr($cache_file, 0, 2);
                $caceh_path = SITE_PATH . $this->cache_path . '/content/' . $cache_pre;
                $rs = helper :: file_cache($cache_file, '', CACHE_TIME, $caceh_path);
                if ($rs == 'timeout') {
                    $rs = $this->dbm->query($sql);
                    $rs = helper :: file_cache($cache_file, $rs, CACHE_TIME, $caceh_path);
                }
            } else {
                $rs = $this->dbm->query($sql);
            }
        } else {
            $rs = $this->dbm->query($sql);
        }

        if (count($rs['list']) > 0) {
            $content = $rs['list'][0];
            $content[$tbname . '_mtitle'] = $content[$tbname . '_title'];
            // SEO别名
            if (strlen($content[$tbname . '_stitle']) > 0) $content[$tbname . '_title'] = $content[$tbname . '_stitle'];
            // 缩略图
            if (isset($content['info_img'])) $content['thumb'] = $this->thumb_url($content['info_img']);
            if (isset($content['app_logo'])) {
                $content['thumb'] = $this->thumb_url($content['app_logo']);
                if (defined('IMG_REWRITE') && IMG_REWRITE) {
                    if (substr($content['app_logo'], 0, 7) == 'http://') {
                        if (strpos($content['app_logo'], '322.com.cn') > 0 || strpos($content['app_logo'], 'appcms.cc') > 0) {
                            $shffix = substr($content['app_logo'], strrpos($content['app_logo'], '.'));
                            //图片地址伪静态
                            $content['app_logo'] = SITE_PATH . 'image/' . base64_encode($content['app_logo']) . $shffix;
                        }
                    }
                }
            }
            // 取外部资源
            $content['resource'] = $this->get_resource($id, $type);
            // 应用则取历史版本
            if ($type == 0) {
                $content['history'] = $this->get_history($id);
            }
            // 资讯取上下页
            if ($type == 1) {
                $content['next'] = $this->get_list(array('type' => $type, 'where' => ' info_id>' . $id, 'pagesize' => 2, 'order' => ' order by info_id asc'));
                $content['prev'] = $this->get_list(array('type' => $type, 'where' => ' info_id<' . $id, 'pagesize' => 2, 'order' => ' order by info_id desc'));
                if (count($content['next']['list']) > 0) {
                    $content['next']['surl'] = $content['next']['list'][0]['surl'];
                    $content['next']['info_title'] = $content['next']['list'][0]['info_title'];
                } else {
                    $content['next']['surl'] = '';
                    $content['next']['info_title'] = '';
                }
                if (count($content['prev']['list']) > 0) {
                    $content['prev']['surl'] = $content['prev']['list'][0]['surl'];
                    $content['prev']['info_title'] = $content['prev']['list'][0]['info_title'];
                } else {
                    $content['prev']['surl'] = '';
                    $content['prev']['info_title'] = '';
                }
            }
            $content['surl'] = $this->url->encode('content_' . $tbname, array('host' => SITE_PATH, 'id' => $id));
        }
        // 是否生成正文内链
        if (CONTENT_NLINK == '1') {
            if ($type == 0) {
                $content['app_desc'] = $this->nlink($content['app_desc']);
            } else {
                $content['info_body'] = $this->nlink($content['info_body']);
            }
        }
        //资讯正文手工分页
        if ($is_page == 1) {
            $pattern = '~<div style="page-break-after: always;">(.*)</div>~isU';
            if ($type == 1) {
                if (preg_match($pattern, $content['info_body'], $arr)) {
                    $content['info_body'] = helper :: escape_stripslashes($content['info_body']);
                    $content_arr = preg_split($pattern, $content['info_body']);
                    $_html = '<div class="pagebar" style="margin:10px 0 0 0;padding:0;">';
                    $content_total = count($content_arr);
                    $pageno = isset($_GET['p']) ? intval($_GET['p']) : 1;
                    if (is_numeric($pageno) && $pageno < 1) {
                        $pageno = 1;
                    }
                    if ($pageno > $content_total) {
                        $pageno = $content_total;
                    }
                    for ($i = 1; $i <= $content_total; $i++) {

                        if ($pageno == $i) {
                            $_html .= '<span class="now_class">' . $i . '</span>';
                        } else {

                            //$_nurl = helper :: url_replace($i, 'pageno');
                            $_nurl = $this->url->encode('content_' . $tbname . "_list", array('host' => SITE_PATH, 'id' => $id, 'p' => $i));
                            //$_nurl = $id.'_'.$i.'.html';
                            $_html .= '<a href="' . $_nurl . '">' . $i . '</a>';
                        }
                    }
                    $_html .= '</div>';
                    $content['info_body'] = $content_arr[$pageno - 1] . $_html;
                }
            }
        }
        $p = isset($_GET['p']) ? $_GET['p'] : 1;
        // 取评论代码
        if (defined('COMMENT_CODE') && COMMENT_CODE != '') {
            $content['comment_code'] = COMMENT_CODE;
        } else {
            $content['comment_code'] = '<div id="comment_list">' . $this->get_comments($id, $type, $p) . $this->getcomment_form($type, $id) . '</div>';
        }
        return $content;
    }

    // 取评论列表
    function get_comments($id, $type, $p = 1)
    {
        $suffix = '';
        $where = "id = '$id' and type = '$type' and parent_id='0'";
        $suffix .= " order by date_add desc ";
        $suffix .= $this->dbm->get_limit_sql(PAGESIZE, $p);
        $params = array('table_name' => TB_PREFIX . 'comment', 'pagesize' => PAGESIZE, 'where' => $where, 'count' => 1, 'suffix' => $suffix);
        $res = $this->dbm->single_query($params);
        $page['comments']['list'] = $res['list'];
        $page['comments']['pagecode'] = $res['pagebar']['pagecode'];
        $html = '<div style="border-bottom:1px solid #ccc;height:30px;line-height:30px;font-size:14px;font-weight:bold;">网友评论</div>';
        if (count($page['comments']['list']) > 0) {
            $top_comment = $page['comments']['list'];
            $i = 0;
            foreach ($top_comment as $c) {
                if ($c['son'] > 0) $top_comment[$i]['sons'] = $this->get_comment($c['comment_id']);
                $i++;
            }
            $html .= $this->show_comment($top_comment);
            // print_r($res);
            $pagesize = ceil($res['total'] / PAGESIZE);
            $html .= '<div class="pagebar" style="margin:10px 0 0 0;padding:0;"><style>.pagebar a{height:18px;line-height:18px;padding:0 5px;}</style>';
            for ($i = 1; $i <= $pagesize; $i++) {
                $html .= '<a onclick="page(' . $id . ',' . $type . ',' . $i . ');">' . $i . '</a>';
            }
            $html .= '<script type="text/javascript">
				//js分页函数
				function page(id, type, p){
					if(!p) p = 1;
					$.get("comment.php?m=list",{id:id,type:type,p:p},function(data){
						$("#comment_list").html(data);
					});
				}
				
			</script>';
            $html .= '</div><br />';
            return $html;
        }
    }

    // 取子评论
    function get_comment($parent_id)
    {
        global $dbm;
        $sql = "select * from " . TB_PREFIX . "comment where parent_id='$parent_id' order by date_add asc";
        $rs = $this->dbm->query($sql);
        $i = 0;
        foreach ($rs['list'] as $c) {
            if ($c['son'] > 0) $rs['list'][$i]['sons'] = $this->get_comment($c['comment_id']);
            $i++;
        }
        return $rs['list'];
    }

    // 输出评论HTML
    function show_comment($comment)
    {
        $html = '<div class="c_list" style="margin:5px 0 0 0;font-size:12px;clear:both;">';
        foreach ($comment as $c) {
            $html .= '<div class="c_list_con" style="background:#f7f7f7;margin:5px 0 0 0;border:1px solid #ccc;padding:5px;clear:both;">';
            $html .= '<div class="c_list_con_top clearfix">';
            $uname = $c['uname'] == '' ? '匿名评论' : $c['uname'];
            $html .= '<div class="l"><b id="uname' . $c['comment_id'] . '">' . $uname . '</b> &nbsp;' . $c['ip_addr'] . ' ' . helper :: utf8_substr($c['ip'], 0, strrpos($c['ip'], '.')) . '.* &nbsp; ' . date('Y-m-d H:i:s', $c['date_add']) . '</div>';
            $html .= '</div>';

            $html .= '<div class="c_list_con_bot" style="text-align:right;"><a href="javascript:void(0);" onclick="recomment(' . $c['comment_id'] . ',\'' . $uname . '\');">回复(<span id="recomment' . $c['comment_id'] . '">' . $c['son'] . '</span>)&nbsp;&nbsp;<a href="javascript:void(0);" onclick="cgood(' . $c['comment_id'] . ');">顶(<span id="good' . $c['comment_id'] . '">' . $c['good'] . '</span>)</a>&nbsp;&nbsp;<a href="javascript:void(0);" onclick="cbad(' . $c['comment_id'] . ');">踩(<span id="bad' . $c['comment_id'] . '">' . $c['bad'] . '</span>)</a>&nbsp;&nbsp;</div>';
            $html .= '<div class="c_  list_con_mid">' . (!empty($c['is_check']) ? $c['content'] : '<font color="red">*******该留言已被屏蔽</font>') . '</div>';
            if (isset($c['sons']) && count($c['sons']) > 0) {
                $html .= $this->show_comment($c['sons']);
            }
            $html .= '</div>';
        }

        $html .= '<script type="text/javascript">
			function cgood(comment_id){
				var good=C.cookie.get(\'good\'+comment_id);
				if(good == null) good =0;
				if(comment_id =="undefined") return false;
				var comment_id = parseInt(comment_id);
				if(!good){
				$.post("comment.php?m=cgood",{comment_id:comment_id},function(data){
					var res = eval("("+data+")");
					$("#good"+comment_id).html(res);
					C.cookie.set(\'good\'+comment_id,comment_id)
				});
				}else{
					alert(\'不好意思，您已经表态啦~~~\');
				}
			}
			function cbad(comment_id){
			var cbad=C.cookie.get(\'good\'+comment_id);
			if(cbad == null) cbad =0;
				if(comment_id =="undefined") return false;
				var comment_id = parseInt(comment_id);
				if(!cbad){
				$.post("comment.php?m=cbad",{comment_id:comment_id},function(data){
					var res = eval("("+data+")");
					$("#bad"+comment_id).html(res);
					C.cookie.set(\'good\'+comment_id);
					
				});
				}else{
					alert(\'不好意思，您已经表态啦~~~\');
				}
			}
			function recomment (comment_id,nick) {
				$("#parent_id").val(comment_id);
				$("#comment_id").val(comment_id);
				$("#comment").val("回复["+nick+"]:");
				$("#comment").focus();
				
			}
		</script>';
        $html .= '</div>';
        return $html;
    }

    public function getcomment_form($type, $id)
    {
        $form = '<style>.comment_ipt{border:1px solid #ccc;height:24px;line-height:24px;width:100px;background:#f7f7f7;}.comment_l{margin-right:8px;}</style>';
        $form .= '<div style="margin-top:10px;">';

        if ($type == 0) {
            $form .= '<input type="hidden" value="0" id="type"/>';
        } else {
            $form .= '<input type="hidden" value="1" id="type" />';
        }
        $form .= '<input type="hidden" value="' . $id . '" id="id" />';
        $form .= '<input type="hidden" value="0" id="parent_id" />';
        $form .= '<div style="line-height:200%;">网友评论仅供网友表达个人看法，并不表明 <strong>' . SITE_NAME . '</strong> 同意其观点或证实其描述</div>';
        $form .= '<div style="margin-top:5px;"><textarea style="width:100%;height:80px;border:1px solid #ccc;font-size:12px;line-height:200%;" name="comment" id="comment"></textarea></div>';
        $form .= '<div style="margin-top:10px;height:40px;">';
        $form .= '  <span class="comment_l">昵称：<input type="text" id="user" value="" class="comment_ipt" /></span>';
        $form .= '  <span class="comment_l">验证码：<input tabindex="4" type="text" id="code" value="" class="comment_ipt" style="width:60px;" /><span>';
        $form .= '  <span class="comment_l"><img title="点击更换验证码" id="chk_code" src="' . SITE_PATH . 'getcode.php?c=feedback" style="width:60px;height:25px;margin:-4px 8px 0 6px;" onclick="this.src=\'' . SITE_PATH . 'getcode.php?c=feedback&t=\'+Math.random();" /><span>';
        $form .= '  <span class="comment_l"><input id="sub" type="button" onclick="sendcomment();" value="发表评论" class="comment_ipt" style="width:auto;height:26px;"/><span>';
        $form .= '</div>';
        $form .= '</div>';
        $form .= '</div>';
        $form .= '<script>
			function showcomment(type,id){
				$.get("comment.php?m=list",{type:type,id:id},function(data){
					
					$("#comment_list").html(data);
					
							
				});	
			}
			function sendcomment(){
			var type = $.trim($("#type").val());
			var parent_id = $("#parent_id").val();
			var comment = $.trim($("#comment").val());
			var id = $("#id").val();
			var user = $("#user").val();
			var code = $.trim($("#code").val());
			$.post("comment.php?m=add",{type:type,parent_id:parent_id,comment:comment,id:id,user:user,code:code},function(data){
				 try{
					var obj = eval("("+data+")");
					if(obj.code > 0){
						C.alert.opacty({"content":"<br>"+obj.msg+""});
                        return;
					}
                    showcomment(type,id);
					$("#chk_code").attr({"src":"' . SITE_PATH . 'getcode.php?c=feedback&t="+Math.random()});
					}catch(e){
						alert(data);
					}
				});
			
			}
		</script>';
        return $form;
    }

    // 敏感词过滤
    function filter_words($_con, $type = '', $isdie = 0)
    {
        $replace_word = array("我靠", "他妈的", "干你娘", "妈的", "我草", "我操"
        );
        $no_word = array("枪支", "弹药", "办证", "假证", "办毕业证", "刻章", "名刀大全", "卖猎枪", "卖气枪", "三唑仑", "男女公关", "男公关", "女公关", "A片", "小姐上门", "裸聊", "氰化钾", "东京热", "苯三酚", "窃听器", "炸药", "高仿身份證", "高仿身份证", "鸭子上门", "成人电影", "小姐服务", "炸弹", "报仇毁容", "批发高仿假钱", "帮人复仇", "打架报仇", "打架杀人", "硫酸铊", "左轮手枪", "军刺", "肝脏", "肝源", "腎源", "肾源", "腎脏", "肾臟", "麻黄素", "氰化钠", "AV", "色片", "发票", "发嘌", "发漂", "fapiao", "海洛因", "冰毒", "代开", "假币", "假钞"
        );
        $no_word_user = array("管理员", "站长", "admin", "易找网", "官方", "客服"
        );

        if ($type == 'user') {
            foreach ($no_word_user as $key) {
                $num = count(explode($key, $_con));
                if ($num > 1) {
                    die('昵称或者用户名不允许字符：' . $key);
                }
            }
        } elseif ($type == 'replace') {
            foreach ($replace_word as $key) {
                $_con = preg_replace('/(' . $key . ')/i', '***', $_con);
            }
        } else {
            // if(preg_match('/(.+)\1{8,}/i',$_con) && $isdie){die('请不要填写8个以上连续相同无意义的字符');}
            // 精准关键词表
            $fword = file_get_contents('./core/filter_word.txt');
            $farr = explode("\n", $fword);
            foreach ($farr as $key) {
                $key = helper :: utf8_substr($key, 0, strlen($key) - 1);
                $num = count(explode($key, $_con));
                if ($num > 1) {
                    die('请不要发布涉黄,毒,枪,色情,制假等非法内容');
                }
            }
            // 宽泛关键词表
            foreach ($no_word as $key) {
                $num = count(explode($key, $_con));
                if ($num > 1) {
                    die('请不要发布涉黄,毒,枪,色情,制假等非法内容');
                }
            }
        }

        return $_con;
    }

    /**
     * 把内容替换为内链
     *
     * @param  $content 内容
     */
    public function nlink($content)
    {
        $sql = "select * from " . TB_PREFIX . "nlink";
        $rs = $this->dbm->query($sql);
        foreach ($rs['list'] as $a) {
            $content = preg_replace('~' . $a['nlink_txt'] . '~', '<a href="' . $a['nlink_url'] . '" target="_blank">' . $a['nlink_txt'] . '</a>', $content, 1);
        }
        return $content;
    }

    /**
     * 根据app_id,info_id取对应的资源
     *
     * @param  $id 资讯或者应用ID
     * @param  $type 类型
     */
    public function get_resource($id, $type)
    {
        if (!is_numeric($id)) return array();
        if (!is_numeric($type)) return array();
        $rs = $this->dbm->query("select * from " . TB_PREFIX . "resource_list where info_app_id='$id' and type='$type'"); //echo($rs['sql'].$rs['sql_time']);
        // 计算缩略图地址
        if (count($rs['list']) > 0) {
            for ($i = 0; $i < count($rs['list']); $i++) {
                $rs['list'][$i]['thumb'] = $this->thumb_url($rs['list'][$i]['resource_url']);
                /**
                 * if(strpos($rs['list'][$i]['resource_url'],'322.com.cn')>0 || strpos($rs['list'][$i]['resource_url'],'appcms.cc')>0){
                 * $rs['list'][$i]['resource_url'] = SITE_PATH."img.php?url=".$rs['list'][$i]['resource_url'];
                 * }
                 */
                //开启图片地址伪静态
                if (defined('IMG_REWRITE') && IMG_REWRITE) {
                    if (substr($rs['list'][$i]['resource_url'], 0, 7) == 'http://') {
                        if (strpos($rs['list'][$i]['resource_url'], '322.com.cn') > 0 || strpos($rs['list'][$i]['resource_url'], 'appcms.cc') > 0) {
                            $shffix = substr($rs['list'][$i]['resource_url'], strrpos($rs['list'][$i]['resource_url'], '.'));
                            //图片地址伪静态
                            $rs['list'][$i]['resource_url'] = SITE_PATH . 'image/' . base64_encode($rs['list'][$i]['resource_url']) . $shffix;

                        }
                    }
                }
            }
        }
        return $rs['list'];
    }

    /**
     * 根据图片地址返回缩略图地址
     */
    public function thumb_url($url)
    {
        $pos = strrpos($url, '/');
        $url = substr($url, 0, $pos + 1) . 'thumb_' . substr($url, $pos + 1, strlen($url) - $pos);
        return $url;
    }

    /**
     * 根据app_id取历史版本
     *
     * @param  $id 资讯或者应用ID
     */
    public function get_history($id)
    {
        if (!is_numeric($id)) return array();
        $rs = $this->dbm->query("select * from " . TB_PREFIX . "app_history where app_id='$id' order by app_update_time asc"); //echo($rs['sql'].$rs['sql_time']);
        for ($i = 0; $i < count($rs['list']); $i++) {
            $rs['list'][$i]['surl'] = $this->url->encode('content_app_history', array('host' => SITE_PATH, 'id' => $id, 'hid' => $rs['list'][$i]['history_id']));
            if (isset($rs['list'][$i]['down_url']) && substr($rs['list'][$i]['down_url'], 0, 7) == 'http://' && $rs['list'][$i]['appcms_history_id'] <= 0) {
                $rs['list'][$i]['appcms_history_id'] = $rs['list'][$i]['history_id'];
            }
        }
        $ret = helper :: ver_sort($rs['list'], 'app_version');
        return $ret;
    }

    /**
     * 采用google API 生成二维码
     *
     * @param  $content 二维码要存储的文字内容
     * @param  $widthHeight 宽高
     * @param  $EC_level 纠错级别
     * @param  $margin 间距
     */
    function google_qrcode($content, $widhtHeight = '150', $EC_level = 'L', $margin = '0')
    {
        $url = urlencode($content);
        echo '<img title="用你的手机扫描这里" src="http://chart.apis.google.com/chart?chs=' . $widhtHeight . 'x' . $widhtHeight . '&cht=qr&chld=' . $EC_level . '|' . $margin . '&chl=' . $content . '"/>';
    }

    /**
     * 采用联图 API 生成二维码
     *
     * @param $context 二维码要存储的文字内容
     * @param int $size 填写二维码尺寸（像素数），默认值：150
     * @param null $logo 二维码中心 Logo 的链接，默认值：null
     * @param bool $cache 是否缓存，默认值：true
     * @param string $name 缓存后的文件名，必须在 $cache 参数为 true 的时候才有效，默认值：'qrcode'
     * @return string
     */
    function liantu_qcode($context, $widhtHeight = 150, $logo = null)
    {
        $logo ? $qrlogo = '&el=h&logo=' . $logo : $qrlogo = '';
        $imgurl = 'http://qr.liantu.com/api.php?text=' . $context . '&w=' . $widhtHeight . '&m=5' . $qrlogo;
        echo '<img title="用你的手机扫描这里" src="' . $imgurl . '"/>';

    }


    /**
     * 根据位置ID获取（广告，专题，推荐）
     * 如果是广告，则输出广告代码，如果是专题或者推荐信息则输出列表
     *
     * @params 参数数组
     * @param  $area_id 位置ID
     * @param  $pagesize 分页大小
     * @param  $p 当前分页
     * @param  $count 是否统计分页
     * @param  $where 查询条件
     * @params $resource 去资源图片列表
     * @params $history 取历史版本
     * @params $order 排序
     */
    function get_recommend($params)
    {
        $area_id = isset($params['area_id']) ? $params['area_id'] : 0;
        $pagesize = isset($params['pagesize']) ? $params['pagesize'] : PAGESIZE;
        $p = isset($params['p']) ? $params['p'] : 1;
        $count = isset($params['count']) ? $params['count'] : 0;
        $where = isset($params['where']) ? $params['where'] : '';
        $rewrite = isset($params['rewrite']) ? $params['rewrite'] : 2;
        $resource = isset($params['resource']) ? $params['resource'] : 0;
        $history = isset($params['history']) ? $params['history'] : 0;
        $order = isset($params['order']) ? $params['order'] : '';

        if (!is_numeric($area_id)) return array();
        if ($area_id == 0) { // 如果位置ID=0，则取列表
            $recommend_list = array();
            $sql = "select * from " . TB_PREFIX . "recommend_area ";
            if ($where != '') {
                $sql .= " where $where";
            }
            $suffix = " order by area_order desc,area_id desc";
            // 拼接分页数据
            $suffix .= " limit " . ($p - 1) * $pagesize . ",$pagesize";
            $result = $this->dbm->query($sql, $suffix, $count); //print_r($result);die;
            for ($i = 0; $i < count($result['list']); $i++) {
                $result['list'][$i]['surl'] = $this->url->encode('special_content', array('host' => SITE_PATH, 'id' => $result['list'][$i]['area_id']));
                if ($result['list'][$i]['area_html'] == '') {
                    $result['list'][$i]['area_html'] = $result['list'][$i]['title'] . '<br>查看' . $result['list'][$i]['title'] . '详情';
                } else {
                    $result['list'][$i]['area_html'] = preg_replace('~<[^>]*>~', '', $result['list'][$i]['area_html']);
                }
            }
            $total = $result['total'];
            // 分页码HTML
            if ($rewrite == 0 || $rewrite == 1) {
                // 前台使用动态地址或者伪静态地址
                if ($this->url != null) {
                    $result['pagebar'] = helper :: pagebar(
                        array('total' => $total, 'pagesize' => $pagesize, 'rewrite' => $rewrite,
                            'rule' =>
                                array('node' => 'special_list',
                                    'obj' => $this->url,
                                    'params' =>
                                        array('host' => SITE_PATH,
                                            'p' => $p
                                        ),
                                )
                        )
                    );
                } else {
                    $result['pagebar'] = array('pagecode' => '');
                }
            } else {
                $result['pagebar'] = helper :: pagebar(array('total' => $total, 'pagesize' => $pagesize, 'rewrite' => $rewrite));
            }
            return $result;
        } else { // 如果位置ID大于0，则取某个位置的数据
            $sql = "select * from " . TB_PREFIX . "recommend_area where area_id='$area_id' limit 1";
            $rs = $this->dbm->query($sql); //print_r($rs);die;
            $recommend = Array('area_id' => 0,
                'title' => '',
                'area_type' => -1,
                'area_order' => 0,
                'id_list' => 0,
                'surl' => 'javascript:void(0);',
                'list' => Array(
                    Array('app_id' => 0,
                        'last_cate_id' => 0,
                        'app_title' => '',
                        'app_stitle' => '',
                        'app_logo' => '',
                        'app_order' => 0,
                        'app_down' => 0,
                        'cate' => Array('cname' => '',
                        ),
                        'surl' => '',
                    ),
                ),
            );
            if (count($rs['list']) == 1) {
                $recommend = $rs['list'][0]; //print_r($recommend);
                $recommend['surl'] = $this->url->encode('special_content', array('host' => SITE_PATH, 'id' => $recommend['area_id']));
                if ($recommend['area_type'] == 2) { // 广告
                    if (preg_match('~^\{[\w\W]*?\}$~', $recommend['area_html'])) {
                        $recommend['list'] = unserialize($recommend['area_html']);
                    }
                } else { // 专题，推荐
                    if ($recommend['area_type'] == 0 || $recommend['area_type'] == 3) {
                        $type = 0;
                        $tbname = 'app';
                    } elseif ($recommend['area_type'] == 1 || $recommend['area_type'] == 4) {
                        $type = 1;
                        $tbname = 'info';
                    } else {
                        return array();
                    }
                    if ($order == '') $order = ' order by field(' . $tbname . '_id,' . $recommend['id_list'] . ')';
                    $list = $this->get_list(array('type' => $type, 'where' => $tbname . '_id in (' . $recommend['id_list'] . ')', 'order' => $order, 'history' => $history, 'resource' => $resource, 'pagesize' => $pagesize));
                    //print_r($list);die;
                    $recommend['list'] = $list['list'];
                }
            }
            //exit(print_r($recommend));
            return $recommend;
        }
    }

    /**
     * 输出广告HTML代码，只需要传入广告位ID，自动输出代码广告或者图片轮播广告，轮播只有一张图片直接输出图片广告；多张需要配合templates/lib/nbspslider-1.1的幻灯片效果JS才能起幻灯片效果，也可以自己编写
     *
     * @param area_id $ 广告位ID
     */
    public function get_ad($area_id)
    {
        $area_html = $this->get_recommend(array('area_id' => $area_id));
        $ad = unserialize($area_html['area_html']); //print_r($ad);
        if ($ad['type'] == '1') { // 代码广告，直接输出
            echo($ad['area_html']);
        } elseif ($ad['type'] == '2') { // 图片轮播
            if (count($ad['area_html']) == 1) { // 一张图片直接输出
                echo('<a href="' . $ad['area_html'][0]['url'] . '" target="_blank"><img src="' . $ad['area_html'][0]['resource_url'] . '" border="0"></a>');
            } else { // 多张图片显示为幻灯片，包含了默认幻灯的JS、CSS文件才会显示为幻灯片，否则就只是图片列表输出
                foreach ($ad['area_html'] as $a) {
                    echo('<li>');
                    echo('<a href="' . $a['url'] . '" target="_blank">');
                    $alt = $a['title'];
                    if ($a['desc'] != '') $alt .= '<br>' . $a['desc'];
                    echo('<img src="' . $a['resource_url'] . '" border="0" alt="' . $alt . '">');
                    echo('</a>');
                    echo('</li>');
                }
            }
        } else {
            echo('广告位类型不存在');
        }
    }

    /**
     * 获取友情链接，返回html文本形式
     *
     * @param  $tpl 当前页面获取到的调用模板页面的参数（$_GET['tpl']），判断友情链接是否首页需要使用
     * @param  $class 判断返回的是文本链接还是图片链接
     */
    public function get_flink($tpl = "index", $class = "flink_txt")
    {
        if (!isset($_GET['tpl']) || $_GET['tpl'] == 'index') { // 展示首页的
            $where = " flink_is_site='0' or flink_is_site ='1'"; //0首页，1全站
        } else { // 全站的
            $where = " flink_is_site !='0'";
        }
        // 获取链接
        $flink = $this->dbm->single_query(array('table_name' => TB_PREFIX . "flink", 'where' => $where));
        if (empty($flink['error'])) {
            // 判断返回的html代码形式
            $html_flink = "";
            if ($class == "flink_txt") { // 返回文字链接
                foreach ($flink['list'] as $k => $v) {
                    if ($k > 0) $html_flink .= '';
                    $html_flink .= '<a href="' . $v['flink_url'] . '" target="_blank">' . $v['flink_name'] . '</a>';
                }
            } else { // 返回图片链接
                foreach ($flink['list'] as $k => $v) {
                    $html_flink .= '<a href="' . $v['flink_url'] . '" target="_blank"><img src="' . $v['flink_img'] . '" border="0" />' . $v['flink_name'] . '</a>';
                }
            }
        }
        return $html_flink;
    }

    /**
     * 更新访问量
     *
     * @param  $id 资讯或者应用ID
     * @param  $type 文章类型：0=应用，1=资讯
     */
    public function update_vistor($id, $type = 0)
    {
        if ($type == 0) {
            $sql = "update " . TB_PREFIX . "app_list set app_visitors=`app_visitors`+1 where app_id='" . $id . "'";
            $res = $this->dbm->query_update($sql);
        } else {
            $sql = "update " . TB_PREFIX . "info_list set info_visitors=`info_visitors`+1 where info_id='" . $id . "'";
            $res = $this->dbm->query_update($sql);
        }
        /**
         * 更新分类浏览量
         */
        $sql = "update " . TB_PREFIX . "cate_relation set id_visitors=`id_visitors`+1 where id='" . $id . "' and type ='" . $type . "'";
        $res = $this->dbm->query_update($sql);
    }

    /**
     * 直接输出<s></s>格式，一个这样的标签标示一个星星
     *
     * @param  $recomment 系统推荐分数
     */
    public function get_star($recomment, $style = "")
    {
        $rec = ceil($recomment / 2);
        switch ($rec) {
            case 1:
                echo "<s class=" . $style . "></s>";
                break;
            case 2:
                echo "<s class=" . $style . "></s><s class=" . $style . "></s>";
                break;
            case 3:
                echo "<s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s>";
                break;
            case 4:
                echo "<s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s>";
                break;
            case 5:
                echo "<s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s>";
                break;
            default:
                echo "<s class=" . $style . "></s><s class=" . $style . "></s><s class=" . $style . "></s>";
                break;
        }
    }
	/**
 	* 取出html标签
 	* 
 	* @access public
 	* @param string str
 	* @return string
 	* 
 	*/
	function deletehtml($str) {
    	$str = trim($str); //清除字符串两边的空格
    	$str = strip_tags($str,""); //利用php自带的函数清除html格式。保留P标签
    	$str = preg_replace("/\t/","",$str); //使用正则表达式匹配需要替换的内容，如：空格，换行，并将替换为空。
    	$str = preg_replace("/\r\n/","",$str); 
    	$str = preg_replace("/\r/","",$str); 
    	$str = preg_replace("/\n/","",$str); 
    	$str = preg_replace("/ /","",$str);
    	$str = preg_replace("/  /","",$str);  //
		$str = preg_replace("/\&/","",$str);  //匹配html中的空格
		return trim($str); //返回字符串
	}
	/**
     * 取资讯或者应用列表
     *
     * @params 为数组格式
     * @param  $ =>type    0 应用,1 资讯 默认为 0
     * @param  $ =>cate_id         分类ID
     * @param  $ =>where          更新的条件语句
     * @param  $ =>order          排序
     * @param  $ =>pagesize       分页大小
     * @param  $ =>p              当前页码
     * @param  $ =>rewrite        URL重写方式 0,1,2，默认为2
     * @param  $ =>count          是否计算总数 0 不计算 1 计算
     * @param  $ =>resource       是否同时取出资源 0 不取 1 取
     * @param  $ =>history   是否同时取出版本 0 不取 1 取
     * @param  $ =>fields     查询的字段
     */

    public function get_randlist($params, $no_cache = 0)
    {
        $tbname = 'app';
        $cate_id = isset($params['cate_id']) ? $params['cate_id'] : '1';
        $where = isset($params['where']) ? $params['where'] : '';
        $order = isset($params['order']) ? $params['order'] : ' order by ' . $tbname . '_update_time desc';
        $pagesize = isset($params['pagesize']) ? $params['pagesize'] : PAGESIZE;
        $p = isset($params['p']) ? $params['p'] : 1;
        $count = isset($params['count']) ? $params['count'] : 0;
        $rewrite = isset($params['rewrite']) ? $params['rewrite'] : 2;
        $fields = isset($params['fields']) ? $params['fields'] : '';
        $node = isset($_GET['tpl']) ? $_GET['tpl'] : 'index';
        $strip_tags_desc = isset($params['strip_tags_desc']) ? $params['strip_tags_desc'] : 1;
        if (isset($_GET['cpy'])) $node = 'cpy_' . $node;

        if ($fields == '') {
            $fields = '*';
        } else {
            $fields_arr = explode(',', $fields);
            $fields = '';
            for ($i = 0; $i < count($fields_arr); $i++) {
                $fields .= ',' . trim($fields_arr[$i]);
            }
            $fields = substr($fields, 1);
        }
		$result['list']=array();
        $total = 0; //总数                                  
        // 拼接SQL语句
		$order = preg_replace('~' . $tbname . '_~', 'id_', $order);
        $user_index = ''; //echo($order);
        if (strpos($order, 'id_update_time') > 0 && strpos($order, 'id_order') <= 0) $user_index = ' use index(id_update_time) ';
        if (strpos($order, 'id_down') > 0 && strpos($order, 'id_order') <= 0) $user_index = ' use index(id_down) ';
        $sql1 = "select * from " . TB_PREFIX . "cate_relation as a $user_index left join " . TB_PREFIX . $tbname . "_list as b on a.id=b." . $tbname . "_id where cate_id='$cate_id' LIMIT ".rand(1,50)*100 .",100 ";
        $sql="SELECT * FROM (".$sql1.") AS t1 JOIN (SELECT ROUND(RAND() * ((SELECT MAX(id) FROM (".$sql1.") as b1)-(SELECT MIN(id) FROM (".$sql1.") as b2))+(SELECT MIN(id) FROM (".$sql1.") as b3)) AS id) AS t2 WHERE t1.id >= t2.id ORDER BY t1.id LIMIT 1; ";
		for($i=0;$i < $count; $i++)
		{
			$result1 = $this->dbm->query($sql);
			if(count($result1['list'])>0)
				$result['list'][$i]=$result1['list'][0];
		}
        
        // 重写返回数组
        for ($i = 0; $i < count($result['list']); $i++) {
            // 分类名
            if ($cate_id == '') {
                // print_r($result['list'][$i]['last_cate_id']);
                // print_r($this -> categories[$result['list'][$i]['last_cate_id']]);
                $result['list'][$i]['cate'] = $this->categories[$result['list'][$i]['last_cate_id']];
            } else {
                $result['list'][$i]['cate'] = $this->categories[$cate_id];
            }
            // 缩略图
            if (isset($result['list'][$i]['info_img'])) $result['list'][$i]['thumb'] = $this->thumb_url($result['list'][$i]['info_img']);
            if (isset($result['list'][$i]['app_logo'])) {
                $result['list'][$i]['thumb'] = $this->thumb_url($result['list'][$i]['app_logo']);
                //开启图片地址伪静态
                if (defined('IMG_REWRITE') && IMG_REWRITE) {
                    if (substr($result['list'][$i]['app_logo'], 0, 7) == 'http://') {
                        if (strpos($result['list'][$i]['app_logo'], '322.com.cn') > 0 || strpos($result['list'][$i]['app_logo'], 'appcms.cc') > 0) {
                            //图片地址伪静态
                            $shffix = substr($result['list'][$i]['app_logo'], strrpos($result['list'][$i]['app_logo'], '.'));
                            $result['list'][$i]['app_logo'] = SITE_PATH . 'image/' . base64_encode($result['list'][$i]['app_logo']) . $shffix;
                        }
                    }
                }

            }
            // URL
            $result['list'][$i]['surl'] = $this->url->encode('content_' . $tbname, array('host' => SITE_PATH, 'id' => $result['list'][$i][$tbname . '_id'], 'rewrite_tag' => @$result['list'][$i]['rewrite_tag']));

        }

        return $result;
    }
	
	public function postbaidu($params)
	{
		$urls = $params;
		$api = BAIDU_SUBMIT;
		$ch = curl_init();
		$options =  array(
    		CURLOPT_URL => $api,
    		CURLOPT_POST => true,
    		CURLOPT_RETURNTRANSFER => true,
    		CURLOPT_POSTFIELDS => implode("\n", $urls),
    		CURLOPT_HTTPHEADER => array('Content-Type: text/plain'),
		);
		curl_setopt_array($ch, $options);
		$result = curl_exec($ch);
		return $result;
	}
	
}

?>
