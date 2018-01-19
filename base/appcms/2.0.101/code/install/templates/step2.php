 <?php include_once('templates/header.php');?>
    <div class="warp-content"> <!-- 主体内容 开始 -->
	  <div class="info">
		<div class="bor">
         <table border="0" cellspacing="1" cellpadding="4" class="list" name="table1" id="table1" align="center" >
                  <tr>
                    <thead>
				</thead><th width="155">检查项目</th>
				<th>当前环境</th>
				<th>APPCMS建议</th>
				<th width="100">功能影响</th>
                  </tr>
                  <tbody>
                  <tr>
                    <th>操作系统</th>
                    <td><?php echo php_uname();?></td>
                    <td>Windows_NT/Linux/Freebsd</td>
                    <td><span class="green" >√</span></td>
                  </tr>
                  <tr>
                    <th>WEB 服务器</th>
                    <td><?php echo $_SERVER['SERVER_SOFTWARE'];?></td>
                    <td>Apache/Nginx/IIS</td>
                    <td><span class="green" >√</span></td>
                  </tr>
                  <tr>
                    <th>PHP 版本</th>
                    <td>PHP <?php echo phpversion();?></td>
                    <td>PHP 5.0.0 及以上</td>
                    <td><?php if(phpversion() >= '5.0.0'){ ?><span class="green" >√</span><?php }else{ ?><font class="red">×&nbsp;无法安装</font><?php }?></font></td>
                  </tr>
                  <tr>
                    <th>MYSQL 扩展</th>
                    <td><?php if(extension_loaded('mysql')){ ?><span class="green">√</span><?php }else{ ?><span class="red">×</span><?php }?></td>
                    <td>必须开启</td>
                    <td><?php if(extension_loaded('mysql')){ ?><span class="green" >√</span><?php }else{ ?><font class="red">×&nbsp;无法安装</font><?php }?></td>
                  </tr>
                  
                  <tr>
                    <th>ICONV/MB_STRING 扩展</th>
                    <td><?php if(extension_loaded('iconv') || extension_loaded('mbstring')){ ?>√<?php }else{ ?>×<?php }?></td>
                    <td>必须开启</td>
                    <td><?php if(extension_loaded('iconv') || extension_loaded('mbstring')){ ?><span class="green">√</span><?php }else{ ?><font class="red">×&nbsp;字符集转换效率低</font><?php }?></td>
                  </tr>
                  
                  <tr>
                    <th>JSON扩展</th>
                    <td><?php if($PHP_JSON){ ?><span class="green">√</span><?php }else{ ?><font class="red">×</font><?php }?></td>
                    <td>必须开启</td>
                    <td><?php if($PHP_JSON){ ?><span class="green">√</span><?php }else{ ?><font class="red">×&nbsp;不只持json,<a href="http://pecl.php.net/package/json" target="_blank">安装 PECL扩展</a></font><?php }?></td>
                  </tr>
                  <tr>
                    <th>GD 扩展</th>
                    <td><?php if($PHP_GD){ ?><span class="green">√</span> （支持 <?php echo $PHP_GD;?>）<?php }else{ ?><font class="red">×</font><?php }?></td>
                    <td>建议开启</td>
                    <td><?php if($PHP_GD){ ?><span class="green">√</span><?php }else{ ?><font class="red">×&nbsp;不支持缩略图和水印</font><?php }?></td>
                  </tr>                                    
                  <tr>
                    <th>ZLIB 扩展</th>
                    <td><?php if(extension_loaded('zlib')){ ?><span class="green">√</span><?php }else{ ?>×<?php }?></td>
                    <td>建议开启</td>
                    <td><?php if(extension_loaded('zlib')){ ?><span class="green">√</span><?php }else{ ?><font class="red">×&nbsp;不支持Gzip功能</font><?php }?></td>
                  </tr>
                  <tr>
                    <th>FTP 扩展</th>
                    <td><?php if(extension_loaded('ftp')){ ?><span class="green">√</span><?php }else{ ?>×<?php }?></td>
                    <td>建议开启</td>
                    <td><?php if(extension_loaded('ftp')){ ?><span class="green">√</span><?php }elseif(ISUNIX){ ?><font class="red">×&nbsp;不支持FTP形式文件传送</font><?php }?></td>
                  </tr>
                                    
                  <tr>
                    <th>allow_url_fopen</th>
                    <td><?php if(ini_get('allow_url_fopen')){ ?><span class="green">√</span><?php }else{ ?><font class="red">×</font><?php }?></td>
                    <td>建议打开</td>
                    <td><?php if(ini_get('allow_url_fopen')){ ?><span class="green">√</span><?php }else{ ?><font class="red">×&nbsp;不支持保存远程图片</font><?php }?></td>
                  </tr>
				  
                  <tr>
                    <th>DNS解析</th>
                    <td><?php if($PHP_DNS){ ?><span class="green">√</span><?php }else{ ?><font class="red">×</font><?php }?></td>
                    <td>建议设置正确</td>
                    <td><?php if($PHP_DNS){ ?><span class="green">√</span><?php }else{ ?><font class="red">×&nbsp;不支持采集和保存远程图片</font><?php }?></td>
                  </tr>
                  </tbody>
                </table>
	    </div>
		
        <?php if($is_right) { ?>
            <a href="javascript:void(0);"  onClick="$('#install').submit();return false;" class="install-btn">下一步</a>
            <?php }else{ ?>
			<a onClick="alert('当前配置不满足Phpcms安装需求，无法继续安装！');" class="install-btn">检测不通过</a>
 			<?php }?>
            <a href="javascript:history.go(-1);"  class="install-btn">上一步</a>
			<form id="install" action="index.php?" method="get">
			<input type="hidden" name="step" value="3">
			</form>
	  </div>
    </div> <!-- 主体内容 结束 -->
</div>
<script type="text/javascript">
	 
</script>
</body>
</html>
