 <?php require_once (dirname(__FILE__) . '/inc_header.php');?>
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
<script language="javascript" type="text/javascript">
var auth='<?php echo(AUTH_CODE); ?>';//授权码
var data_center_url='<?php echo(DATA_CENTER_URL); ?>';//数据中心
var local_cate=<?php echo(json_encode($c->categories));?>;//本地分类
</script>
<script language="javascript" type="text/javascript" src="templates/css/js/collect.js" ></script>
<?php require_once (dirname(__FILE__) . '/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> »</span> 添加应用</div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody" style="">
        <div class="clearfix">
             <div>
                <div class="l"> 
                    <!--<a href="javascript:void(0);" class="but2 l" onclick="zhineng('','');" >批量智能添加</a>
                    <a href="javascript:void(0);" class="but2 l" onclick="show_cate();">批量手动添加</a> -->
                    &nbsp;&nbsp;<span style="color:#CCCCCC;display:inline-block;line-height:30px;text-align:center"> 排序方式：</span>
                    <select id="order">
                        <option value="0">更新时间</option>
                        <option value="1">系统评分</option>
                        <option value="2">系统推荐</option>
                        <option value="3">下载次数</option>
                    </select>
                </div>
                <div class="r">
                    <input type="hidden" id='auth_code' value="<?php echo AUTH_CODE; ?>" />
                    <div class="l" style="color:#CCCCCC;display:inline-block;line-height:30px;text-align:center"> 关键词：</div>
                    <input style="width:150px;margin-right:5px;" class="ipt l" id='q' onKeyUp="if(event.keyCode==13){collect(1,'-1',$('#q').val());}" />
                    <a href="javascript:void(0);" class="but2 l" onclick="collect(1,'-1',$('#q').val());" >查 询</a>
                </div>
            </div>
            <div class="mt10" id="showcate" style="padding-top:10px;"></div>
            <div>
                <a href="javascript:void(0);" class="but2 l" onclick="blindcate();" style="margin-top:10px;" title="添加应用之前，需要先绑定本地分类">绑定分类</a>
                <a href="javascript:void(0);" class="but2 l" onclick="zhineng('','');" style="margin-top:10px;" >批量智能添加</a>
                <a href="javascript:void(0);" class="but2 l" onclick="show_cate();" style="margin-top:10px;">批量手动添加</a>
				<div id="pagecode" class="pagecode pagebar clearfix l"></div>
			</div>
            
        </div>
        
        <div>
            
            <table class="tb mt10" >
                <?php if(!AUTH_CODE) {?>
                <tr style="border:0px;background:none;"><td style="border:0px;padding-top:24px;background:none;">
                <span class="l"><a href="javascript:void(0);" class="but2" onclick="show_getauth();" >申请授权码</a>&nbsp;&nbsp;</span>
                <span class="l" style="color:red;">申请了授权码才可以添加应用数据</span>
                </td></tr>
                <?php }?>
                <tbody id="applist"></tbody>
   
            </table>

            <div>
                <a href="javascript:void(0);" class="but2 l" onclick="blindcate();" style="margin-top:10px;" title="添加应用之前，需要先绑定本地分类">绑定分类</a>
                <a href="javascript:void(0);" class="but2 l" onclick="zhineng('','');" style="margin-top:10px;" >批量智能添加</a>
                <a href="javascript:void(0);" class="but2 l" onclick="show_cate();" style="margin-top:10px;">批量手动添加</a>
				<div id="pagecode" class="pagecode pagebar clearfix l"></div>
			</div>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<div id="blindcate" style="display:none;margin:10px;*width:540px;">
    <form id="cates">
        <ul class="tb mt10" style="width:520px; margin-left:10px;  border:1px solid #ccc;margin-top:10px;background:#FAFAFA;color:#0965B8;">
            <div id="bind_html" style="height:300px; overflow-y:scroll;width:100%;">
            </div>
        </ul>
        <p class="clearfix" style="height:30px;display:block;margin-left:10px;">
            <span class="l" style="margin:5px 10px 0 0;">绑定分类后，你只需要勾选要添加的应用，添加的应用可以智能归类。</span><span class="l"><a href="javascript:void(0);" class="but2" onclick="saveblind();">绑定分类</a></span>
        </p>
    </form>

</div>
<div id="message" style="display:none; ">
    <div id="showmessage" style="overflow-y:scroll;line-height:200%;color:#888;width:500px;height:220px;margin:10px;">
    </div>
</div>
<div id="show_cate" style="display:none;" >
<br />
<a href="javascript:void(0);" class="but2" style="float:right; margin-top:-3px\9;" onclick="into_app($('#cateids').val(),$('#appid').val());">&nbsp;添加&nbsp;</a>
&nbsp;&nbsp;选择添加到的分类：<select id="cateids">
<?php 
    foreach($c->categories as $a){
        if($a['son']==0 && $a['type']==0){
            echo('<option value="'.$a['cate_id'].'">'.$a['cname'].'</option>');
        }
    }
?>
</select>
<input type="hidden" id="appid" value="" />

</div>
<div id="getauth" style="display:none;">
<table class="tb3">
    <tr><td colspan="3">授权码用于唯一标识每个网站，QQ号必须正确，否则取消授权</td></tr>
    <tr><td colspan="3">前台页面请保留我们的版权标识，否则取消授权</td></tr>
	<tr>
        <td>推荐人ID：</td>
        <td><input id='uid' value='' class='ipt' style="width:80px;" /> 如果没有推荐人请留空</td>
        <td></td>
    </tr>
    <tr>
        <td>QQ号码：</td>
        <td colspan="3"><input id='qq' value='' class='ipt' style="width:150px;"/></td>
       
    </tr>
	<tr>
		<td></td>
        <td><a href="javascript:void(0);" class="but2" onclick="getauth();" >我要申请</a></td>
		<td></td>
    </tr>
</table>
</div>
<!-- 隐藏表单半透明层 -->
</body>
</html>