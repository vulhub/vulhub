<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/template.js" ></script>
<script language="javascript" type="text/javascript">
<?php if(isset($page['get']['jsfun']) && $page['get']['jsfun']=='add'){
    echo 'window.onload=function(){show_temp();}';
}?>
</script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">系统管理</a> » 模板管理  <?php echo isset($page['get']['filename'])? "»".$page['get']['filename']:''; ?></div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <?php
            if (function_exists("tpl__" . $page['get']['m'])) {
            call_user_func("tpl__" . $page['get']['m']);
        } ?>

        <?php function tpl__list(){global $page;?>
        <div class="mt10 clearfix">
            <div class="l">
                 <a href="javascript:void(0);" class="but2" onClick="show_temp();">添加模板</a>
            </div>
            <div class="r" style="color:red;">模板文件名称定义：list_ 开头为分类列表模板；cover_ 开头为分类封面模板；content_开头为内容页模板</div>
        </div>
        <div class="">
            <table class="tb mt10" id="mbody" >
                <tr>
                    <th class="align_left" >文件列表</th>
                    <th class="align_left" >模板类型</th>
                    <th class="align_left" >文件大小</th>
                    <th>修改时间</th>
                    <th width="140"></th>
                </tr>
                <?php if(!empty($page['files'])){//print_r($page['files']);
                        foreach($page['files'] as $kf=>$vf){
                ?>
                <tr>
                    <td class="align_left" style="text-align:left;"><?php echo($vf['name']);  ?></td>
                    <td class="align_left" style="text-align:left;"><?php echo(vars::get_field_str('tpl_system',$vf['name'],''));?></td>
                    <td class="align_left" style="text-align:left;"><?php echo($vf['size'])?>&nbsp;bytes</td>
                    <td><?php echo(date("Y-m-d H:m:s",$vf['update_time']));?></td>
                    <td><a class="but2 but2s" href="template.php?m=show&filename=<?php echo $vf['name'];?>" style="float:right;">编辑</a></td>
                </tr>
            <?php } } ?>
            </table>
        </div>
        <?php } ?>

        <?php function tpl__show(){global $page;?>
            <div class="mt10 clearfix">
            <form action="template.php?m=save_edit" method="post" id="filesave" name="filesave">
                <input type="hidden" name="filename" value="<?php echo $page['get']['filename']; ?>" />
                <script language="javascript" type="text/javascript">  
                var twidth=parseInt($('.right_body').width())-170;
                C.form.textarea.create_line({"name":"content","width":twidth+"px","height":"420px","content":"<?php echo urlencode(htmlspecialchars($page['content'])); ?>"});
                </script>
                <br><a href="javascript:void(0);" style="margin-top:10px;" class="but2" id="subtn" onclick="$('#filesave').submit()" >确 定</a>
                <a href="javascript:void(0);" style="margin-top:10px;" class="but2" onclick="history.back();" >取 消</a>
            </form>
            </div>
        <?php }?>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
    <!--添加模板也的隐藏表单 -->
    <div id="addtemplate" style="display:none;">
        <table class="tb3">
            <tr>
                <td  width="80">文件名称：</td>
                <td  class="alignleft">
                    <input type="text" class="ipt" style="width:150px;" id="tempname"   name="tempname" value=""/>
                    <span>例：list_demo.php</span>
                </td>      
            </tr>
            <tr>
                <td></td>
                <td  class="alignleft"><a href="javascript:void(0);" class="but2 ml5" id="subtn" onclick="save_temp();" >确 定</a> </td>
            </tr>
        </table>
    </div>
</body>
</html>