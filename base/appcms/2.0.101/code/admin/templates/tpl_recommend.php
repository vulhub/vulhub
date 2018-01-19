<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/recommend.js" ></script>
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
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 
    <?php if(!isset($page['get']['type']) ){$page['get']['type']=2;}
        if($page['get']['type']=="0"){echo "应用推荐位";}
        else if($page['get']['type']=="1"){echo "资讯推荐位";}
    ?></div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="mt10 clearfix"> 
            <div class="l">
                <?php if($page['get']['type']=="0"){ 
                    $type="type=0";
                }else if( $page['get']['type']=="1" ){ 
                    $type="type=1";
                }?>
                <?php if($page['get']['type']=="0"){ ?>
                    <a href="recommend.php?type=0" class="but2">查看全部</a>
                <?php } else if($page['get']['type']=="1"){ ?>
                    <a href="recommend.php?type=1" class="but2">查看全部</a>
                <?php }?>
                
                <a href="javascript:void(0);" class="but2" onclick="C.form.batch_modify('recommend.php?m=del','.cklist');">删除选中</a>
                <?php if( $page['get']['type']=="0"){ ?>
                    <a href="javascript:void(0);" class="but2"  onclick="recommend_show(0,0);" id="type_0">添加推荐位</a>
                <?php } else if( $page['get']['type']=="1" ){ ?> 
                    <a href="javascript:void(0);" class="but2"  onclick="recommend_show(0,1);" id="type_1">添加推荐位</a>
                <?php }?>
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
                    <input type="txt" id="search_txt" class="ipt seachIpt" onkeydown="if(event.keyCode==13) search_submit('recommend.php?<?php echo $type?>');" />&nbsp;
                    <a href="javascript:void(0);" class="but2 mr0" onclick="search_submit('recommend.php?<?php echo $type?>');" >查 询</a>
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
                    <th align='center' >备注</th> 
                    <th width="80">推荐总数</th> 
                    <th width="200"></th>
                </tr>
                <?php 
                    if(!empty($page['recommends']['list'])){ 
                        foreach ($page['recommends']['list'] as $kn => $vn){ 
                            //计算推荐的总数
                            if($page['get']['type']==2){
                                $count = "-";
                            }else{
                                $count = count(explode(",",$vn['id_list']));
                            }
                    
                            //如果是广告,应用和咨询，推荐内容ID和排序是没有的
                            if($vn['area_type']<=2){
                                //$vn['area_order']="-";$vn['id_list']="—";
                            }else{
                                $vn['area_order'] = '<input type="text" size=2 class="order" area_id="'.$vn['area_id'].'" value="'.$vn['area_order'].'" />';
                            }
                ?>
                <tr>   
                    <td><input type="checkbox" class="cklist" value="<?php echo $vn['area_id']; ?>" /></td>
                    <td><?php echo $vn['area_id']; ?></td>
                    <td><?php echo $vn['title']; ?></td>
                    <td><?php echo $vn['area_remarks']; ?></td> 
                    <td><?php echo $count; ?></td>
                    <td>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="recommend_show(<?php echo $vn['area_id'] ?>,<?php echo $vn['area_type']; ?>);">修改</a>
                        <a class="but2 but2s" href="javascript:void(0);" onClick="recommend_del(<?php echo $vn['area_id'] ?>);">删除</a>
                    </td>
                </tr>
                <?php  
                        } 
                    } 
                ?>
            </table>
            </form>

            <div class="pagebar clearfix">
  	            <?php if(!empty($page['recommends']['pagebar']['pagecode'])) echo($page['recommends']['pagebar']['pagecode']); ?>  
            </div>
        </div> 

    </div><!-- 右侧主体内容结束 -->
    
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
    <!-- 添加广告位隐藏表单 -->
    <div id="addrecommend_html" style="display:none">
        <div id="e_box" style="overflow-y:auto;overflow-x:hidden;height:410px;border:1px solid #ddd; border-width:1px 0 1px 1px;  margin:10px 10px 0;position:relative;">
        <form method="post" id="areaform" action="recommend.php?m=edit">
        <table class="tb3" width="100%" id="tab1" >
            <input type="hidden"  class="ipt"  id="area_id"   name="area_id" value="0"/> 
            <input type="hidden"  class="ipt"  id="area_type"   name="area_type" value="2"/>    
            <tr>
                <td  width="100">标题：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="title" name="title" style="width:400px;" value=""/> * 用于显示文字或者标记
                </td>      
            </tr>
            <tr>
                <td  width="100">排序：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="area_order" style="width:400px;" name="area_order" value="0"/> 
                </td>      
            </tr>
            <tr id="list_tr">
                <td  width="100">内容ID列表：</td>
                <td  class="alignleft">
                <textarea id="id_list" name="id_list" style="width:390px;height:60px;font-size:12px;line-height:14px;" onblur="this.value=this.value.replace(/，/g,',')" ></textarea> * 应用ID或者资讯ID，用,分隔</span>
                </td>      
            </tr>
            <tr>
                <td  width="100">详情描述：</td>
                <td style="">
                <div style="position:relative;">
                <textarea id="area_html" name="area_html" style="display:none"></textarea>
                    <script type="text/javascript" src="<?php echo SITE_PATH?>templates/lib/ckeditor/ckeditor.js"></script>
                    <script type="text/javascript">
                    var editor1 = CKEDITOR.replace( "area_html",{height:100,width:"96%",skin:"v2",menu_subMenuDelay:0,
                     toolbar : ckeditor_toolbar
                     });
                    </script>
                    <span class="upbtn_box" id="upbtn_box"><script>C.ckeditor.init("#upbtn_box","editor1");</script></span>
                </div>
                </td>
            </tr>
            <tr>
                <td  width="100">备注：</td>
                <td  class="alignleft">
                <textarea style="width:390px;height:50px;font-size:12px;line-height:14px;" id="area_remarks"   name="area_remarks"></textarea>
                没有可留空
                </td>      
            </tr>
        </table>
        </form>
        </div>
		<div style="margin:10px 0 0 120px;">
          <a href="javascript:void(0);" class="but2" id="subtn" onclick="recommend_edit();">确 定</a>
        </div>
    </div>
</body>
</html>