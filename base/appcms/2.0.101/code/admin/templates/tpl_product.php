<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/product.js" ></script>
<script language="javascript" type="text/javascript">
var data_center_url='<?php echo(DATA_CENTER_URL); ?>';//数据中心
</script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 产品列表</div><!-- 当前位置结束 -->
         <input id="auths" type="hidden" value="<?php echo AUTH_CODE; ?>" />

    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div style="font-size:14px;line-height:200%;" id="product">
            根据厂商的要求，联盟产品只针对日IP超过2000的网站用户开放，请努力发展网站！如需开通，请联系客服QQ:98705900
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
</body>
</html>