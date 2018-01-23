</head>
<body>
<!-- 修改密码，全部页面公用 -->
<div id="edit_pass" style="display:none;">
    <table class="tb3">
        <input type="hidden" id="action" value=""><input type="hidden" id="uidnew" value="<?php echo $_SESSION['uid'];?>" />
        <tr><td>旧 密 码：</td><td><input type="password" id="upassnew" class="ipt" value="" style="width:200px;"></td><td>&nbsp;<span id="upass_tips"></span></td></tr>
        <tr><td>新 密 码：</td><td><input type="password" id="new_upass" class="ipt" value="" style="width:200px;"></td><td>&nbsp;<span id="upass_tips"></span></td></tr>
        <tr><td>确认密码：</td><td><input type="password" id="re_upass" class="ipt" value="" style="width:200px;"></td><td>&nbsp;<span id="upass_tips"></span></td></tr>
        <tr><td></td><td colspan="2"><a href="javascript:void(0);" id="btn_edit_pass" onClick="edit_pass();" class="but2">修改资料</a>&nbsp;&nbsp;<span class="div_alert"></span></td></tr>
    </table>
</div>
<!-- 顶部开始 -->
<div class="header-wrapper" id="top"> 
    <div class="header">
        <div class="logo">
            <a href="frame.php" class="cms"></a>
        </div>    
        <div class="l version"></div>
        <div class="top-nav">
            <b class="user"><?php echo $_SESSION['uname']?>&nbsp;&nbsp;</b>
            <a href="collect.php" style="color:yellow">添加应用</a>
            <a href="frame.php">管理首页</a>
            <a href="<?php echo(SITE_PATH);?>" target="_blank">网站首页</a>
            <a href="javascript:void(0);" onClick="user_show(<?php echo $_SESSION['uid']?>);">修改密码</a>
            <a href="javascript:void(0);" onClick="logout();">安全退出</a>
        </div>
     </div>
</div> 
<!-- 顶部结束 -->
