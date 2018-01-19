<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/advert.js" ></script>
<script>
    <?php if(isset($page['get']['jsfun']) && $page['get']['jsfun']=='add'){
        echo 'window.onload=function(){recommend_show(0,'.$page['get']['type'].');}';
    }?>
</script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> »广告列表 
   </div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10 clearfix"> 
            <div class="l">
                    <a href="advert.php" class="but2">查看全部</a>
                    <a href="javascript:void(0);" class="but2" onclick="C.form.batch_modify('advert.php?m=del_adpace','.cklist');">删除选中</a>
                    <a href="javascript:void(0);" class="but2"  onclick="show_ad_pace();" id="type_2">添加广告位</a>
            </div> 
            <div class="r">
                <div class="select-box l">
                    <div class="select-wrap">
                        <select id="search_type" class="search_type">
                            <option value="title">标题</option>
                            <option value="area_remarks">备注</option></select>
                    </div>
                </div>
                <div class="l">
                    <input type="txt" id="search_txt" class="ipt seachIpt" onkeydown="if(event.keyCode==13) search_submit('advert.php?m=list');" />&nbsp;
                    <a href="javascript:void(0);" class="but2 mr0" onclick="search_submit('advert.php?m=list');" >查 询</a>
                </div>
            </div>
        </div>
        <div>
            <form action="" name="form_order" method="post">
            <table class="tb mt10">
                <tr>
                    <th width="80"><a href="javascript:void(0);" onclick="C.form.check_all('.cklist');">全选/反选</a></th>
                    <th width="50">ID</th>
                    <th align='center' width="150">标题</th> <!-- class="alignleft" -->
                    <!-- <th>广告</th> -->
                    <th align='center' >备注</th> 
                    <th width="300">操作</th>
                </tr>
                <?php 
                    if(!empty($page['adverts'])){ 
                        foreach ($page['adverts'] as $v){ 
                ?>
                <tr>   
                    <td><input type="checkbox" class="cklist" value="<?php echo $v['area_id']; ?>" /></td>
                    <td><?php echo $v['area_id']; ?></td>
                    <td><?php echo $v['title']; ?></td>
                    <!-- <td><?php //echo $v['area_html'];?></td> -->
                    <td><?php echo $v['area_remarks']; ?></td> 
                    
                    <td >
                        <a class="but2 but2s" href="javascript:void(0);" onClick="del_adpace(<?php echo $v['area_id'] ?>);">删除</a>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="show_ad(<?php echo $v['area_id']; ?>,<?php echo $v['type']; ?>);">管理广告</a>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="edit_adpace(<?php echo $v['area_id'];?>);">修改</a>   
                    </td>
                </tr>
                <?php  
                        } 
                    } 
                ?>
            </table>
            </form>

            <div class="pagebar clearfix">
  	            <?php if(!empty($page['pagecode'])) echo($page['pagecode']); ?>  
            </div>
        </div> 

    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
    <!-- 添加广告位隐藏表单 -->
    <div id="advert_html" style="display:none">
        <form method="post" id="areaform" >
        <table class="tb3" id="tab1" >
            <input type="hidden"  class="ipt"  id="area_id"   name="area_id" value="0"/>   
            <tr>
                <td  width="100">标题：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="title" style="width:200px;" name="title" value=""/> 
                </td>      
            </tr>
             <tr>
                <td  width="100">广告类型：</td>
                <td  class="alignleft">
                <select  id="type"><option value="1">代码广告</option><option value="2">轮播广告</option></select>
                </td>      
            </tr>
            <tr>
                <td  width="100">备注：</td>
                <td  class="alignleft">
                <textarea style="width:250px;height:100px;font-size:12px;line-height:14px;" id="area_remarks"   name="area_remarks"></textarea>
                </td>      
            
            <tr>
                <td></td>
                <td  class="alignright" style="text-align:left;"><a href="javascript:void(0);" class="but2" id="subtn" onclick="save_adpace();" >确 定</a></td><td></td>
            </tr>
        </table>
        </form>
    </div>
    
    <!-- 管理广告div-->
    <div style="display:none;overflow-y:auto;overflow-x:hidden;height:440px;border:1px solid #ddd;" id="fouce_advarts" >
		<input class="ad_pace" type='hidden' value="" />
		<input class="ad_type" type='hidden' value="" />
		<div style="margin:10px; height:215px; overflow-x:hidden; overflow-y:scroll; border:1px solid #ccc; border-width:1px 0 1px 1px;">
			<table class="tb3" style="width:800px; margin:5px!important;">
			<tr><th>标题</th><th>简介</th><th>链接</th><th>图片</th><th align="right" style="text-indent:0;"> <a style="float:right; margin-right:0; font-weight:normal;" href="javascript:void(0);" class="but2" id="subtn" onclick="add_ad_html();" >添加</a></th></tr>
			<tbody id='contents'>
			</tbody>
			</table>
		</div>
		<div style="margin-left:10px;">
			<a href="javascript:void(0);" class="but2"  id="subtn" onclick="save_ads();" >保存</a>
		</div>
    </div>
    <!-- 文字广告 div-->
    <div style="display:none" id="font_adverts">
     <input class="ad_pace" type='hidden' value="" />
     <input class="ad_type" type='hidden' value="" />
    <table class="tb3">
        <tr>
    <td><textarea id="area_html" class="ipt" style="width:500px;height:200px;font-size:12px;line-height:14px;" ></textarea></td></tr>    
   <tr><td colspan="2" align="center"><a href="javascript:void(0);" class="but2" id="subtn" onclick="save_ads();" >保存</a></td><tr>
    </table>
    </div>
</body>
</html>