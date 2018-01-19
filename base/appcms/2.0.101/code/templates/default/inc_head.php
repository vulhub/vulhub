<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/common.js"></script>
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/search.js"></script>
<script type="text/javascript">
/*搜索*/
function do_search(){
    var searchvalue = $("#seach-txt").val();
    if(searchvalue==''){return false;}
    window.location.href="<?php echo SITE_PATH?>index.php?tpl=search&q="+encodeURIComponent(searchvalue);
}
</script>
</head>
<body>
<div class="warp">
    <div class="warp-top"> <!-- 顶部  开始-->
        <div class="top-bar-area">
            <div class="marauto">
               <div class="l bar-left" style="color:#aaa;">
                    <a class="web-name" href="<?php echo SITE_URL;?>"><?php echo SITE_NAME;?></a>&nbsp;&nbsp;&nbsp;&nbsp;<?php $a=$c->get_app_info_count();echo('应用：'.$a['app_total'].' 当日：'.$a['app_today'].' 新闻：'.$a['info_total'].' 当日：'.$a['info_today'].' '); ?>
                </div>
                <div class="r bar-right">
                    <a href="<?php echo SITE_PATH."index.php?tpl=rss"?>" >RSS</a>&nbsp;&nbsp;&nbsp;
                    <a href="javascript:void(0);" onclick="C.fav('<?php echo SITE_NAME;?>','<?php echo SITE_URL;?>');" id="collecttion">收藏本站</a>&nbsp;&nbsp;&nbsp;
                    <a href="javascript:void(0);" onclick="C.sethome('<?php echo SITE_URL;?>');" id="home-page">设为首页</a>
                </div>
          </div>
        </div>
        <div class="top-head">
            <div class="marauto">
                <a href="javascript:void(0);" class="l web-logo"><?php if(defined('SITE_LOGO') && SITE_LOGO != ''){ echo '<img border="0" src="'.SITE_LOGO.'">';}?></a>
                <ul class="r head-seach">
                    <li class="seach-bar">
                        <p class="bdr-span">
                            <input type="text" class="l seach-txt" id="seach-txt" onkeyup="search_up(this,event)" onclick="show_hot(this);" onkeydown="search_down(this,event); if(event.keyCode==13) do_search();" value="<?php if(isset($_GET['q'])) echo $_GET['q'];?>"/>
                            <a href="javascript:void(0);" class="l seach-btn" onClick="do_search()">搜应用</a>
							
                        </p>
                    </li>
					<li class="keyword">
						热门搜索：
                        <?php foreach ($c -> get_keyword() as $v) { ?>
                        <a href="/index.php?tpl=search&q=<?php echo $v;?>"><?php echo $v;?></a>
                        <?php } ?>
					</li>
                </ul>
            </div>
        </div>
        <div class="top-nav">
            <div class="pr marauto">
                <ul class="ul-nav">
                    <li><a href="<?php echo SITE_PATH;?>" <?php if($cid<=0 && !strstr($_SERVER['QUERY_STRING'],'special') && !isset($topid) && !strstr($_SERVER['QUERY_STRING'],'lastupdate')) echo 'class="selected"';?>>首页</a></li>
                    <?php //print_r($c->categories);
                        foreach($c->categories as $a){
                            if($a['parent_id']=='0'){//只输出顶级分类
                                $style='';
                                if((intval($cid)>0 && ($a['cate_id'] == $cid || $a['cate_id']==$c->categories[$cid]['parent_id'])) || (isset($topid) && $topid == $a['cate_id'])) $style='class="selected"';
                               if($a['cat_show'] == 1) echo('<li><a href="'.$a['surl'].'" '.$style.'>'.$a['cname'].'</a></li>');
                            }
                        }
                    ?>
                    <li>
                        <a href="<?php echo $c->url->encode('special_list', array('host' => '/','p' => 1));?>" <?php if(strstr($_SERVER['QUERY_STRING'],'special')) echo 'class="selected"';?>>专题 </a>
                    </li>
                    <li>
                        <a href="<?php echo $c->url->encode('lastupdate', array('host' => '/','p' => 1));?>" <?php if(strstr($_SERVER['QUERY_STRING'],'lastupdate')) echo 'class="selected"';?>>最新</a>
                    </li>
                </ul>
            </div>
        </div>
    </div><!-- 顶部 结束-->
