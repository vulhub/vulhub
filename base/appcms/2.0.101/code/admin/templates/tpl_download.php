<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/download.js" ></script>

<?php require_once (dirname(__FILE__).'/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 图片本地化 </div><!-- 当前位置结束 -->
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div style="line-height:180%;height:100px;">
            默认添加的应用，图片为数据中心的远程地址（可节省流量带宽）；如果需要下载到本地，在下面填写开始和结束的应用ID即可，点击“开始下载”按钮<br>
            <font color=red>注：下载图片可能耗时较长，下载过程中，请勿关闭窗口！</font>
        </div>
        <?php 
            $a=$dbm->scalar("select min(app_id) as ts,max(app_id) as te from ".TB_PREFIX."app_list");
        ?>
        <div class="mt10" style="height:40px;">
            <span class="l" style="line-height:26px;">开始ID：</span>
            <span class="l"><input class="ipt" style="width:80px" id="start" value="<?php echo($a['ts']);?>"/>&nbsp;&nbsp;&nbsp;&nbsp;</span>
            <span class="l" style="line-height:26px;">结束ID：</span></span>
            <span class="l"><input class="ipt" style="width:80px" id="end" value="<?php echo($a['te']);?>"/>&nbsp;&nbsp;&nbsp;&nbsp;</span>
            <span class="l"><a class="but2" href="javascript:void(0);"  onClick="set_src();" >开始下载</a>&nbsp;&nbsp;&nbsp;&nbsp;</span>
            <span class="l"><a class="but2" href="javascript:void(0);"  onClick="window.location.href='download.php';" >停止下载</a></span>
        </div>
        <div>
            <iframe id="downiframe" src="about:blank"  width="99%" height="350" scrolling=""  frameborder="0" style="overflow-x: hidden; overflow-y: auto; "></iframe>
        </div>
    </div>
 
<?php require_once(dirname(__FILE__)."/inc_footer.php");?>
</div>
</body>
</html>