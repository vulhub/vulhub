<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/app.js" ></script>
<script language="javascript" type="text/javascript">
<?php if(isset($page['get']['jsfun']) && $page['get']['jsfun']=='add'){
    echo 'window.onload=function(){show_edit(0);}';
}?>
$(window).ready(function(){
    <?php if(isset($_GET['cate_id'])) echo '$("#cate_id").val('.$_GET['cate_id'].');';?>

});

</script>
<?php require_once(dirname(__FILE__).'/inc_top.php');?>
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body">
    <!-- 当前位置开始 -->
    <div class="snav">当前位置：<a href="frame.php">管理首页</a>&nbsp;&nbsp;»&nbsp;&nbsp;应用列表</div><!-- 当前位置结束 -->

    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <?php
	        global $page;
	        if(function_exists("tpl__".$page['get']['m'])){
		    call_user_func("tpl__".$page['get']['m']);
	    }?>
        <?php function tpl__list(){ global $page,$c,$dbm,$time_start;?>
        <div class="mt10 clearfix">
            <div class="l">
                <a href="app.php?m=list" class="but2">全部应用</a>
                <a href="javascript:void(0);" class="but2" onClick="C.form.update_field('app.php?m=order','.order','app_id');">修改排序</a>
                <a href="javascript:void(0);" class="but2" onClick="if (confirm('确认删除选择数据吗？')){C.form.batch_modify('app.php?m=del','.cklist');}">删除选中</a>
                <a href="collect.php" class="but2">添加应用</a>
				<a href="javascript:void(0);" onClick="show_edit(0);" class="but2">添加本地应用</a>
                <a href="javascript:void(0);" class="but2" onclick="recommend('0');">加入推荐位</a>
                <a href="javascript:void(0);" class="but2" onclick="recommend('3');">加入专题</a>
            </div>
            <div class="r">
            <div class="select-box l">
                    <div class="select-wrap">
                        <select id="cate_id" class="search_type" style=" text-align:left">
                            <option value="0" >分类选择</option>
                            <?php $c->html_cate_select($page['tree'],0,'','');?>
                        </select>
                        </div>
                        </div>
                <div class="select-box l">
                    <div class="select-wrap">
                        <select id="search_type" class="search_type">
                            <option value="app_title" <?php if(isset($_GET['search_type'])&&$_GET['search_type']=="app_title" ) echo "selected"?>>应用名称</option>
                            <option value="app_id" <?php if(isset($_GET['search_type'])&&$_GET['search_type']=="app_id" ) echo "selected"?>>应用id</option>
                        </select>
                    </div>
                </div>
                <div class="l">
                    <input type="text" id="search_txt" class="ipt seachIpt" value="<?php if(isset($_GET['search_txt'])) echo $_GET['search_txt']; ?>" onkeydown="if(event.keyCode==13) search_submit('app.php?m=list');">&nbsp;
                    <a href="javascript:void(0);" class="but2 mr0" onclick="search_submit('app.php?m=list');" >查 询</a>
                </div>
            </div>
        </div>

        <div class="mt10">
           <table class="tb">
                <tr>
                    <th width="80"><a href="javascript:void(0);" onclick="C.form.check_all('.cklist');">全选/反选</a></th>
                    <th width="50">排序</th>
                    <th width="50">ID</th>
                    <th class="alignleft">应用名称</th>
                    <th>所属分类</th>
                    <th width="70">图片</th>
                    <th width="80" class="alignleft">版本</th>
                    <th width="80" class="alignleft">类型</th>
                    <th width="80" class="alignleft">访问量</th>
                    <th width="80" class="alignleft">下载量</th>
                    <th width="60" class="alignleft">大小</th>
                    <th width="50" class="alignleft">评分</th>
                    <th width="80" class="alignleft">更新时间</th>
                    <th width="200"></th>
                </tr>
                <?php
                    if (!empty($page['apps']['list'])){
                        foreach ($page['apps']['list'] as $kapp => $vapp){//print_r($vapp['history']);
                ?>
                <tr>
                    <td><input type="checkbox" value="<?php echo $vapp['app_id'] ?>" class="cklist" /></td>
                    <td><input type="text" size="2" class="order" app_id="<?php echo $vapp['app_id']; ?>" name="listorders[<?php echo $vapp['app_order']?>]" value="<?php echo $vapp['app_order']?>" /></td>
                    <td><?php echo $vapp['app_id'];?></td>
                    <td class="alignleft">
                    <?php if($vapp['app_logo']!='') echo('<img src="'.$vapp['app_logo'].'" width="30" height="30">&nbsp;&nbsp;'); ?><a href="<?php echo $vapp['surl'];?>" target="_blank"><?php echo $vapp['app_title'];?></a>
                    </td>

                    <td><?php $ai=0;foreach($c->cate_father($vapp['last_cate_id']) as $a){if($ai>0) echo(' - '); echo($a['cname']);$ai++;}; ?></td>
                    <td><?php echo(' <font color=blue>'.count($vapp['resource']).' 图</font>');?></td>
                    <td class="alignleft"><?php echo $vapp['app_version'];?></td>
                    <td class="alignleft"><?php echo $vapp['app_type'];?></td>
                    <td class="alignleft"><?php echo $vapp['app_visitors'];?></td>
                    <td class="alignleft"><?php echo $vapp['app_down'];?></td>
                    <td class="alignleft"><?php echo $vapp['app_size'];?></td>
                    <td class="alignleft"><?php echo $vapp['app_grade'];?></td>
                    <td class="alignleft"><?php if(strtotime(date('Y-m',time()))<=$vapp['app_update_time']){?> <font color="red"> <?php echo date('Y-m-d',$vapp['app_update_time']); ?></font><?php }else{ echo date('Y-m-d',$vapp['app_update_time']); }?></td>
                    <td>
						<a class="but2 but2s" href="javascript:void(0);" onclick="show_history(0,<?php echo $vapp['app_id'];?>)" >添加版本</a>
                        <a class="but2 but2s" href="javascript:void(0);" class="btn2" onClick="show_edit(<?php echo $vapp['app_id'] ?>);" >修改</a>
                        <a class="but2 but2s" href="javascript:void(0);" class="btn2" onClick="app_del(<?php echo $vapp['app_id'];?>)" >删除</a>

					</td>
                </tr>
                <?php } } ?>
            </table>
            <div class="pagebar clearfix"><?php echo $page['apps']['pagebar']['pagecode']; ?></div>
        </div>
    </div><!-- 右侧主体内容结束 -->

    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
    <div id="appedit" style="display:none" >
        <div style="margin:10px;">
            <form method="post" action="app.php?m=save_edit" id="appform">
            <div class="tab_table">
                 <div class="btn_box" style="height:28px;padding:0;" >
                    <a href="javascript:void(0);" class="current" id="tabbtn01">基本信息</a>
                    <a href="javascript:void(0);"  id="tabbtn02">历史版本</a>
                    <a href="javascript:void(0);"  id="tabbtn03">资源图片</a>
                </div>
                <div id="e_box" style="overflow-y:auto;overflow-x:hidden;height:410px;border:1px solid #ddd;zoom:1;position:relative;">
                    <table class="tb3" id="tab001">
                        <input type="hidden" size="50" class="ipt"  id="app_id"   name="app_id" value="0"/>
                        <tr>
                            <td width="100">应用名称：</td>
                            <td  class="alignleft"><input type="text" size="50" class="ipt"  id="app_title"   name="app_title" value=""/>
                            <span>* 不能为空</span>
                            </td>
                            <td  width="100">SEO名称：</td>
                            <td  class="alignleft"><input type="text" size="50" class="ipt"  id="app_stitle"   name="app_stitle" value=""/>
                            </td>
                        </tr>
                        <tr>
                            <td>栏目分类：</td>
                            <td  class="alignleft">
                                <select id="last_cate_id" name="last_cate_id"> <?php
                                    $c->html_cate_select($c->cate_tree('0'));?>
                                </select> &nbsp;&nbsp;* 必填
                            </td>
                            <td  width="100">标签：</td>
                            <td  class="alignleft"><input type="text" class="ipt" id="app_tags" name="app_tags" onblur="this.value=this.value.replace(/，/g,',')"  value=""/>&nbsp;<a href=" javascript:void(0);" class="but2" id="subtn"  onclick="get_tags($('#app_title').val());" >提取</a><span> * 多个标签用,分开</span></td>
                        </tr>
                        <tr>
                            <td>缩略图：</td>
                            <td  class="alignleft">
                            <div class="l">
                                <input type="text" class="ipt"  name="app_logo" id="app_logo" value="" style="float:left; margin-right:10px;"/><span id="showimg"></span>
                            </div>
                            <div class="l" >
                               <script>C.create_upload_iframe('{"func":"callback_upload_thumb","img":"#showimg","thumb":{"width":"300","height":"300"}}');</script>
                            </div>
                            </td>
                            <td>评分：</td>
                            <td  class="alignleft"><input type="text" class="ipt" id="app_grade" name="app_grade"  value="50"/></td>
                        </tr>
                        <tr>
                            <td>浏览量：</td>
                            <td  class="alignleft"><input type="text" class="ipt" id="app_visitors" name="app_visitors"  value=""/></td>
                            <td>评论量：</td>
                            <td  class="alignleft"><input type="text" class="ipt" id="app_comments" name="app_comments"  value=""/></td>

                        </tr>
                        <tr>
                            <td>推荐星级：</td>
                            <td  class="alignleft" colspan="3"><input type="text" class="ipt" id="app_recomment" name="app_recomment"  value="6"/>
                            &nbsp;&nbsp;* 一般的推荐星级显示，是【<span style="color:red"> 1.0 至 10.0 </span>】（保留一位小数），请正确填写</td>
                        </tr>
                        <tr>
                            <td>计费包ID：</td>
                            <td colspan="3" class="alignleft"><input type="text" class="ipt" id="charge_app_id" name="charge_app_id"  value=""/>
                                &nbsp;&nbsp;* 如果申请了联盟的独立计费包，在此填写计费包ID，没有则留空，<a href="<?php echo(UNION_URL);?>" target="_blank">点此申请</a>
                            </td>
                        </tr>
                        <tr>
                            <td>伪静态标签：</td>
                            <td colspan="3" class="alignleft"><input type="text" class="ipt" id="rewrite_tag" name="rewrite_tag"  value=""/>
                                &nbsp;&nbsp;输入weixin前台伪静态页面就变成 http://www.****.com/app/weixin.htm
                                </a>
                            </td>
                        </tr>
                        <tr>
                            <td>应用详情：</td>
                            <td colspan="3">
                            <div style="position:absolute;">
                            <textarea name="app_desc" id="app_desc" style="display:none"></textarea>
                                <script type="text/javascript" src="<?php echo SITE_PATH ?>templates/lib/ckeditor/ckeditor.js?t=B49E5BQ"></script>
                                <script type="text/javascript">
                                    var editor1 = CKEDITOR.replace( "app_desc",{height:300,width:740,skin:"v2",menu_subMenuDelay:0,
                                 toolbar : ckeditor_toolbar
                                   });
                                </script>
								<span class="upbtn_box" id="upbtn_box"><script>C.ckeditor.init("#upbtn_box","editor1");</script></span>
                            </div>
                            </td>
                        </tr>
                    </table>
                    <table class="tb4" id="tab002">
						<tr>
							<th width="100">版本号</th>
							<th width="100">程序大小</th>
							<th width="100">APK包名</th>
							<th width="100">下载地址</th>
							<th width="100">系统要求</th>
							<th width="100">操作</th>
						</tr>
						<tbody id="history_list">

						<!--
						<tr>
							<td  class="alignleft" width="50"><input type="text" size="50" class="ipt hiswidth"   id="app_version"   name="app_version" value=""/>
							</td>
							<td  class="alignleft" width="50"><input type="text" size="50" class="ipt hiswidth"  id="app_size"   name="app_size" value=""/>
							</td>
							<td  class="alignleft" width="50">
								<input type="text" class="ipt hiswidth"  name="app_source_url" id="app_source_url" value=""/>
							</td>
							<td  class="alignleft" width="50"><input type="text" class="ipt hiswidth"  name="app_apk" id="app_apk" value=""/></td>
							<td  class="alignleft" width="50"><input type="text" size="50" class="ipt hiswidth"  id="app_system"   name="app_system" value=""/>
							</td>
							<td style="text-align:right;"><a href="javascript:void(0)" class="but2" style="margin-right:0;">确认</a></td>
						</tr>-->

						</tbody>
					</table>
                    <table class="tb4" id="tab003">
                        <tr>
                            <td style="padding:0;">
                                <table style="width:100%;">
                                    <tbody id="reso_list"></tbody>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <div style="line-height:30px;"><a href="javascript:;" class="add01" onclick="create_upload_resource('add');"></a></div>
                                <div id="mode_up_box"></div>
                            </td>
                        </tr>
                    </table>
                </div>
                <div style="margin:10px 0 0 110px;" >
                  <a href=" javascript:void(0);" class="but2" id="subtn"  onclick="app_edit()" >确 定</a>
                </div>
            </div>
            </form>
        </div>
    </div>
    <div id="history" style="display:none">
        <form id="historyform">
        <table class="tb2">
            <input type="hidden" size="50" class="ipt"  id="his_app_id"   name="app_id" value="0"/>
            <input type="hidden" size="50" class="ipt"  id="history_id_his"   name="history_id" value="0"/>
            <tr>
                <td  width="100" style="text-indent:10px;">上传APK：</td>
                <td  class="alignleft">
                <script>C.create_upload_iframe('{"func":"callback_upload_apk","img":"#showimg","thumb":{"width":"300","height":"300"}}');</script>
                 * 上传APK需要开启PHP的ZIP模块
				</td>
            </tr>
            <tr>
                <td  width="100" style="text-indent:10px;">版本号：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="app_version_his"   name="app_version_his" value=""/> * 必填
                </td>
            </tr>
            <tr>
                <td  width="100" style="text-indent:10px;">软件大小：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="app_size_his"   name="app_size_his" value=""/>  * 必填
                </td>
            </tr>
            <tr>
                <td  width="100" style="text-indent:10px;">系统要求：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="app_system_his"   name="app_system_his" value=""/> * 如Android2.0以上
                </td>
            </tr>

            <tr>
                <td  width="100" style="text-indent:10px;">下载地址：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="down_url_his"   name="down_url_his" value=""/> * 必须以http://开头
                </td>
            </tr>
            <tr>
                <td  width="100" style="text-indent:10px;">APP包名：</td>
                <td  class="alignleft">
                <input type="text" class="ipt"  id="packname_his"   name="packname_his" value=""/>
                </td>
            </tr>

            <tr>
                <td width="100"></td>
                <td  class="alignleft"><a href="javascript:void(0);" class="but2" onClick="app_history_edit();" id="subtn">确 定</a></td>
            </tr>
        </table>
        </form>
    </div>

    <!--推荐位隐藏表单 -->
        <div id="recommendhtml" style="display:none">
            <table class="tb3" >
            <input type="hidden" value="" id="app_ids" name="app_ids" />
                <tr>
                    <td>选择推荐位</td>
                    <td>
                    <div id="recommendsel">
                    </div>
                  </td>
                </tr>
                <tr>
                    <td width="100"></td>
                    <td  class="alignleft"><a href="javascript:void(0);" class="but2" onClick="save_recommend();" id="subtn">确 定</a></td>
                </tr>
            </table>
        </div>
</body>
</html>
<?php } ?>