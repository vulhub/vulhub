 <?php require_once (dirname(__FILE__) . '/inc_header.php');?>
  <script type="text/javascript" src="templates/css/js/baidu.js"></script>
 <style>
#showcate a{
 width:auto;
 border:thick #6BA57C 1px;
 font-size:13px;
 margin-left:4px;
 display:inline-block;
 text-align:center;
 padding:2px 5px;
}

.cateselected {
  background:#25BDF5;
  color:#fff;
}
</style>

<?php require_once (dirname(__FILE__) . '/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> »</span> 百度链接提交</div><!-- 当前位置结束 -->
    
   <div style="margin:15px;">
   <form method="post" action="?m=submit" id="appform">
   填写本次提交的APP的ID，用（,）隔开:<br />
   <textarea type="text" rows="10" cols="100" style="font-size:12px;" id="app_ids" name="app_ids" ></textarea>
   <br />
   <a href=" javascript:void(0);" class="but2" id="subtn"  onclick="submit_app(2)" >确 定</a>
   </form>
   </div>
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
</body>
</html>