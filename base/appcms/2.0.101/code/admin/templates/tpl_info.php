<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/info.js" ></script>
<script language="javascript" type="text/javascript">
$(window).ready(function(){
    <?php if(isset($_GET['cate_id'])) echo '$("#cate_id").val('.$_GET['cate_id'].');';?>

});

</script>
<?php require_once (dirname(__FILE__).'/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 资讯列表  </div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10" style="height:28px;"> 
            <div class="l">
                <a class="but2" href="javascript:void(0);"  onClick=" window.location.href='info.php' ;" />查看全部</a>
                <a href="javascript:void(0);" class="but2"  onclick="C.form.update_field('info.php?m=save_corder','.infoorder','pid');" >修改排序</a>
                <a class="but2" href="javascript:void(0);" onClick="del_check();" >删除选中</a>
                <a class="but2" href="javascript:void(0);"  onClick="edit('','add');" />添加资讯</a>
                <a href="javascript:void(0);" class="but2" onclick="recommend('1');">加入推荐位</a>
                <a href="javascript:void(0);" class="but2" onclick="recommend('4');">加入专题</a>
            </div> 
            <div class="r">
                <div class="select-box l" style="_margin-top:2px;">
                    <div class="select-wrap">
                        <select id="cate_id" class="search_type" style=" text-align:left">
                        <option value="0" >分类选择</option>
                        <?php $c->html_cate_select($page['tree'],0,'','');?>
                        </select>
                    </div>
                </div>
                <div class="select-box l" style="_margin-top:2px;">
                    <div class="select-wrap" >
                        <select id="search_type" class="search_type">
                            <option value="keys" <?php echo isset($page['get']['search_type'])&&$page['get']['search_type']=='keys'?'selected':''; ?>>标题</option>
                            <option value="info_id" <?php echo isset($page['get']['search_type'])&&$page['get']['search_type']=='info_id'?'selected':''; ?>>资讯ID</option>
                        </select>
                    </div>
                </div>
                <div class="l">
                    <input type="text" id="search_txt" style="vertical-align:middle;"  class="ipt seachIpt" value="<?php echo isset($page['get']['search_txt'])?$page['get']['search_txt']:''; ?>" onKeyUp="if(event.keyCode==13){window.location='?&cate_id='+$('#cate_id').val()+'&search_txt='+$('#search_txt').val()+'&search_type='+$('#search_type').val();}"  >
                    &nbsp;<a href="javascript:void(0);" class="but2 mr0" onClick="window.location='?&cate_id='+$('#cate_id').val()+'&search_txt='+$('#search_txt').val()+'&search_type='+$('#search_type').val();" style="vertical-align:middle;">查询</a>
                </div>
            </div>
        </div>
        <div class="mt10">
            <table class="tb" >
                <tr>
                    <th width="80"><a href="javascript:void(0);" onClick="C.form.check_all('.infoff');">全选/反选</a></th>
                    <th align='center' width="50">排序</th>
                    <th align='center' width="70">ID</th>
                    <th class="alignleft">标题</th>
                    <th>所属分类</th>
                    <th width="40">图片</th>
                    <th width="60">浏览</th>
                    <th width="40">评论</th>
                    <th>编辑</th>
                    <th width="80">发布时间</th>
                    <th width="120"></th>
                </tr>
                
               <?php 
               if(!empty($page['infos']['list'])){
               foreach($page['infos']['list'] as $r){
               ?>
                <tr>   
                    <td><input type="checkbox" class="cklist infoff" value="<?php echo $r['info_id']; ?>" /></td>
                    <td><input type="text" size="2" value="<?php echo $r['info_order']; ?>" class="infoorder" pid="<?php echo $r['info_id']; ?>" /></td>
                    <td><?php echo $r['info_id']; ?></td>
                    <td class="alignleft"><?php if($r['info_img']!='') echo('<img src="'.$r['thumb'].'" height="30"> ');?><a target="_blank" href="<?php echo $r['surl'];?>" > <?php echo $r['info_title']; ?></a></td>
                    <td><?php $ai=0;foreach($c->cate_father($r['last_cate_id']) as $a){if($ai>0) echo(' - '); echo($a['cname']);$ai++;}; ?></td>
                    <td><?php echo $r['resource_total']; ?></td>
                    <td><?php echo $r['info_visitors']; ?></td>
                    <td><?php echo $r['info_comments']; ?></td>
                    <td><?php echo $r['uname']; ?></td>
                    <td title="<?php echo date('Y-m-d H:i:s',$r['create_time']); ?>" ><?php if(strtotime(date('Y-m',time()))<=$r['create_time']){?> <font color="red"> <?php echo date('Y-m-d',$r['create_time']); ?></font><?php }else{ echo date('Y-m-d',$r['create_time']); }?></td>
                    <td><a class="but2 but2s" href="javascript:void(0)"  onclick="edit(<?php echo $r['info_id'];?>,'edit')">编辑</a>
                    <a class="but2 but2s" href="javascript:void(0);"  onClick=" del(<?php echo $r['info_id']; ?>);" >删除</a> </td>
                </tr>
               <?php  } } ?> 
            </table>
            <div class="pagebar clearfix"><?php echo $page['infos']['pagecode']; ?></div>
        </div>
    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
<div style="display:none;" id="addinfo">
    <div style="margin:10px;">
    
        <div class="tab_table">
           <div class="btn_box" style="height:28px;padding:0;" >
                <a href="javascript:void(0);" class="current" id="bn1" >基本信息</a>
                <a href="javascript:void(0);" id="bn2">资源图片</a>
           </div>
        </div>
        <div id="e_box" style="overflow-y:auto;overflow-x:hidden;height:440px;border:1px solid #ddd;position:relative;">
            <form id="info_form"><input type="hidden" id="info_id" name="info_id" class='ipt' value="" />
            <table class="tb3" id="tab1">
                <tr>
                    <td  width="100">标题：</td>
                    <td  colspan="3" class="alignleft"  ><input type="text" class="ipt" id="info_title" name="info_title" value="" style="width:500px;" />&nbsp;&nbsp;* 必填，信息标题</td> 
                </tr>
                <tr>
                    <td  width="100">SEO标题：</td>
                    <td  colspan="3" class="alignleft"><input type="text" class="ipt" id="info_stitle" name="info_stitle"  value="" style="width:500px;"/>&nbsp;&nbsp;该标题不为空，详情页 title 会被替换</td>      
                </tr>
                <tr>
                    <td>栏目分类：</td>
                    <td  class="alignleft" >
                        <select id="last_cate_id" name="last_cate_id">
                            <option value="0"  >≡ 选择分类 ≡</option>
                            <?php $c->html_cate_select($page['tree'],0);?>
                        </select>&nbsp;&nbsp;* 必填 <a href="category.php">分类管理</a>
                    </td>       
                   
                </tr>
				<tr>
					 <td>标签：</td>
                    <td  class="alignleft"><input type="text" class="ipt" id="info_tags" name="info_tags"  value=""/>&nbsp;&nbsp;<a href=" javascript:void(0);" class="but2" id="subtn"  onclick="get_tags();" >提取标签</a>&nbsp;&nbsp;多个用逗号分开</td>
				</tr>
                <tr>
                <td>缩略图：</td>
                    <td  class="alignleft">
                        <div class="l"><input type="text" class="ipt" id="info_img" name="info_img" style="float:left; margin-right:10px;"  value="0"/><span id="showimg"></span></div>
                        <div class="l" >
                           <script>C.create_upload_iframe('{"func":"callback_upload_thumb","thumb":{"width":"300","height":"300"}}');</script>
                        </div>     
                    </td>
                       
                    <td>来源：</td>
                    <td  class="alignleft"><input type="text" class="ipt" id="info_from" name="info_from"  value=""/>&nbsp;&nbsp;如：新浪科技</td>      
                </tr>
                <tr>
                    <td>跳转地址：</td>
                    <td colspan="3" class="alignleft"><input type="text" class="ipt" id="info_url" name="info_url"  value="" style="width:500px;"/>&nbsp;&nbsp;填写该地址则直接跳转</td> 
                </tr>
                <tr>
                    <td>摘要：</td>
                    <td colspan="3" class="alignleft">
                        <textarea name="info_desc"  id="info_desc" class="ipt" style="width:700px; height:50px;"></textarea>
                    </td>      
                </tr>
                <tr>
                    <td>资讯详情：</td>
                    <td style="" colspan="3">
                        <div style="position:relative;">
                            <textarea name="info_body" id="info_body" style="display:block"></textarea>
                            <script type="text/javascript" src="<?php echo SITE_PATH;?>templates/lib/ckeditor/ckeditor.js?t=B49E5BQ"></script>
                            <script type="text/javascript">
                                var editor1 = CKEDITOR.replace( "info_body",{height:300,width:720,skin:"v2",menu_subMenuDelay:0,
                             toolbar : ckeditor_toolbar
                               });
                            </script>
                            <span class="upbtn_box" id="upbtn_box"><script>C.ckeditor.init("#upbtn_box","editor1");</script></span>
                        </div>
                    </td> 
                </tr>
            </table>
            </form>      
            <table class="tb4" id="tab2">
                <tr>
                    <td style="padding:0;">
                        <table style="width:100%;">
                            <tbody id="reso_list"></tbody>
                        </table>
                    </td>
                <tr>
                <tr>
                    <td>
                        <div style="line-height:30px;"><a href="javascript:;" class="add01" onclick="create_upload_resource('add');"></a></div> 
                        <div id="mode_up_box"></div>
                    </td>
                </tr>
            </table>
        </div>
        <div style="margin:10px 0 0 110px;" >
          <a href=" javascript:void(0);" class="but2" id="subtn"  onclick="save();" >确 定</a>
        </div>
    </div>
</div>
<!--推荐位隐藏表单 -->
<div id="recommendhtml" style="display:none">
    <table class="tb3" >
    <input type="hidden" value="" id="app_ids" name="app_ids" />
        <tr>
            <td>选择推荐位</td>
            <td><div id="recommendsel"></div>
          </td>
        </tr>
        <tr>
            <td width="100"></td>
            <td  class="alignleft"><a href="javascript:void(0);" class="but2" onClick="save_recommend();" id="subtn">确 定</a></td>
        </tr>
    </table>
</div>

<?php if(isset($_GET['jsfun']) && $_GET['jsfun']=='add'){?>
<script language="javascript" type="text/javascript" >
    $(document).ready(function(){
       edit('','add');
    });
</script>
<?php }?>
</body>
</html>