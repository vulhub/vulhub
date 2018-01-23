<?php
    require_once(dirname(__FILE__)."/../core/init.php");
    $upload_server= SITE_PATH."upload/";
    // 上传安全验证字符串
    $verify=helper::encrypt(UPLOAD_CODE.strtotime(date('Y-m-d H:i:s')),UPLOAD_KEY);
    $params=$_GET['params'];
    $params=preg_replace('~(\\\")~','"',$params);
    $json=json_decode($params);
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo SITE_NAME;?>后台管理</title>
<style>
body{cursor:default;}
.input-file {overflow:hidden;position:relative;background:#eee;border:1px solid #ccc;cursor:default;width:70px;height:25px;line-height:25px; margin-top:1px\9; height:26px\9; font-size:12px;display:inline-block;text-align:center;color:blue;}
.input-file:hover{background:#ddd;cursor:default;}
.input-file input{opacity:0;filter:alpha(opacity=0);font-size:100px;position:absolute;top:0;right:0;cursor:default;}
</style>
<script language="javascript" type="text/javascript" src="<?php echo SITE_PATH?>templates/lib/jquery-1.7.1.min.js" ></script>
<script type="text/javascript"> 
 $(document).ready(function() { 
     //提交上传
    $('#file').bind('change', function() {
        $('#form').submit();
    });
}) 
//回调通知
function callback_upload(ret){
    window.parent.<?php echo($json->func); ?>(ret);
}
</script>
</head>
<body> 
    <form action='<?php echo($upload_server); ?>upload_file.php?params=<?php echo urlencode($params);?>&v=<?php echo($verify);?>' id="form" name="form" enctype="multipart/form-data" method="post" target="hidden_frame">   
       <a class="input-file">上传文件<input type="file" id="file" name="file" size="1" style="width:70px;cursor:default;height:25px;line-height:25px;"></a>
       <iframe name="hidden_frame" id="hidden_frame" frameborder="no" border="0″ marginwidth="0″ marginheight="0" scrolling="no" allowtransparency="yes"></iframe>
   </form> 
</body>
</html>