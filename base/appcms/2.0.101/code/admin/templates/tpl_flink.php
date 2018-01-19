<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/flink.js" ></script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 友情链接</div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div>
            <div class="mt10 clearfix">
                <div class="l">
                    <a href="flink.php" class="but2" >查看全部</a> 
                    <a href="javascript:void(0);" class="but2" onClick="C.form.update_field('flink.php?m=order','.order','flink_id');">修改排序</a>
                    <a href="javascript:void(0);" class="but2" onclick="C.form.batch_modify('flink.php?m=del','.cklist');" >删除选中</a>
                    <a href="javascript:void(0);" class="but2"  onclick="flink_show(0);">添加友情链接</a>
                </div> 
                <div class="r">
                    <div class="select-box l">
                        <div class="select-wrap">
                            <select id="search_type" class="search_type">
                                <option value="flink_name">链接名称</option>
                                <option value="flink_url">链接地址</option>
                            </select>
                        </div>
                    </div>
                    <div class="l">
                        <input type="txt" id="search_txt" class="ipt seachIpt" value="<?php echo isset($page['get']['search_txt'])?$page['get']['search_txt']:''; ?>" onkeydown="if(event.keyCode==13) search_submit('flink.php?m=list');" />&nbsp;
                        <a href="javascript:void(0);" class="but2 mr0" onclick="search_submit('flink.php?m=list');">查 询</a>
                    </div>
                </div>
            </div>
        </div>

        <div>
            <form action="flink.php?m=order" name="form_order" method="post">
            <table class="tb mt10">
                <tr>
                    <th width="80"><a href="javascript:void(0);" onclick="C.form.check_all('.cklist');">全选/反选</a></th>
                    <th align='center' width="50"> 排序</th>
                    <th align='center' width="200" >链接名称</th>
                    <th align='center' width="200" >链接地址</th>
                    <th>LOGO图片</th>
                    <th>链接位置</th>
                    <th width="120"></th>
                </tr>
                <?php  if (!empty($page['flinks']['list']) ) {
                    foreach ($page['flinks']['list'] as $kf => $vf){
                    $vf['flink_img'] = empty($vf['flink_img']) ? '-' : '<img style="height:30px;margin:2px 0 2px 0;" src="'.$vf['flink_img'].'" />';
                    $vf['flink_is_site']= vars::get_field_str('flink_is_site',$vf['flink_is_site']);
                ?>
                <tr>   
                    <td><input type="checkbox" class="cklist" value="<?php echo $vf['flink_id'];?>" /></td>
                    <td><input type="text" size="2" class="order" flink_id="<?php echo $vf['flink_id'];?>" value="<?php echo $vf['flink_order'];?>" /></td>
                    <td><?php echo $vf['flink_name'];?></td>
                    <td><a href="<?php echo $vf['flink_url']?>" target="_blank"><?php echo $vf['flink_url'];?></a></td>
                    <td><div><?php echo $vf['flink_img'];?></div> </td>
                    <td><?php echo $vf['flink_is_site']?></td>
                    <td>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="flink_show(<?php echo $vf['flink_id']?>);">修改</a>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="flink_del(<?php echo $vf['flink_id']?>);">删除</a>
                    </td>    
                </tr>
                <?php } }?>
            </table>
            </form>
            <div class="pagebar clearfix">
                <?php if(!empty($page['flinks']['pagebar'])) echo $page['flinks']['pagebar']['pagecode'];?>
            </div>
        </div> 
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
<!-- 添加友链的隐藏表单 -->
<div id="addflink" style="display:none;">
    <table class="tb3">
        <input type="hidden" id="flink_id" value="0">
        <tr>
            <td  width="80">排序：</td>
            <td  class="alignleft"><input type="text" size="10" class="ipt"  id="flink_order"   name="flink_order" value="0"/> </td>      
        </tr>
        <tr>
            <td>链接名称：</td>
            <td  class="alignleft"><input type="text" class="ipt"  id="flink_name"   name="flink_name" value=""/></td>      
        </tr>
        <tr>
            <td>链接地址：</td>
            <td  class="alignleft"><input type="text"  class="ipt"  id="flink_url"   name="flink_url" value=""/> <span> 以 http:// 开头</span></td>      
        </tr>
        <tr>
            <td>LOGO图片：</td>
            <td  class="alignleft">
                <div class="l"><input type="text" class="ipt" id="flink_img" name="flink_img" value=""/></div>
                <div class="l" style="margin-left:10px;margin-left:5px \9;vertical-align:bottom;">
                    <script>C.create_upload_iframe('{"func":"callback_upload","thumb":{"width":"300","height":"300"}}');</script>
                </div>
            </td>      
        </tr>
        <tr>
            <td>链接位置：</td>
            <td  class="alignleft"><?php echo vars::input_str(array('node'=>'flink_is_site','type'=>'radio','default'=>'0')); ?></td>      
        </tr>
        <tr>
            <td>&nbsp;</td>
            <td  class="alignleft"><a href="javascript:void(0);" class="but2 ml5" id="subtn" onclick="flink_edit();">确 定</a></td>
        </tr>
    </table>
</div>  
</body>
</html>