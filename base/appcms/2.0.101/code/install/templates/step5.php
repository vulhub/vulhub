 <?php include_once('templates/header.php');?>
    <div class="warp-content"> <!-- 主体内容 开始 -->
	  <div class="finish-info">
		 <p class="line-t-10"></p>
		<?php
           if($isok==0){ ?>
               <div class="finish">
			<br />
			<br />
			<br />
			 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	安装appcms 失败
			<br />
			<br />
			<br />
			<a>向开发团队获取帮助</a>
			<br />
			<br />
			<br />
			<a>建议删除安装文件夹</a>
		 </div>
         <?php }else{?>
         <div class="finish">
			<br />
			<br />
			<br />
			 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	恭喜您已经成功安装 AppCMS
			<br />
			<br />
			<br />
			<a href="../admin/" target="_blank">管理首页</a>&nbsp;&nbsp;
            <a href="../" target="_blank">网站首页</a>
			<br />
			<br />
			<br />
			<a>建议删除目录下的&nbsp; install&nbsp;,&nbsp;data&nbsp; 文件夹</a>
		 </div>
         <?php }?>
	  </div>
    </div> <!-- 主体内容 结束 -->
</div>
<script type="text/javascript">
	 
</script>
</body>
</html>
<div style="display:none">
<script type="text/javascript">var cnzz_protocol = (("https:" == document.location.protocol) ? " https://" : " http://");document.write(unescape("%3Cspan id='cnzz_stat_icon_1000067704'%3E%3C/span%3E%3Cscript src='" + cnzz_protocol + "s22.cnzz.com/z_stat.php%3Fid%3D1000067704' type='text/javascript'%3E%3C/script%3E"));</script>
</div>