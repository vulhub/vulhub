<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no">
<title><?php if(defined('SEO_TITLE') && SEO_TITLE != ''){ echo SEO_TITLE;}else{echo SITE_NAME.' - 安卓手机游戏下载|安卓应用市场';}?></title>
<link rel="stylesheet"  href="<?php echo SITE_PATH;?>templates/m/css/style.css"  type="text/css" />
<script type="text/javascript" src="<?php echo SITE_PATH;?>templates/m/css/js/comm.js"></script>
</head>
<body>
	<div class="box wrap">
		<!-- 头部内容 开始 -->
			<?php require_once ('inc_head.php');?>
		<!-- 头部内容 结束 -->
		<p class="line-t-10"></p>
		<div class="box wrap-content">
			<div class="box app-area">
				<ul class="app-list" id="app_url">
				<?php
				$a = $c->get_recommend(array('area_id'=>0,'pagesize'=>5,'count'=>0,'rewrite'=>SITE_REWRITEURL,'where'=>'area_type=0','order'=>'order by area_order desc'));//print_r($a);//推荐位?>
					<?php if(!empty($a['list'])){foreach($a['list'] as $k=> $v){?>
					<?php
					$b = $c->get_list(array('count'=>0,'rewrite'=>SITE_REWRITEURL,'where'=>'app_id in ('.$v['id_list'].')','order'=>'order by app_order desc,app_update_time desc'));//print_r($b);//推荐位的详细?>
					<p class="line-t-10"></p>
					<div class="box box-tit"><div><?php echo $v['title'];?></div></div>
					<p class="line-t-10"></p>
					<?php if(!empty($b['list'])){foreach($b['list'] as $k2=> $v2){?>
					<li class="box" url="<?php echo 'http://'.$_SERVER['HTTP_HOST'].$v['surl'];?>" onclick="locaUrl(this)">
						<a class="app-img" href="<?php echo $v2['surl'];?>">
							<img src="<?php echo $v2['app_logo'];?>" border="0">
						</a>
						<div class="app-info">
							<span class="app-name">
								<a href="<?php echo $v2['surl'];?>" title="<?php echo $v2['app_title'];?>"><?php echo $v2['app_title'];?></a>
							</span>
							<span class="b-v">大小：<?php echo $v2['app_size'];?> &nbsp;&nbsp; 版本：<?php echo $v2['app_version'];?></span>
							<div class="l level"><span class="<?php echo "l".ceil($v2['app_recomment']/2); ?>"></span></div>
						</div>
					</li>
					<?php } }else{ echo '<div class="h788"><span></span></div>';}?>
					<?php } }else{ echo '<div class="h788"><span></span></div>';}?>
				</ul>
			</div>
		</div>
		<!-- 底部内容 开始 -->
			<?php require_once ('inc_foot.php');?>
		<!-- 底部内容 结束 -->
	</div>
</body>
</html>