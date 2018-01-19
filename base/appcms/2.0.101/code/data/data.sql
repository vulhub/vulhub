create table if not exists `appcms_app_list` (
 `app_id` int(11) unsigned not null auto_increment comment '应用id',
 `last_cate_id` int(11) not null default 0 comment '终极分类ID',
 `app_title` varchar(500) not null default '' comment '应用名称',
 `app_stitle` varchar(500) not null default '' comment 'SEO标题',
 `app_version` varchar(500) not null default '' comment '版本号（最新）',
 `app_update_time` int(11) not null default 0 comment '更新时间（最新）',
 `app_size` varchar(50) not null default '' comment '程序大小（最新）',
 `app_system` varchar(100) not null default '' comment '系统要求（最新）',
 `app_type` varchar(20) not null default '' comment '软件类型：免费/收费',
 `app_logo` varchar(500) not null default '' comment '缩略图',
 `app_desc` text null comment '应用详情',
 `app_company` varchar(500) not null default '' comment '开发商',
 `app_company_url` varchar(500) not null default '' comment '开发商网址',
 `app_tags` varchar(1000) not null default '' comment '标签',
 `create_time` int(11) not null default 0 comment '入库时间',
 `app_grade` float not null default 0 comment '用户评分',
 `app_recomment` float not null default 0 comment '系统评分',
 `uid` int(11) not null default 0 comment '发布人ID',
 `uname` varchar(100) not null default '' comment '发布人',
 `app_comments` int(11) not null default 0 comment '评论量',
 `app_visitors` int(11) not null default 0 comment '浏览量',
 `app_down` int(11) not null default 0 comment '下载量',
 `app_order` int(11) not null default 0 comment '排序',
 `data_app_id` int(11) not null default 0 comment '数据中心应用ID',
 `charge_app_id` int(11) not null default 0 comment '独立计费包ID',
 `rewrite_tag` varchar(100) NOT NULL DEFAULT '' COMMENT 'app页面伪静态标签',
 `seo_title` text NULL DEFAULT '' COMMENT '应用SEO标题',
 `seo_keywords` text NULL DEFAULT '' COMMENT '应用SEO关键字',
 `seo_desc` text NULL DEFAULT '' COMMENT '应用SEO描述',
 primary key (`app_id`),
 index `cate_update_time` (`last_cate_id`,`app_update_time`),
 index `cate_order` (`last_cate_id`,`app_order`),
 index `cate_visitor` (`last_cate_id`,`app_visitors`),
 index `cate_create_time` (`last_cate_id`,`create_time`),
 index `cate_down` (`last_cate_id`,`app_down`),
 index `app_update_time` (`app_update_time`),
 index `app_order` (`app_order`),
 index `app_down` (`app_down`)
) engine=myisam default charset=utf8 comment '应用表';

create table if not exists `appcms_app_history` (
 `history_id` int(11) unsigned not null auto_increment comment 'app下载id',
 `app_id` int(11) not null default 0 comment '应用ID',
 `app_version` varchar(500) not null default '' comment '版本号',
 `app_update_time` int(11) not null default 0 comment '更新时间',
 `app_size` varchar(500) not null default '' comment '程序大小',
 `app_system` varchar(500) not null default '' comment '系统要求',
 `appcms_history_id` int(11) not null default 0 comment 'APPCMS数据中心下载ID',
 `down_url` varchar(500) not null default '' comment '下载地址',
 primary key (`history_id`),
 index app_id(`app_id`),
 index appcms_history_id(`appcms_history_id`)
) engine=myisam default charset=utf8 comment '应用历史版本表';

create table if not exists `appcms_search_keyword` (
  `id` int(11) unsigned null null auto_increment,
  `q` varchar(200) not null default '' comment '搜索关键字',
  `qnum` int(11) unsigned not null default 0 comment '搜索次数',
  `qorder` int(11) unsigned not null default 0 comment '关键字排序',
   primary key (`id`),
  index `qorder` (`qorder`),
  index `q` (`q`)
) engine=myisam default charset=utf8 comment='搜索关键字记录表';

