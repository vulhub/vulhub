
<?php require_once(dirname(__FILE__)."/inc_header.php");?>
<script language="javascript" type="text/javascript" src="templates/css/js/comment.js" ></script>
<?php require_once(dirname(__FILE__)."/inc_top.php");?>
<!-- 加载左侧导航栏 -->

<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
   <div class="snav">您的位置：<a href="frame.php">管理首页</a> »</span> 评论管理</div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div>
            <div class="mt10 clearfix">
                <div class="l">
                <a class="but2" href="comment.php">查看全部</a>
                   <a class="but2" href="javascript:void(0);" onclick="C.form.batch_modify('comment.php?m=del','.ccomment');" >删除选中</a>
                   <a class="but2" href="javascript:void(0);" onclick="C.form.batch_modify('comment.php?m=edit&is_check=1','.ccomment');" />审核选中</a>
                   <a class="but2" href="javascript:void(0);"  onclick="C.form.batch_modify('comment.php?m=edit&is_check=0','.ccomment');" >屏蔽选中</a>
                </div> 
                <div class="r">
                    <div class="select-box l">
                        <div class="select-wrap">
                            <select id="search_type" class="search_type">
                                <option value="keys" <?php if(isset($page['get']['search_type'])&&$page['get']['search_type']=='keys'){echo 'selected';} ?>>关键字</option>
                            </select>
                        </div>
                    </div>
                    <div class="l">
                        <input type="text" id="search_txt" class="ipt seachIpt" value="<?php echo isset($page['get']['search_txt'])?$page['get']['search_txt']:''; ?>" onkeyup="if(event.keyCode==13){window.location='?&search_txt='+$('#search_txt').val()+'&search_type='+$('#search_type').val();}"  >&nbsp;
                        <a class="but2 mr0" href="javascript:void(0);" onclick="window.location='?&search_txt='+$('#search_txt').val()+'&search_type='+$('#search_type').val();" >查 询</a>
                    </div>
                </div>
            </div>
        </div>
        <div>
            <table class="tb">
                <form action="?m=save_order" name="form_order" method="post">
                <tr>
                    <th width="80"><a href="javascript:void(0);" onclick="C.form.check_all('.ccomment');">全选/反选</a></th>
                    <th width="60">ID</th>
					<th width="60">评论标题</th>
                    <th width="400"  class="alignleft">评论内容</th>
                    <th>用户</th>
                    <th>评论类型</th>
                    <th>评论时间</th>
                    <th>IP地址</th>
                    <th width="50">状态</th>
                    <th width="120"></th>
                </tr>
                <?php 
                    foreach($page['comments']['list'] as $r){
                ?>
                    <tr>   
                        <td><input type="checkbox" class="ccomment" value="<?php echo $r['comment_id']; ?>" /></td>
                        <td><?php echo $r['comment_id']; ?></td>
						<td><a href="<?php echo $r['url'];?>" target="_blank"><?php echo $r['comment_name']; ?></a></td>
                        <td class="alignleft"><div style="height:24px; line-height:24px; width:350px; overflow:hidden;"><?php echo $r['content']; ?></div></td>
                        <td><?php echo $r['uname']; ?></td>
                        <td><?php echo vars::get_field_str('catetype',$r['type']); ?></td>
                        <td><?php echo date('Y-m-d H:i:s',$r['date_add']); ?></td>
                        <td><?php echo $r['ip']; ?></td>
                        <td><?php echo vars::get_field_str('is_check',$r['is_check'],''); ?></td>
                        <td>
                            <?php if($r['is_check']==1){ ?>
                                <a class="but2 but2s" href="javascript:void(0);" onclick="edit(<?php echo $r['comment_id'];?>,'0');">屏蔽</a>
                            <?php }else{ ?>
                                <a class="but2 but2s" href="javascript:void(0);" onclick="edit(<?php echo $r['comment_id'];?>,'1');">通过</a> 
                            <?php } ?>
                            <a class="but2 but2s" href="javascript:void(0);" onclick="del(<?php echo $r['comment_id'];?>);">删除</a>
                        </td>
                    </tr>
                <?php 
                    } 
                ?> 
                </form>
            </table>
            <div class="pagebar"><?php echo $page['comments']['pagecode']; ?></div>
            <div class="clear"></div>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->

</body>
</html>















































