<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/user.js" ></script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 后台账号</div>
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10">
            <a href="admin_user.php" class="but2">全部账号</a>
            <a class="but2" href="javascript:void(0);" onclick="C.form.batch_modify('admin_user.php?m=set_ustate&ustate=1','.cklist');">停用账号</a>
            <a class="but2" href="javascript:void(0);" onclick="C.form.batch_modify('admin_user.php?m=set_ustate&ustate=0','.cklist');">启用账号</a>
            <a href="javascript:void(0);" onclick="add_user_show(0);" class="but2">添加账号</a>
        </div>
        <table class="tb mt10">
            <tr>
                <th width="80"><a href="javascript:void(0);" onclick="C.form.check_all('.cklist');">全选/反选</a></th> 
                <th>账号ID</th>
                <th>账号</th>
                <th>用户状态</th>
                <th>开通时间</th>
                <th width="60" class="alignleft"></th>
            </tr>
            <?php 
                if (count($page['admins']['list']>0)){ 
                    foreach ($page['admins']['list'] as $ka => $va){
                        $action = $va['ustate'] ==0 ? "停用" : "启用";
            ?>
            <tr>
                <td><input type="checkbox" value="<?php echo $va['uid']; ?>" class="cklist"></td> 
                <td><?php echo $va['uid']; ?></td>
                <td><?php echo $va['uname']; ?></td>
                <td><font color=green><?php echo vars::get_field_str('ustate', $va['ustate'],'html'); ?></font></td>
                <td><?php echo date("Y-m-d H:m:s",$va['reg_date']); ?></td>
                <td>
                    <a class="but2 but2s" href="javascript:void(0);" onClick="edit_ustate(<?php echo $va['uid']; ?>,<?php echo $va['ustate']; ?>);"><?php echo $action; ?></a>
                </td>
            </tr>
            <?php 
                    } 
                }
            ?>    
        </table>
        <!--分页码-->
        <div class="pagebar clearfix">
            <?php echo $page['admins']['pagebar']['pagecode']; ?>
        </div>

    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php"); ?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
<div id="html_user" style="display:none;">
    <table class="tb3">
        <input type="hidden" id="action" value="">
        <input type="hidden" id="uid" value="0" />
        <tr>
            <td style="width:70px;">账　　号：</td>
            <td align="left"><input type="text" id="uname" class="ipt" value="<?php echo $_SESSION['uname']?>" style="width:200px;"></td>
        </tr>
        <tr>
            <td>密　　码：</td>
            <td><input type="password" id="upass" class="ipt" value="" style="width:200px;"></td>
        </tr>
        <tr>
            <td>确认密码：</td>
            <td><input type="password" id="re_pass" class="ipt" value="" style="width:200px;"></td>
        </tr>
        <tr>
            <td>&nbsp;</td>
            <td colspan="2" style=""><a href="javascript:void(0);" id="btn_update_user" onclick="user_edit();" class="but2 ml5">确 定</a></td>
        </tr>
    </table>
</div>

</body>
</html>