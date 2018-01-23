 <?php include_once('templates/header.php');?>
    <div class="warp-content"> <!-- 主体内容 开始 -->
	  <div class="info">
		  <div class="bor">
			<table>
				<thead>
				</thead><th>目录</th>
				<th>系统要求</th>
				<th>现在状态</th>
			 
			 <tbody>
				<?php
if(!empty($dirs)){
	foreach($dirs as $k =>$v){
	?>	
  <tr align="center" >
                    <th align="left" style="text-indent:185px;"><?php echo is_dir($v['path']) && substr($k, -1) != '/' ? $k . '/' : $k ;?></th>
                    <th align="center" style="width:180px;"><font >可写</font></th>
                    <th align="center" style="width:180px;"><?php if($v['iswrite']){echo '<font class="green">√可写</font>';}else{ echo '<font class="red">×&nbsp;不可写</font>';} ?></th>
                  </tr>

<?php		
		}
	
	}


?>
			 </tbody>
			</table>
		  </div>
          
          <?php if($is_right) { ?>
            <a href="javascript:void(0);"  onClick="$('#install').submit();return false;" class="install-btn">下一步</a>
            <?php }else{ ?>
			
            <a onClick="alert('当前配置不满足appcms安装需求，无法继续安装！');" class="install-btn">检测不通过</a>
 			<?php }?>
            <a href="javascript:history.go(-1);"  class="install-btn">上一步</a>
			<form id="install" action="index.php?" method="get">
			<input type="hidden" name="step" value="4">
			</form>
	  </div>
    </div> <!-- 主体内容 结束 -->
</div>
<script type="text/javascript">
	 
</script>
</body>
</html>
