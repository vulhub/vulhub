 <?php include_once('templates/header.php');?>
    <div class="warp-content"> <!-- 主体内容 开始 -->
	  <div class="info">
		 <div class="bor">
		 <p class="line-t-10"></p>
			<form id="install" action="index.php?" method="get">	
			<input type="hidden" name="step" value='5' />
            <fieldset class="fld">
				<legend>数据库信息</legend>
				<table>
					<tbody>
						<tr>
							<th width="200">数据库主机：</th>
							<td><input type="text"  id="host" name="host" value="<?php echo DB_HOST;?>" /></td>
						</tr>
						<tr>
							<th>用户名：</th>
							<td><input type="text" id="dbuname" name='dbuname'  value="<?php echo DB_USERNAME;?>" /></td>
						</tr>
						<tr>
							<th>数据库密码：</th>
							<td><input type="text" id="dbpass" name="dbpass" value="<?php echo DB_PASS;?>" /></td>
						</tr>
						<tr>
							<th>数据库名称：</th>
							<td><input type="text" id="dbname" name="dbname"  value="<?php echo DB_DBNAME;?>" /> </td>
						</tr>
						<tr>
							<th>表前缀：</th>
							<td><input type="text" id="dbprefix" name="dbprefix"  value="<?php echo TB_PREFIX;?>" /></td>
						</tr>
						<tr>
							<th>数据库编码：</th>
							<td><input type="text" id="dbcharset" name="dbcharset" value="utf8" readonly /></td>
						</tr>
					</tbody>
				</table>
			</fieldset>
			<br /> 
			<fieldset class="fld">
				<legend>管理员信息</legend>
				<table>
					<tbody>
						<tr>
							<th width="200">账号：</th>
							<td><input type="text" id="aduname" name="aduname"  value="admin" /> * 建议改为其他名字</td>
						</tr>
						<tr>
							<th>密码：</th>
							<td><input type="password"  id="adpass" value="" name="adpass" /> * 后台登录密码</td>
						</tr>
						<tr>
							<th>安全码：</th>
							<td><input type="text"  id="safecode" name="safecode"  value="" /> <font color=red>* 增加后台登录安全性，可留空</font></td>
						</tr>
					</tbody>
				</table>
			</fieldset>
			<p class="line-t-10"></p>
		 </div>
         </form>
         
		 <a href="javascript:void(0);"  onClick="testdb();return false;" class="install-btn">下一步</a>
         <a href="javascript:history.go(-1);"  class="install-btn">上一步</a>
	  </div>
    </div> <!-- 主体内容 结束 -->
    <br><br><br>
</div>
<script type="text/javascript">
	 
</script>
</body>
</html>




<script>

function testdb(){
	//先检查是否信息都填完全了
	var host=$("#host").val();
	var dbuname=$("#dbuname").val();
	var dbpass=$("#dbpass").val();
	var dbprefix=$("#dbprefix").val();
	var dbcharset=$("#dbcharset").val();
	var dbname=$("#dbname").val();
	var aduname=$("#aduname").val();
	var adpass=$("#adpass").val();
	var emial=$("#emial").val();
	if(adpass==''){ alert('用户密码不能为空！');return false;}

	/*
	var inputlen=$("input").size();
	   $.each($("input"),function(i,v){
		  if($("input")[i].value==''){
			  alert('还有信息没有填完全');
			  
			  
			  };
		   }) */
	
	$.ajax({
		type:"get",
		url:"index.php?step=10",
		data:{"host":host,"dbuname":dbuname,"dbpass":dbpass,"dbprefix":dbprefix,"dbcharset":dbcharset,"dbname":dbname,"aduname":aduname,"adpass":adpass},
		success:function(json){
			 var jieguo=eval('('+json+')');
			//var jieguo=eval('('+json+')')
			if(jieguo.code=='0'){
				//检测通过 跳转到下一步
					$("#install").submit();
				}else{
					C.alert.opacty({'content':'<div style="margin-top:10px;">'+jieguo.msg+'</div>'});
				}
			
			}

		})
	
	
	}


</script>