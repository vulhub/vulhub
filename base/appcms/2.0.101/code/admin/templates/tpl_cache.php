<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/cache.js" ></script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 清理缓存</div>
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10">
            <a class="but2" href="javascript:void(0);" onclick="cache('category');">分类缓存</a>
            <a class="but2" href="javascript:void(0);" onclick="cache('url_rewrite');">伪静态缓存</a>
            <a class="but2" href="javascript:void(0);" onclick="cache('page');">页面数据缓存</a>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php"); ?>

</div><!-- 右侧区域结束 -->
</body>
</html>