<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no">
<title>专题 - <?php echo SITE_NAME;?></title>
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
				<!-- 专题列表 开始 -->
				<?php $a = $c->get_recommend(array('pagesize'=>PAGESIZE,'count'=>1,'rewrite'=>SITE_REWRITEURL,'where'=>' area_type=3','p'=>$p));?>
				<ul class="app-list special-list" id="app_url">
					<?php foreach($a['list'] as $v):?>
					<li class="box" url="<?php echo 'http://'.$_SERVER['HTTP_HOST'].$v['surl'];?>" onclick="locaUrl(this)">
						<a class="app-img" href="<?php echo $v['surl'];?>">
							<img src="<?php echo $v['area_logo'];?>" border="0">
						</a>
						<div class="app-info">
							<span class="app-name"><a href="<?php echo $v['surl'];?>" title="<?php echo $v['title'];?>"><?php echo $v['title'];?></a></span>
							<span><?php echo $v['area_html'];?></span>
						</div>
					</li>
					<?php endforeach;?>
				</ul>
				<!-- 专题列表 结束 -->
			</div>
			<div class="pager">
				<?php if(!empty($a['list']))echo $a['pagebar']['pagecode']?>
			</div>
		</div>
		<!-- 底部内容 开始 -->
			<?php require_once ('inc_foot.php');?>
		<!-- 底部内容 结束 -->
	</div>
</body>
</html>