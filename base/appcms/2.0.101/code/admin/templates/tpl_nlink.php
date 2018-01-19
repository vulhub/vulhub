<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/nlink.js" ></script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">系统管理</a> » 正文内链</div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10 clearfix">
            <div class="l">
                <a href="nlink.php" class="but2" >查看全部</a>
                <a href="javascript:void(0);" class="but2" onclick="C.form.batch_modify('nlink.php?m=del','.cklist');">删除选中</a>
                <a href="javascript:void(0);" class="but2"  onclick="nlink_show(0);">添加正文内链</a>
            </div> 
            <div class="r">
                <div class="select-box l">
                    <div class="select-wrap">
                        <select id="search_type" class="search_type">
                            <option value="nlink_txt">内链关键词</option>
                            <option value="nlink_url">链接地址</option>
                        </select>
                    </div>
                </div>
                <div class="l">
                    <input type="txt" id="search_txt" class="ipt seachIpt"value="<?php echo isset($page['get']['search_txt'])?$page['get']['search_txt']:''; ?>" onkeydown="if(event.keyCode==13) search_submit('nlink.php?m=list');" />&nbsp;
                    <a href="javascript:void(0);" class="but2 mr0" onclick="search_submit('nlink.php?m=list');">查 询</a>
                </div>
            </div>
        </div>
        <!-- 列表开始 -->
        <div>
            <form action="" name="form_order" method="post">
            <table class="tb mt10">
                <tr>
                    <th width="80"><a href="javascript:void(0);" onclick="C.form.check_all('.cklist');">全选/反选</a></th>
                    <th align='center' width="200" >内链关键词</th> <!-- class="alignleft" -->
                    <th align='center'>链接地址</th>
                    <th width="120"></th>
                </tr>
                <?php 
                    if(!empty($page['nlinks']['list'])){ 
                        foreach ($page['nlinks']['list'] as $kn => $vn){ 
                ?>
                <tr>   
                    <td><input type="checkbox" class="cklist" value="<?php echo $vn['nlink_id']; ?>" /></td>
                    <td><?php echo $vn['nlink_txt']; ?></td>
                    <td><a href="<?php echo $vn['nlink_url']; ?>" target="_blank"><?php echo $vn['nlink_url']; ?><a></td>
                    <td>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="nlink_show(<?php echo $vn['nlink_id'] ?>);">修改</a>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="nlink_del(<?php echo $vn['nlink_id'] ?>);">删除</a>
                    </td>
                </tr>
                <?php  
                        } 
                    } 
                ?>
            </table>
            </form>

            <div class="pagebar clearfix">
  	            <?php print_r($page['nlinks']['pagebar']['pagecode']); ?>  
            </div>
        </div>
        <!-- 列表结束 -->
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
<!-- 添加内链词的隐藏表单 -->
<div id="addnlink" style="display:none;">
    <table class="tb3">
        <input type="hidden" id="nlink_id" value="0">
        <tr>
            <td  width="100">内链关键词：</td>
            <td  class="alignleft">
            <input type="text" class="ipt"  id="nlink_txt" style="width:180px;" name="nlink_txt" value=""/>
            </td>      
        </tr>
        <tr>
            <td  width="100">链接地址：</td>
            <td  class="alignleft">
            <input type="text"  class="ipt"  id="nlink_url"  style="width:180px;" name="nlink_url" value=""/> 
            </td>      
        </tr>
        <tr>
            <td></td>
            <td  class="alignleft"><a href="javascript:void(0);" class="but2 ml5" id="subtn" onclick="nlink_edit();" >确 认</a> </td>
        </tr>
    </table>
</div> 
</body>
</html>