<?php require_once (dirname(__FILE__) . '/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/category.js" ></script>

<?php require_once (dirname(__FILE__) . '/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>

<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
   <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 分类列表</div><!-- 当前位置结束 -->    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="main mhead clearfix">
            <div class="mt10" style="height:28px; margin-bottom:10px;">
                <div class="l">
                    <a href="javascript:void(0);" class="but2"  onclick="cate_order();" >修改排序</a>
                    <a href="javascript:void(0);" class="but2" onclick="cate_show(0,0);" >添加分类</a>
					<a href="javascript:void(0);" class="but2"  onclick="cate_cdata();" >修正数量</a>
                </div> 
            </div>
        </div>
        <div class="mt10 clearfix">
            <form action="?m=save_corder" name="form_corder" method="post">
                <table class="tb" id='list'>
                    <tr>
                        <th align='center' width="50">排序</th>
                        <th align='center' width="80">分类ID</th>
                        <th class="alignleft">分类名称</th>
                        <th width=120>列表模版</th>
                        <th width=120>内容模版</th>
                        <th width=60>模型</th>
                        <th width=60>数据</th>
                        <th width=350></th>
                    </tr>
                    <?php html_cate_list($c->cate_tree()); ?>
                </table>
            </form>
        </div>
    </div><!-- 右侧主体内容结束 -->
        
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->
    

<!-- 隐藏表单半透明层 -->
<!-- 添加编辑分类 -->
<div  style="display:none;" id="addcate">
    <form method="post" action="?m=save" id="cate_form">
        <input type="hidden" id="cate_id" name="cate_id" class='ipt' value="0" />
        <div class="tab_table">
           <div class="btn_box" style="border:1px solid #ccc; border-width: 0 0 1px 0; margin:10px 0; display:inline-block;width:490px;">
                <a href="javascript:void(0);" class="current" id="bn1" >基本设置</a>
                <a href="javascript:void(0);" id="bn2">模版设置</a>
                <a href="javascript:void(0);" id="bn3">SEO设置</a>
           </div>
        </div>
        <div id="e_box">
            <table class="tb3" width="100%" id="tab1" style="clear:both" >
                <tr>
                    <td width="80">上级栏目：</td>
                    <td class="alignleft">
                      <select id="parent_id" name="parent_id">
                      <option value="0">≡ 作为一级栏目 ≡</option>
                      <?php echo($c->html_cate_select($c->cate_tree(),0,'')); ?>
                      </select>
                    </td>      
                </tr>
                 <tr>
                    <td>分类模型：</td>
                    <td class="alignleft">
                    <?php 
                        echo(vars::input_str(array('node'=>'catetype')));
                    ?>
                  
                    </td>      
                </tr>
                <tr>
                    <td>分类名称：</td>
                    <td class="alignleft"><input type="text" class="ipt" id="cname" onkeyup="get_pinyin();"  name="cname" value=""/></td>      
                </tr>
                <tr>
                    <td>字母别名：</td>
                    <td class="alignleft"><input type="text" class="ipt" id="cname_py" name="cname_py"  value=""/>
                    <a  href="javascript:void(0);" class="but2"  onclick="get_pinyin();" >获取拼音</a></td>
                </tr>
				<tr>
                    <td>分类显示：</td>
                    <td class="alignleft"><?php echo vars::input_str(array('node'=>'cat_show','type'=>'radio','default'=>'1')); ?></td>
                </tr>
				<tr>
                    <td>分类图片：</td>
                    <td class="alignleft"><input type="text" class="ipt" id="cimg" name="cimg" value=""/><script>C.create_upload_iframe('{"func":"callback_upload","thumb":{"width":"300","height":"300"}}');</script></td>
                </tr>
                <tr>
                    <td>排序：</td>
                    <td class="alignleft"><input type="text" class="ipt" id="corder" name="corder"  value="0"/></td>      
                </tr>
            </table>
               
            <table class="tb3" style="display:none;clear:both" id="tab2" >
                <?php $tpl=show_tpl();//print_r($tpl);?>
                <tr>
                    <td width="80">封面模版：</td>
                    <td class="alignleft">
                    <select id="tpl_index" >
                    <option value="">默认</option>
                    <?php if(!empty($tpl['cover'])){
                    foreach($tpl['cover'] as $v){ ?>
                    <option value="<?php echo $v;?>"><?php echo $v;?></option>
                    <?php } 
                    }?>
                    </select>
                    </td>      
                </tr>
                
                <tr>
                    <td>列表模板：</td>
                    <td class="alignleft">
                      <select id="tpl_listvar" >
                    <option value="">默认</option>
                    <?php if(!empty($tpl['list'])){
                    foreach($tpl['list'] as $v){ ?>
                    <option value="<?php echo $v;?>"><?php echo $v;?></option>
                    <?php } 
                    }?>
                    </select>
                    </td>      
                </tr>
                <tr>
                    <td>详细模版：</td>
                    <td class="alignleft">
                     <select id="tpl_content" >
                    <option value="">默认</option>
                    <?php if(!empty($tpl['content'])){
                    foreach($tpl['content'] as $v){ ?>
                    <option value="<?php echo $v;?>"><?php echo $v;?></option>
                    <?php } 
                    }?>
                    </select>
                    </td>      
                </tr>
                
               
             </table>
             
             <table class="tb3" style="display:none;clear:both" id="tab3">
                <tr>
                    <td width="80">SEO标题：</td>
                    <td class="alignleft" >
                        <textarea id="ctitle" name="ctitle" class="ipt mar10" style="width:360px; height:40px;"></textarea>
                    </td>      
                </tr>
                <tr>
                    <td align="left">SEO关键词：</td>
                    <td class="alignleft">
                        <textarea id="ckey" name="ckey" class="ipt mar10" style="width:360px; height:40px;"></textarea>
                    </td>      
                </tr>
                <tr>
                    <td>SEO描述：</td>
                    <td class="alignleft">
                        <textarea id="cdesc" name="cdesc" class="ipt mar10" style="width:360px; height:40px;"></textarea>
                    </td>      
                </tr>  
            </table>   
        </div>

        <div style="margin:10px 0 0 90px;" >
          <a href=" javascript:void(0);" class="but2 ml5" id="subtn"  onclick="cate_save()" >确 定</a>
        </div>

    </form>
</div>
<!-- 移动分类 -->
<div id="movecate" style="display:none" >
    <table class="tb3">
        <input type="hidden" name="mycateid" id="mycateid" />
        <tr align="center">
            <td>当前分类：</td>
            <td><font color="green" id="catename"></font> &nbsp;&nbsp;<font color=red> * 内容分类之间移动，只移动数据</font></td>
        </tr>
        <tr>
            <td>移动到：</td>
            <td class="alignleft">
                <select id="moveid" name="moveid">
                    <option value="0">≡ 作为一级栏目 ≡</option>
                    <?php echo($c->html_cate_select($c->cate_tree(),0,'')); ?>
                </select>
            </td>
        </tr>
        <tr><td>&nbsp;</td><td><a href="javascript:void(0);" class="but2 ml5"  id="subtn"  onclick="cate_move()" >确 定</a></td></tr>
    </table>
</div>
</body>
</html>