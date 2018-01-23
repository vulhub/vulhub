<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="viewport" content="width=device-width,minimum-scale=1.0,maximum-scale=1.0,user-scalable=no">
<title><?php echo $con['app_title'].'免费下载|'.$con['app_title'].'手机版下载 - '.SITE_NAME;?></title>
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
			<div class="box box-tit"><div>
			<!-- 导航 开始 -->
			<?php
				$nav = "";
				foreach($c->cate_father($con['last_cate_id']) as $n){
                 $nav .= '<a href="'.$n['surl'].'">'.$n['cname'].'</a>&nbsp;»&nbsp;';
				}
				echo $nav;
			?>
			<?php echo $con['app_title'];?>
			<!-- 导航 结束 -->
			</div></div>
			<p class="line-t-10"></p>
			<div class="app-list app-details">
				<a class="app-img" onclick="window.open('<?php echo(SITE_PATH.'download.php?id='.$con['history'][0]['appcms_history_id']);?>');">
					<img src="<?php echo $con['app_logo'];?>" border="0">
				</a>
				<div class="app-info">
					<span class="app-name"><a href="javascript:;" onclick="window.open('<?php echo(SITE_PATH.'download.php?id='.$con['history'][0]['appcms_history_id']);?>');" title="<?php echo $con['app_title'];?>"><?php echo $con['app_title'];?></a></span>
					<span class="app-desc">系统要求：<?php echo $con['app_system'];?></span>
					<span class="b-v">大小：<?php echo $con['app_size'];?> &nbsp;&nbsp; 版本：<?php echo $con['app_version'];?></span>
					<a class="app-btn"  href="javascript:void(0);" onclick="window.open('<?php echo(SITE_PATH.'download.php?id='.$con['history'][0]['appcms_history_id']);?>');">下&nbsp;载</a>
				</div>
				<p class="line-t-10"></p>
				<!-- 历史版本 开始 -->
				<div class="app-his">
                    <ul class="date_history">
                        <li class="his_listg">
                            <ul class="his_son">
                                <li>版本号</li>
                                <li>大小</li>
                                <li>更新时间</li>
                                <li>立即下载</li>
                            </ul>
                        </li>
						<?php foreach($con['history'] as $v):?>
                        <li class="his_item">
                            <ul class="">
                                <li><a href="<?php echo $v['surl'];?>"><?php echo $v['app_version'];?></a></li>
                                <li><?php echo $v['app_size'];?></li>
                                <li><?php echo date('Y-m-d',$v['app_update_time']);?></li>
                                <li><a href="javascript:;" class="his-down" onclick="window.open('<?php echo(SITE_PATH.'download.php?id='.$v['appcms_history_id']);?>');">下载</a></li>
                            </ul>
                        </li>
						<?php endforeach;?>
                    </ul>
                </div>
				<!-- 历史版本 结束 -->
				<p class="line-t-10"></p>
				<div class="box box-tit"><div>应用介绍</div></div>
				<p class="line-t-10"></p>
				<div class="box app-infos">
					<div class="box info-tab" id="info-tab">
						<a href="javascript:;" id="item-1" class="t-sel" onclick ="tab_info('item-1')">介绍</a>
						<a href="javascript:;" id="item-2" onclick="tab_info('item-2')">截图</a>
					</div>
					<div class="box tab-1" id="tab-1">
						<?php echo $con['app_desc'];?>
					</div>
					<div class="box tab-2" id="tab-2"  style="display:none;">
					<?php foreach($con['resource'] as $v):?>
						<img src="<?php echo $v['resource_url'];?>" width="485" height="430" border="0">
					<?php endforeach;?>
					</div>
				</div>
			</div>
		</div>
		<!-- 底部内容 开始 -->
			<?php require_once ('inc_foot.php');?>
		<!-- 底部内容 结束 -->
	</div>
</body>
</html>