create table if not exists `appcms_resource_list` (
 `id` int(11) unsigned not null auto_increment comment '资源',
 `info_app_id` int(11) not null default 0 comment '应用 或资讯ID',
 `type` int(2) not null default 0 comment '关联的类型，应用=0，资讯=1',
 `resource_url` varchar(300) not null default '' comment '资源地址', 
 `width` int(11) not null default 0 comment '资源宽度',
 `height` int(11) not null default 0 comment '资源高度',
 `size` int(11) not null default 0 comment '资源大小',
 primary key (`id`),
 index id_type(`info_app_id`,`type`),
 index resource_url(`resource_url`)
) engine=myisam default charset=utf8 comment '资源表';

create table if not exists `appcms_info_list` (
 `info_id` int(11) unsigned not null auto_increment comment '资讯id',
 `last_cate_id` int(11) not null default 0 comment '终极分类ID',
 `info_title` varchar(500) not null default '' comment '标题',
 `info_stitle` varchar(500) not null default '' comment 'SEO标题',
 `info_img` varchar(500) not null default '' comment '缩略图',
 `info_desc` varchar(500) not null default '' comment '摘要',
 `info_body` text null comment '详情',
 `info_tags` varchar(1000) not null default '' comment '标签',
 `info_update_time` int(11) not null default 0 comment '更新时间',
 `create_time` int(11) not null default 0 comment '创建时间',
 `info_from` varchar(500) not null default '' comment '来源',
 `uid` int(11) not null default 0 comment '发布人ID',
 `uname` varchar(100) not null default '' comment '发布人',
 `info_comments` int(11) not null default 0 comment '评论量',
 `info_visitors` int(11) not null default 0 comment '浏览量',
 `info_order` int(11) not null default 0 comment '排序',
 `info_url` varchar(500) not null default '' comment '外部URL',
 primary key (`info_id`),
 index `cate_update_time` (`last_cate_id`,`info_update_time`),
 index `cate_order` (`last_cate_id`,`info_order`),
 index `cate_visitor` (`last_cate_id`,`info_visitors`),
 index `cate_create_time` (`last_cate_id`,`create_time`),
 index `info_update_time` (`info_update_time`),
 index `info_order` (`info_order`)
) engine=myisam default charset=utf8 comment '资讯表';

create table if not exists `appcms_comment` (
  `comment_id` int(11) unsigned not null auto_increment comment '评论id',
  `id` int(11) not null default 0 comment '应用id，资讯id',
  `type` int(2) not null default 0 comment '评论类型：应用=0，资讯=1',
  `content` varchar(500) not null default '' comment '评论内容',
  `date_add` int(11) not null default 0 comment '发布时间',
  `uid` int(11) not null default 0 comment '用户id',
  `uname` varchar(500) not null default '' comment '昵称',
  `ip` varchar(20) not null default 0 comment 'ip地址',
  `ip_addr` varchar(200) not null default '' comment '地理位置',
  `parent_id` int(11) not null default 0 comment '上级id',
  `is_check` int(1) not null default 1 comment '是否审核',
  `son` int(11) not null default 0 comment '子评论数',
  `good` int(11) not null default 0 comment '赞',
  `bad` int(11) not null default 0 comment '踩',
  primary key  (`comment_id`),
  index id_type(`id`,`type`)
) engine=myisam default charset=utf8 comment '评论表';

create table if not exists `appcms_category` (
 `cate_id` int(11) unsigned not null auto_increment comment '类别id',
 `parent_id` int(11) not null default 0 comment '父类id',
 `cname` varchar(100) not null default '' comment '分类名称',
 `cname_py` varchar(100) not null default '' comment '分类字母别名',
 `ctitle` varchar(500) not null default '' comment 'SEO标题',
 `ckey` varchar(500) not null default '' comment 'SEO关键词',
 `cdesc` varchar(500) not null default '' comment 'SEO描述',
 `corder` int(11) not null default 0 comment '分类排序',
 `cimg` varchar(100) not null default '' comment '分类图片',
 `cat_show` int(2) not null default '1' comment '是否显示分类',
 `tpl_index` varchar(500) not null default '' comment  '封面模板',
 `tpl_listvar` varchar(500) not null default '' comment  '列表变量模板',
 `tpl_content` varchar(500) not null default '' comment  '内容页模板',
 `type` int(2) not null default 0 comment  '分类类型：应用=0，资讯=1',
 `cdata` int(11) not null default 0 comment  '分类下的数据',
 primary key (`cate_id`)
) engine=myisam default charset=utf8 comment '分类表';

