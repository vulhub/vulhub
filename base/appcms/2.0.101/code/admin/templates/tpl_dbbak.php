<?php require_once(dirname(__FILE__).'/inc_header.php');?>
<script language="javascript" type="text/javascript" src="templates/css/js/set.js" ></script>
<script language="javascript" type="text/javascript" src="templates/css/js/dbbak.js" ></script>
<style>
    #title_colorpanel table tr{ line-height:inherit; border-bottom:none;}
    a.close-own {
       display: block;width: 16px;height: 16px;position: absolute;outline: none;right: 7px;top: 8px;text-indent: 200px;overflow: hidden;
    }
    .tb3 tr {border:1px solid #f1f1f1; border-width: 0 0 1px 0;}
    a.close-own:hover{background-position: left -46px}
</style>
<?php require_once (dirname(__FILE__).'/inc_top.php');?>
<!-- 加载左侧导航栏 -->
<?php require_once(dirname(__FILE__)."/inc_menu.php");?>
<!-- 右侧区域开始 -->
<div class="right_body"> 
    <!-- 当前位置开始 -->
    <div class="snav">您的位置：<a href="frame.php">管理首页</a> » 数据库管理</div><!-- 当前位置结束 -->
    
    <!-- 右侧主体内容开始 -->
    <div class="mbody">
        <div class="main " id="mbody" style="margin-top:0">
            <div class="tab_table">
               <div class="btn_box" style="padding-left:0px;height:28px;width:100%; clear:both;border:1px solid #ccc; border-width:0 0 1px 0;margin-bottom:5px;">
                    <a href="javascript:void(0);" class="current" id="bn1" >数据库备份</a>
                    <a href="javascript:void(0);" id="bn2">数据库还原</a>
                </div>
            </div>
            <div id="e_box"> 
                <form id="set" >
                    <div id="tab1">
                        <?php
						foreach ($page['list'] as $v){
						?>
							<div style="float: left; width:20%; text-align:left; padding: 15px; line-height: 10%;">
							 <span style="height:30px;" id="list<?php echo $v; ?>"><input type="checkbox" name="table[]" value="<?php echo $v; ?>" id="<?php echo $v; ?>"> <label for="<?php echo $v; ?>"><?php echo $v; ?></label></span>
							</div>
						<?php
						}
						?>
						<div style="clear:both;"></div>
						<div style=" text-align:center;">
							<input type="radio" name="RadioGroup1" id='chkAll' onclick='CheckAll(this.form)'>
							<label for="chkAll">全选</label>
							<input type="radio" name="RadioGroup1" id="chkallno"  onclick='CheckAllNO(this.form)' checked>
							<label for="chkallno">不选</label>&nbsp;&nbsp;
							<input name="submit" class="but2" value="备份" onclick="backall('dbbak.php?m=save');"type="button" />
						</div>
                    </div>
					
					<div id="tab2">
                        <table class="tb3" style="clear:both;width:100%;" >
                            <thead>
								<tr>
									<th align="left">版本</th>
									<th align="left">时间</th>
									<th align="left">数据库大小</th>
									<th align="left">操作</th>
								</tr>
							</thead>
							<tbody>
								<?php
								
								if(count($page['rows'])>0){
							?>
							<tr>
                                <td><?php echo $page['rows']['version']; ?></td>
								<td><?php echo $page['rows']['time']; ?></td>
								<td><?php echo $page['rows']['dbsize']; ?></td>
								<td><a class="but2" onclick="backin('dbbak.php?m=backin')">导入</a> <a class="but2" onclick="backdel()">删除</a></td>
                            </tr>
							
							<?php
								
							} else {	
							?>
								<tr>
									<td width="160">没有还原数据库文件</td>
									<td class="alignleft"></td>
								</tr>
							<?php
								}
							?>
							</tbody>
                        </table>
                    </div>

                </form> 
            </div>
        </div>
		<div>
            <iframe id="autobackiframe" name="autobackiframe" src="about:blank"  width="100%" height="230" scrolling=""  frameborder="0" style="overflow-x: hidden; overflow-y: auto; "></iframe>
        </div>
    </div><!-- 右侧主体内容结束 -->
    <?php require_once(dirname(__FILE__)."/inc_footer.php");?>

</div><!-- 右侧区域结束 -->

<!-- 隐藏表单半透明层 -->
</body>
</html>