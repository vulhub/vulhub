<?php require_once(dirname(__FILE__).'/inc_header.php');?>  
<script type="text/javascript">
 function setbug(appid,url,host){
    $.getJSON("http://data.appcms.cc/apperror.php?&jsoncallback=?",{"appid":appid,"url":url,"host":host},function(json){
    try{        
        alert(json.msg);
    }catch(e){alert(e.message);}
    });
 }
</script>
<script language="javascript" type="text/javascript" src="templates/css/js/frame.js" ></script>
<script language="javascript" type="text/javascript">
var data_center_url='<?php echo(DATA_CENTER_URL); ?>';//数据中心
</script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<script type="text/javascript">
$(function(){
       function panelW(){
        var right_width = $('.right_body').width();
        var panelWidth = right_width / 2- 17;
        var obj = $('.panel').children('.p-sty');
        $('.panel').children('.p-sty').width(panelWidth);
        obj.height() < 160 ? obj.height('160') : obj.height(obj.height());
    }
    $(window).ready(function(){ panelW();});
    $(window).resize(function(){panelW();});
});
</script>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 概况和升级</div><!-- 当前位置结束 -->
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
     <input id="auths" type="hidden" value="<?php echo AUTH_CODE; ?>" />
        <div class="panel">
            <div class="panel-advert">
                <span class="panel-tit">站长推荐</span>
                <ul id="adlist" class="clearfix"></ul>
            </div>
            <div class="p-sty  panel-one l" style="margin:0 5px 5px 0;">
                <span class="panel-tit">个人信息</span>
                <ul >
                    <li>您好，<?php echo $_SESSION['uname'];?></li>
                    <li>欢迎使用 <a href="http://www.appcms.cc/" target="_blank">AppCMS</a> 移动应用下载专用内容管理系统</li>
                    <li></li>
                    <li>本次登录IP：<?php echo helper::getip();?></li>
                </ul>
            </div>
            <div class="p-sty panel-two r" style="margin:0 0 5px 5px;">
                <span class="panel-tit">安全和升级</span>
                <ul id="safe_tips">
                </ul>
            </div>
            <div class="p-sty panel-three l" style="margin:5px 5px 10px 0;">
                <span class="panel-tit">开发团队</span>
                <ul id="dev_team">
                </ul>
            </div>
            <div class="p-sty panel-four  " style="margin:5px 0 10px 5px;">
                <span class="panel-tit">系统信息</span>
                <ul>
                    <li>AppCMS版本：<?php echo VERSION ;?></li>
                    <li>操作系统：<?php echo php_uname('s') ;?></li>
                    <li>服务器软件：<?php echo $apache = function_exists('apache_get_version')?apache_get_version():"";?></li>
                    <li>MySql版本：<?php echo $page['mysql_version'];?></li>
                 </ul>
            </div>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->

</body>
</html>