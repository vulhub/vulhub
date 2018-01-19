<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/autodown.js"></script>

<?php require_once (dirname(__FILE__).'/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 定时更新 </div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div style="background:#fef3e8;clear:both;border:1px solid orange;padding:5px;">本页功能现完全免费，但基于对服务器资源消耗较多，如需开通，请用户前往论坛申请。<a href="http://bbs.appcms.cc" target="_blank"><font color=red><b>点此申请</b></font></a>。 <a href="http://bbs.appcms.cc" target="_blank"><font color=red><b>查看申请说明</b></font></a></div>
        <!--定时更新-->
        <div style="line-height:180%;margin-top:10px;">
            <span><strong>定时更新：</strong></span>无人值守自动更新最新应用数据，服务器任务执行定时更新<br>
            <font color=#888>注：关闭浏览器或者切换到其他页面不影响本功能</font>
        </div>
        <div class="mt10" style="margin-top:10px;height:40px;">
            <span id="auto_start" <?php if(file_exists("../cache/flag_auto_down")) echo "style='display:none'";?>><a class="but2" href="javascript:void(0);" onClick="auto_start('<?php echo md5(AUTH_CODE);?>');">开始定时更新</a>&nbsp;&nbsp;&nbsp;&nbsp;</span>
            <span id="auto_stop" <?php if(!file_exists("../cache/flag_auto_down")) echo "style='display:none'";?>><a class="but2"  href="javascript:void(0);" onClick="auto_stop('<?php echo md5(AUTH_CODE);?>');">停止定时更新</a>&nbsp;&nbsp;&nbsp;&nbsp;</span>
            <span>如果有Linux服务器的控制权限，请使用contrab定时任务命令会更稳定，命令格式范例：
            */5 * * * * /PHP安装目录/php /APPCMS程序安装目录/后台管理目录/contrab.php&nbsp;&nbsp;&nbsp;&nbsp;</span>
        </div>

        <!--同步全部数据-->
        <div style="line-height:180%;">
            <span><strong>同步全部数据：</strong></span>一次性同步数据中心的全部应用数据，一般用于第一次安装后初始化数据<br>
            <font color=#888>注：关闭浏览器或者切换到其他页面即失效</font>
        </div>
        <div class="mt10" style="margin-top:10px;height:40px;">
            <span id="autodown"><a class="but2"  href="javascript:void(0);" onClick="downall('autodown_frame.php?m=downall&auth=<?php echo md5(AUTH_CODE);?>');" target="autodowniframe">开始同步数据</a></span>&nbsp;
            <span id="stopdown"><a class="but2" href="javascript:void(0);" onClick="window.location.href='autodown.php';" target="autodowniframe">停止同步数据</a></span>
			<span style="line-height:26px;">开始页码：</span><span><input id="start" class="ipt" value="" style="width:40px"></span> 
			<span style="line-height:26px;">结束页码：</span>
			<span><input id="end" class="ipt" value="" style="width:40px"></span>
        </div>
		
        <div>
            <iframe id="autodowniframe" name="autodowniframe" src="about:blank"  width="100%" height="230" scrolling=""  frameborder="0" style="overflow-x: hidden; overflow-y: auto; "></iframe>
        </div>
    </div>
    
<?php require_once(dirname(__FILE__)."/inc_footer.php");?>
</div>
</body>
</html>