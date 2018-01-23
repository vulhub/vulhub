<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/keyword.js" ></script>


<?php require_once (dirname(__FILE__).'/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 关键字管理  </div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10" style="height:28px;"> 
            <div class="l">
                <a href="javascript:void(0);" class="but2"  onclick="C.form.update_field('keyword.php?m=save_qorder','.infoorder','pid');" >修改排序</a>
                <a class="but2" href="javascript:void(0);" onClick="C.form.batch_modify('keyword.php?m=del','.cklist');" >删除选中</a>
                <a class="but2" href="javascript:void(0);"  onClick="keyword_show(0);" />添加关键字</a>
            </div> 
            <div class="r">
                <div class="l">
                    <input type="text" id="search_txt" style="vertical-align:middle;"  class="ipt seachIpt" value="<?php echo isset($page['get']['q'])?$page['get']['q']:''; ?>" onKeyUp="if(event.keyCode==13){window.location='?&q='+$('#search_txt').val();}"  >
                    &nbsp;<a href="javascript:void(0);" class="but2 mr0" onClick="window.location='?&q='+$('#search_txt').val()" style="vertical-align:middle;">查询</a>
                </div>
            </div>
        </div>
        <div class="mt10">
            <table class="tb" >
                <tr>
                    <th width="80"><a href="javascript:void(0);" onClick="C.form.check_all('.infoff');">全选/反选</a></th>
					<th align='center' width="50"> 排序</th>
                    <th align='center' width="200" >ID</th>
                    <th align='center' width="200" >关键字</th>
                    <th>搜索次数</th>
                    <th width="100">编辑</th>
                    <th width="120"></th>
                </tr>
                
               <?php 
               if(!empty($page['keyword']['list'])){
               foreach($page['keyword']['list'] as $r){
               ?>
                <tr>   
                    <td><input type="checkbox" class="cklist infoff" value="<?php echo $r['id']; ?>" /></td>
                    <td><input type="text" size="2" value="<?php echo $r['qorder']; ?>" class="infoorder" pid="<?php echo $r['id']; ?>" /></td>
                    <td><?php echo $r['id']; ?></td>
                    <td><?php echo $r['q']; ?></td>
                    <td><?php echo $r['qnum']; ?></td>
                    <td align="center" width="100"><a class="but2 but2s" href="javascript:void(0)"  onclick="keyword_show(<?php echo $r['id'];?>)">编辑</a>
                    <a class="but2 but2s" href="javascript:void(0);"  onClick=" keyword_del(<?php echo $r['id']; ?>);" >删除</a> </td>
					<td width="120"></td>
				</tr>
               <?php  } } ?> 
            </table>
            <div class="pagebar clearfix"><?php echo $page['keyword']['pagecode']; ?></div>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->


<!-- 隐藏表单半透明层 -->
<!-- 添加关键字的隐藏表单 -->
<div id="addkeyword" style="display:none">
	
	<table class="tb3">
		<input type="hidden" name="keyword_id" id="keyword_id" value="0" />   
		<tr>
			<td  width="100">排序：</td>
			<td  class="alignleft"><input type="text" size="10" class="ipt"  id="q_order"   name="qorder" value="0"/> </td>    
		</tr>
		 <tr>
			<td  width="100">关键字名称：</td>
			<td  class="alignleft"><input type="text" class="ipt"  id="q_name"   name="q" value=""/></td>      
		</tr>
		<tr>
			<td  width="100">搜索次数：</td>
			 <td  class="alignleft"><input type="text"  class="ipt"  id="q_num"   name="qnum" value=""/></td>     
		
		<tr>
			<td>&nbsp;</td>
			<td  class="alignright" style="text-align:left;"><a href="javascript:void(0);" class="but2" id="subtn" onclick="keyword_edit();" >确 定</a></td>
		</tr>
	</table>
</div>

</body>
</html>