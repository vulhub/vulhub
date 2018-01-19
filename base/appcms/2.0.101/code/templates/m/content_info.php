<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no">
<title><?php echo $con['info_title'].'免费下载|'.$con['info_title'].'手机版下载 - '.SITE_NAME;?></title>
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
			<div class="box box-tit">
			<div>
			<!-- 导航 开始 -->
			<?php
				$nav = "";
				foreach($c->cate_father($con['last_cate_id']) as $n){
                 $nav .= '<a href="'.$n['surl'].'">'.$n['cname'].'</a>&nbsp;»&nbsp;';
				}
				echo $nav;
			?>
			<?php echo $con['info_title'];?>
			<!-- 导航 结束 -->
			</div>
			</div>
			<p class="line-t-10"></p>
			<!-- 资讯内容 开始 -->
			<div class="app-list app-details app-con">
				<div class="box con-head">
					<span class="c-tit"><?php echo $con['info_title'];?></span>
				</div>
				<p class="line-t-10"></p>
				<div class="app-infos">
					<?php echo $con['info_body'];?>
				</div>
			</div>
			<!-- 资讯内容 结束 -->
		</div>
		<!-- 底部内容 开始 -->
			<?php require_once ('inc_foot.php');?>
		<!-- 底部内容 结束 -->
	</div>
</body>
</html>
