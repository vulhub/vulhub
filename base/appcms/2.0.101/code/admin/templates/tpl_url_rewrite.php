<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/url_rewrite.js" ></script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 伪静态规则</div><!-- 当前位置结束 -->
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <?php
            if (function_exists("tpl__" . $page['get']['m'])) {
                call_user_func("tpl__" . $page['get']['m']);
            } 
        ?>
    </div><!-- 右侧主体内容结束 -->

    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
<!-- 添加伪静态规则隐藏表单 -->
<div id="addurl" style="display:none;">
    <form action="#" id="addurlform">
    <table class="tb3">
        <input type="hidden" id="url_id" value="0">
    <tr>
        <td  width="120">规则节点名称：</td>
        <td  class="alignleft">
        <input type="text" class="ipt"  id="url_title"   name="url_title" value="" style="width:250px;"/> &nbsp;以英文字母开头，和数字下划线组合
        </td>      
    </tr>
    <tr>
        <td>动态地址规则：</td>
        <td  class="alignleft">
        <input type="text"  class="ipt"  id="url_true"   name="url_true" value="" style="width:250px;"/> &nbsp;默认显示URL
        </td>      
    </tr>
    <tr>
        <td>伪静态地址规则：</td>
        <td  class="alignleft">
        <input type="text"  class="ipt"  id="url_rule"   name="url_rule" value="" style="width:250px;"/> &nbsp;伪静态显示URL，参数需要跟动态地址对应
        </td>      
    </tr>
    <tr>
        <td>&nbsp;</td>
        <td class="alignleft"><a href="javascript:void(0);" class="but2 ml5" id="subtn" onclick="url_edit();" >确 定</a> </td>
    </tr>
    </table>
    </form>
</div>

</body>
</html>

<?php 
    //列表HTML
    function tpl__list(){
        global $page,$dbm,$time_start;
?>
    <div class="mt10 clearfix">
        <div class="l">
            <!--<a href="javascript:void(0);" class="but2"  onclick="url_show(0);">添加规则</a>-->
            1）所有伪静态规则请在默认规则上进行修改，花括号括起的部分如{id},{hid},{cid},{cpy}请不要修改；<br>
            2）如果是目录形式的地址，必须以 / 结束；<br>
            3）所有 app,info,special 这样的前缀可以改名，但必须要有，否则会引起冲突无法伪静态页面
        </div> 
    </div>
    <!-- 列表开始  -->
    <div>
        <table class="tb mt10">
            <tr>
                <th width="200" class="alignleft">&nbsp;&nbsp;&nbsp;&nbsp;规则节点名称</th>
                <th class="alignleft">伪静态地址</th>
                <th class="alignleft">动态地址</th>
                <th width="120" class="alignleft"></th>
            </tr>
            <?php if(isset($page['url_rewrites']['list'])&&!empty($page['url_rewrites']['list'])){
                foreach($page['url_rewrites']['list'] as $kurl=>$vurl){
            ?>
            <tr>
                <td class="alignleft">&nbsp;&nbsp;&nbsp;&nbsp;<?php echo($vurl['url_title']);  ?></td>
                <td class="alignleft"><?php echo($vurl['url_rule']);  ?></td>
                <td class="alignleft"><?php echo($vurl['url_true']);  ?></td>
                <td  class="alignright"> <a class="but2 but3s" href="javascript:void(0);" onClick="url_show(<?php echo($vurl['url_id']);?>);">修改</a></td>
            </tr>
            <?php } }?>
        </table>
    </div>
    <!-- 列表结束-->
<?php 
    }
?>