create table if not exists `appcms_cate_relation` (
 `relation_id` int(11) unsigned not null auto_increment comment '关系ID',
 `cate_id` int(11) not null default 0 comment '分类ID',
 `type` int(1) not null default 0 comment '类型 0=应用 1=资讯',
 `id` int(11) not null default 0 comment '资讯ID，应用ID', 
 `id_down` int(11) not null default 0 comment '下载量',
 `id_order` int(11) not null default 0 comment '排序',
 `id_update_time` int(11) not null default 0 comment '更新时间',
 `id_create_time` int(11) not null default 0 comment '创建时间',
 `id_visitors` int(11) not null default 0 comment '访问量',
 primary key (`relation_id`),
  index `cate_id` (`cate_id`),
  index `id_update_time` (`cate_id`,`id_update_time`),
  index `id_down` (`cate_id`,`id_down`),
  index `id_order` (`cate_id`,`id_order`),
  index `order_id_update` (`cate_id`,`id_order`,`id_update_time`),
  index `order_id_down` (`cate_id`,`id_order`,`id_down`),
  index `id_visitors` (`cate_id`,`id_visitors`),
  index `id_create_time` (`cate_id`,`id_create_time`)
) engine=myisam default charset=utf8 comment '分类关系表';

create table if not exists `appcms_flink` (
 `flink_id` int(11) unsigned not null auto_increment comment '友情ID',
 `flink_name` varchar(100) not null default '' comment '链接文字',
 `flink_img` varchar(500) not null default '' comment '链接图片',
 `flink_url` varchar(500) not null default '' comment '链接地址',
 `flink_is_site` int(2) default 0 comment '0=首页，1代表全站',
 `flink_order` int(11) default 0 comment'排序',
 primary key(`flink_id`)
)engine=myisam default charset=utf8 comment '友情链接表';

create table if not exists `appcms_nlink`(
  `nlink_id` int(11) unsigned not null auto_increment comment '内链ID',
  `nlink_txt` varchar(100)  not null default '' comment '名称',
  `nlink_url` varchar(500)  not null default '' comment '网址',
   primary key(`nlink_id`)
)engine=myisam default charset=utf8 comment '正文內链词表';

create table if not exists `appcms_recommend_area` (
 `area_id` int(11) unsigned not null auto_increment,
 `title` varchar(100) not null default '' comment '位置标题',
 `area_type` int(2) not null default 0 comment '位置类型：推荐位（应用）=0，推荐位（资讯）=1，广告=2，专题（应用）=3，专题（资讯）=4',
 `area_html` text null comment '广告HTML或者描述文本',
 `area_remarks` varchar(1000) not null default '' comment '备注',
 `area_logo` varchar(200) not null default '' comment '位置LOGO图',
 `area_order` int(11) not null default 0 comment '排序',
 `id_list` varchar(2000) not null default '' comment '应用或者资讯ID列表，用逗号分割',
 primary key (`area_id`)
) engine=myisam default charset=utf8 comment '推荐位置（广告，推荐位，专题）';

create table if not exists `appcms_admin_list` (
 `uid` int(11) unsigned not null auto_increment,
 `uname` varchar(100) not null default '' comment '账号',
 `upass` varchar(100) not null default '' comment '密码',
 `ustate` int(2) not null default 0 comment '用户状态（正常=0，停用=1）',
 `reg_date` int(11) not null default 0 comment '开通时间',
 primary key (`uid`)
) engine=myisam default charset=utf8 comment '后台管理表';

create table if not exists `appcms_url_rewrite`(
 `url_id` int(11) not null auto_increment,
 `url_title` varchar(400) not null default '' comment '地址名称',
 `url_true` varchar(400) not null default '' comment '真实地址',
 `url_rule` varchar(400) not null default '' comment '伪静态规则',
 primary key(`url_id`)
) engine=myisam default charset=utf8 comment '伪静态规则表';
